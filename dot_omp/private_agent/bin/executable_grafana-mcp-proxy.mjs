#!/usr/bin/env node
//
// stdio MCP front-end for the Teleport-fronted grafana-mcp-server.
//
// Why this exists instead of `tsh mcp connect grafana-mcp-server`:
// `tsh mcp connect` builds a *per-client* Teleport connection and reissues an
// app certificate on every start, so N concurrent omp sessions/subagents cause
// N cert reissues against ~/.tsh. Under that contention the client's 30s
// initialize budget is blown ("Connection to MCP server timed out after
// 30000ms" / "Transport closed"), and omp gives up on the server for the rest
// of the session. A single shared `tsh proxy mcp` listener is reused by every
// client, so only the first start pays the Teleport cost.
//
// Logs: ~/.cache/omp/grafana-mcp-proxy.log
//

import { spawn, execFile } from "node:child_process";
import { mkdirSync, appendFileSync, writeFileSync, unlinkSync, statSync } from "node:fs";
import { homedir, tmpdir } from "node:os";
import { join } from "node:path";
import { promisify } from "node:util";

const execFileAsync = promisify(execFile);

const PORT = process.env.GRAFANA_MCP_PORT || "8888";
const APP_NAME = process.env.GRAFANA_MCP_APP || "grafana-mcp-server";
const CLUSTER = process.env.TELEPORT_CLUSTER || "ue1-staging";
const PIDFILE = join(process.env.TMPDIR || tmpdir(), `tsh-proxy-${APP_NAME}.pid`);
const URL = `http://127.0.0.1:${PORT}/mcp`;

const HEALTH_CHECK_INTERVAL_MS = 10_000;
const LOG_MAX_BYTES = 1_048_576;

const LOG_DIR = join(homedir(), ".cache", "omp");
const LOG_FILE = join(LOG_DIR, "grafana-mcp-proxy.log");
try { mkdirSync(LOG_DIR, { recursive: true }); } catch {}
try {
  const { size } = statSync(LOG_FILE);
  if (size > LOG_MAX_BYTES) {
    writeFileSync(LOG_FILE, `--- log truncated at ${new Date().toISOString()} (was ${size} bytes) ---\n`);
  }
} catch {}

main().catch(() => process.exit(1));

async function main() {
  if (!await isProxyRunning()) {
    if (!await startAndWaitForProxy()) process.exit(1);
  } else {
    log(`Proxy already running on port ${PORT}`);
    // A listener left over from an expired Teleport session still accepts TCP,
    // so liveness is not enough — the MCP endpoint has to answer.
    const validation = await validateEndpoint();
    if (!validation.ok) {
      log(`WARNING: existing proxy is broken (${validation.error}), restarting...`);
      await killExistingProxy();
      if (!await startAndWaitForProxy()) process.exit(1);
    }
  }

  // The tsh listener is deliberately NOT torn down on exit: it is shared by
  // every omp session, and killing it here would break clients still using it.
  startHealthCheck();

  await bridgeStdio();
  process.exit(0);
}

// Minimal stdio <-> Streamable HTTP bridge (MCP 2025-03-26 transport).
// supergateway was used here before: its streamableHttp client swallows the
// initialize/tools/list responses from this endpoint, so nothing ever reaches
// stdout. This does the transport by hand instead of adding a dependency.
async function bridgeStdio() {
  let sessionId = null;
  let protocolVersion = null;
  let sseStarted = false;

  const send = (msg) => process.stdout.write(JSON.stringify(msg) + "\n");

  const headers = () => {
    const h = {
      "Content-Type": "application/json",
      "Accept": "application/json, text/event-stream",
    };
    if (sessionId) h["Mcp-Session-Id"] = sessionId;
    if (protocolVersion) h["MCP-Protocol-Version"] = protocolVersion;
    return h;
  };

  // Server-initiated messages (notifications, sampling) arrive on a GET stream.
  const openServerStream = async () => {
    if (sseStarted) return;
    sseStarted = true;
    try {
      const res = await fetch(URL, { method: "GET", headers: { ...headers(), Accept: "text/event-stream" } });
      if (!res.ok || !res.body) return;
      for await (const msg of readSse(res.body)) send(msg);
    } catch (err) {
      log(`server stream ended: ${err.message}`);
    }
  };

  const post = async (msg) => {
    let res;
    try {
      res = await fetch(URL, { method: "POST", headers: headers(), body: JSON.stringify(msg) });
    } catch (err) {
      log(`POST failed: ${err.message}`);
      if (msg.id !== undefined) {
        send({ jsonrpc: "2.0", id: msg.id, error: { code: -32000, message: `proxy transport error: ${err.message}` } });
      }
      return;
    }

    const newSession = res.headers.get("mcp-session-id");
    if (newSession) sessionId = newSession;

    if (!res.ok) {
      const body = await res.text().catch(() => "(unreadable)");
      log(`HTTP ${res.status} for ${msg.method}: ${body.slice(0, 500)}`);
      if (msg.id !== undefined) {
        send({ jsonrpc: "2.0", id: msg.id, error: { code: -32000, message: `HTTP ${res.status}: ${body.slice(0, 500)}` } });
      }
      return;
    }

    // 202 Accepted answers notifications and has no body.
    if (res.status === 202 || !res.body) return;

    const type = res.headers.get("content-type") || "";
    if (type.includes("text/event-stream")) {
      for await (const out of readSse(res.body)) send(out);
    } else {
      const text = await res.text();
      for (const line of text.split("\n")) {
        const trimmed = line.trim();
        if (trimmed) process.stdout.write(trimmed + "\n");
      }
    }
  };

  for await (const msg of readStdinMessages()) {
    if (msg.method === "initialize" && msg.params?.protocolVersion) {
      protocolVersion = msg.params.protocolVersion;
    }
    await post(msg);
    if (msg.method === "notifications/initialized") void openServerStream();
  }
}

async function* readStdinMessages() {
  let buf = "";
  process.stdin.setEncoding("utf8");
  for await (const chunk of process.stdin) {
    buf += chunk;
    let nl;
    while ((nl = buf.indexOf("\n")) !== -1) {
      const line = buf.slice(0, nl).trim();
      buf = buf.slice(nl + 1);
      if (!line) continue;
      try {
        yield JSON.parse(line);
      } catch {
        log(`dropping unparseable stdin line: ${line.slice(0, 200)}`);
      }
    }
  }
}

async function* readSse(body) {
  const decoder = new TextDecoder();
  let buf = "";
  for await (const chunk of body) {
    buf += decoder.decode(chunk, { stream: true });
    let sep;
    while ((sep = buf.search(/\r?\n\r?\n/)) !== -1) {
      const rawEvent = buf.slice(0, sep);
      buf = buf.slice(sep + (buf.slice(sep).startsWith("\r\n") ? 4 : 2));
      const data = rawEvent
        .split(/\r?\n/)
        .filter((l) => l.startsWith("data:"))
        .map((l) => l.slice(5).trim())
        .join("\n");
      if (!data) continue;
      try {
        yield JSON.parse(data);
      } catch {
        log(`dropping unparseable sse data: ${data.slice(0, 200)}`);
      }
    }
  }
}

async function startAndWaitForProxy() {
  const child = spawnTshProxy();
  if (!await waitForProxy(child)) {
    log("ERROR: tsh proxy did not start within 15s");
    if (child._stderr?.buf) log(`tsh stderr: ${child._stderr.buf.trim()}`);
    try { process.kill(child.pid); } catch {}
    tryUnlinkSync(PIDFILE);
    return false;
  }
  const validation = await validateEndpoint();
  if (!validation.ok) {
    log(`ERROR: tsh proxy started but MCP endpoint is broken: ${validation.error}`);
    if (child._stderr?.buf) log(`tsh stderr: ${child._stderr.buf.trim()}`);
    try { process.kill(child.pid); } catch {}
    tryUnlinkSync(PIDFILE);
    return false;
  }
  log("MCP endpoint validated OK");
  return true;
}

function startHealthCheck() {
  setInterval(async () => {
    if (!await isProxyRunning()) {
      log("Health check: tsh proxy is down, restarting...");
      if (await startAndWaitForProxy()) {
        log("Health check: tsh proxy restored");
      } else {
        log("Health check: failed to restart tsh proxy");
      }
    }
  }, HEALTH_CHECK_INTERVAL_MS);
}

function log(msg) {
  const line = `${new Date().toISOString()} [grafana-mcp-proxy] ${msg}\n`;
  process.stderr.write(line);
  try { appendFileSync(LOG_FILE, line); } catch {}
}

function tryUnlinkSync(path) {
  try { unlinkSync(path); } catch {}
}

async function killExistingProxy() {
  try {
    const { stdout } = await execFileAsync("lsof", ["-iTCP:" + PORT, "-sTCP:LISTEN", "-t"]);
    for (const pid of stdout.trim().split("\n").filter(Boolean)) {
      try { process.kill(parseInt(pid, 10)); } catch {}
    }
    await new Promise((r) => setTimeout(r, 1000));
  } catch {}
  tryUnlinkSync(PIDFILE);
}

async function validateEndpoint() {
  try {
    const controller = new AbortController();
    const timeout = setTimeout(() => controller.abort(), 10_000);
    const res = await fetch(URL, {
      method: "POST",
      headers: { "Content-Type": "application/json", "Accept": "application/json, text/event-stream" },
      body: JSON.stringify({
        jsonrpc: "2.0", id: "__healthcheck__",
        method: "initialize",
        params: { protocolVersion: "2024-11-05", capabilities: {}, clientInfo: { name: "healthcheck", version: "0.1.0" } },
      }),
      signal: controller.signal,
    });
    clearTimeout(timeout);
    if (!res.ok) {
      const body = await res.text().catch(() => "(unreadable)");
      return { ok: false, error: `HTTP ${res.status}: ${body}` };
    }
    return { ok: true };
  } catch (err) {
    return { ok: false, error: err.message };
  }
}

async function isProxyRunning() {
  try {
    await execFileAsync("lsof", ["-iTCP:" + PORT, "-sTCP:LISTEN", "-t"]);
    return true;
  } catch {
    return false;
  }
}

function spawnTshProxy() {
  log(`Starting tsh proxy mcp ${APP_NAME} on port ${PORT} (cluster: ${CLUSTER})...`);
  const child = spawn("tsh", ["proxy", "mcp", APP_NAME, "-p", PORT], {
    stdio: ["ignore", "ignore", "pipe"],
    detached: true,
    env: { ...process.env, TELEPORT_CLUSTER: CLUSTER },
  });
  // tsh reports auth failures (expired cert, role not found) only on stderr.
  const stderr = { buf: "" };
  child.stderr.on("data", (chunk) => {
    const text = chunk.toString();
    stderr.buf += text;
    for (const line of text.split("\n").filter(Boolean)) {
      log(`tsh: ${line}`);
    }
  });
  child.stderr.unref();
  child.unref();
  writeFileSync(PIDFILE, String(child.pid));
  child._stderr = stderr;
  return child;
}

async function waitForProxy(child) {
  for (let i = 0; i < 30; i++) {
    if (await isProxyRunning()) {
      log(`Proxy ready (pid ${child.pid})`);
      return true;
    }
    try { process.kill(child.pid, 0); } catch {
      log("ERROR: tsh proxy exited unexpectedly");
      tryUnlinkSync(PIDFILE);
      return false;
    }
    await new Promise((r) => setTimeout(r, 500));
  }
  return false;
}
