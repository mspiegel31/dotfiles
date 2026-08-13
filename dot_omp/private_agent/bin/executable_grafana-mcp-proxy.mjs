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

async function main() {
  const proxyMgr = new TshProxyManager();
  await proxyMgr.ensureRunning();

  const bridge = new StdioProxy(URL);
  bridge.start();
  await proxyMgr.startHealthCheck(bridge);

  await bridge.done();
  process.exit(0);
}

// ──────────────────────────────────────────────
// TshProxyManager — lifecycle for the shared
// `tsh proxy mcp` listener on the local port.
// ──────────────────────────────────────────────

class TshProxyManager {

  async ensureRunning() {
    if (await this.#isProxyRunning()) {
      log(`Proxy already running on port ${PORT}`);
      const validation = await this.#validateEndpoint();
      if (!validation.ok) {
        log(`WARNING: existing proxy is broken (${validation.error}), restarting...`);
        await this.#killExistingProxy();
        if (!await this.#startAndWaitForProxy()) process.exit(1);
      }
    } else {
      if (!await this.#startAndWaitForProxy()) process.exit(1);
    }
  }

  async startHealthCheck(bridge) {
    // The tsh listener is deliberately NOT torn down on exit:
    // it is shared by every omp session.
  setInterval(async () => {
      if (!await this.#isProxyRunning()) {
        log("Health check: tsh proxy is down, restarting...");
        if (await this.#startAndWaitForProxy()) {
          log("Health check: tsh proxy restored");
          await bridge.restoreSseStream();
        } else {
          log("Health check: failed to restart tsh proxy");
        }
      }
    }, HEALTH_CHECK_INTERVAL_MS);
  }

  // ── Proxy lifecycle (private helpers) ──

  async #startAndWaitForProxy() {
    const child = this.#spawnTshProxy();
    if (!await this.#waitForProxy(child)) {
      log("ERROR: tsh proxy did not start within 15s");
      if (child._stderr?.buf) log(`tsh stderr: ${child._stderr.buf.trim()}`);
      try { process.kill(child.pid); } catch {}
      tryUnlinkSync(PIDFILE);
      return false;
    }
    const validation = await this.#validateEndpoint();
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

  async #isProxyRunning() {
    try {
      await execFileAsync("lsof", ["-iTCP:" + PORT, "-sTCP:LISTEN", "-t"]);
      return true;
    } catch {
      return false;
    }
  }

  async #killExistingProxy() {
    try {
      const { stdout } = await execFileAsync("lsof", ["-iTCP:" + PORT, "-sTCP:LISTEN", "-t"]);
      for (const pid of stdout.trim().split("\n").filter(Boolean)) {
        try { process.kill(parseInt(pid, 10)); } catch {}
      }
      await new Promise((r) => setTimeout(r, 1000));
    } catch {}
    tryUnlinkSync(PIDFILE);
  }

  async #validateEndpoint() {
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

  #spawnTshProxy() {
    log(`Starting tsh proxy mcp ${APP_NAME} on port ${PORT} (cluster: ${CLUSTER})...`);
    const child = spawn("tsh", ["proxy", "mcp", APP_NAME, "-p", PORT], {
      stdio: ["ignore", "ignore", "pipe"],
      detached: true,
      env: { ...process.env, TELEPORT_CLUSTER: CLUSTER },
    });
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

  async #waitForProxy(child) {
    for (let i = 0; i < 30; i++) {
      if (await this.#isProxyRunning()) {
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
}

// ──────────────────────────────────────────────
// StdioProxy — stdio ↔ Streamable HTTP bridge
// (MCP 2025-03-26 transport).
// supergateway was used before: its streamableHttp
// client swallows initialize/tools/list responses
// from this endpoint. This does the transport by
// hand — no dependency needed.
// ──────────────────────────────────────────────

class StdioProxy {
  #url;
  #sessionId = null;
  #protocolVersion = null;
  #sseStarted = false;
  #doneResolve = null;
  #donePromise = null;

  constructor(url) {
    this.#url = url;
    this.#donePromise = new Promise((r) => { this.#doneResolve = r; });
  }

  done() {
    return this.#donePromise;
  }

  // Public: re-open the SSE stream after a health-check restart.
  async restoreSseStream() {
    this.#sseStarted = false;
    await this.#openServerStream();
  }

  start() {
    process.stdin.setEncoding("utf8");
    (async () => {
      try {
        for await (const msg of this.#readStdinMessages()) {
          if (msg.method === "initialize" && msg.params?.protocolVersion) {
            this.#protocolVersion = msg.params.protocolVersion;
          }
          await this.#post(msg);
          if (msg.method === "notifications/initialized") {
            void this.#openServerStream();
          }
        }
      } finally {
        this.#doneResolve();
      }
    })();
  }

  // ── Transport helpers (private) ──

  #headers() {
    const h = {
      "Content-Type": "application/json",
      "Accept": "application/json, text/event-stream",
    };
    if (this.#sessionId) h["Mcp-Session-Id"] = this.#sessionId;
    if (this.#protocolVersion) h["MCP-Protocol-Version"] = this.#protocolVersion;
    return h;
  }

  #send(msg) {
    process.stdout.write(JSON.stringify(msg) + "\n");
  }

  async #post(msg) {
    let res;
    try {
      res = await fetch(this.#url, { method: "POST", headers: this.#headers(), body: JSON.stringify(msg) });
    } catch (err) {
      log(`POST failed: ${err.message}`);
      if (msg.id !== undefined) {
        this.#send({ jsonrpc: "2.0", id: msg.id, error: { code: -32000, message: `proxy transport error: ${err.message}` } });
      }
      return;
    }

    const newSession = res.headers.get("mcp-session-id");
    if (newSession) this.#sessionId = newSession;

    if (!res.ok) {
      const body = await res.text().catch(() => "(unreadable)");
      log(`HTTP ${res.status} for ${msg.method}: ${body.slice(0, 500)}`);
      if (msg.id !== undefined) {
        this.#send({ jsonrpc: "2.0", id: msg.id, error: { code: -32000, message: `HTTP ${res.status}: ${body.slice(0, 500)}` } });
      }
      return;
    }

    // 202 Accepted answers notifications and has no body.
    if (res.status === 202 || !res.body) return;

    const type = res.headers.get("content-type") || "";
    if (type.includes("text/event-stream")) {
      for await (const out of this.#readSse(res.body)) this.#send(out);
    } else {
      const text = await res.text();
      for (const line of text.split("\n")) {
        const trimmed = line.trim();
        if (trimmed) process.stdout.write(trimmed + "\n");
      }
    }
  }

  async #openServerStream() {
    if (this.#sseStarted) return;
    this.#sseStarted = true;
    try {
      const res = await fetch(this.#url, { method: "GET", headers: { ...this.#headers(), Accept: "text/event-stream" } });
      if (!res.ok || !res.body) return;
      for await (const msg of this.#readSse(res.body)) this.#send(msg);
    } catch (err) {
      log(`server stream ended: ${err.message}`);
    }
  }

  async *#readStdinMessages() {
    let buf = "";
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

  async *#readSse(body) {
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
}

function log(msg) {
  const line = `${new Date().toISOString()} [grafana-mcp-proxy] ${msg}\n`;
  process.stderr.write(line);
  try { appendFileSync(LOG_FILE, line); } catch {}
}

function tryUnlinkSync(path) {
  try { unlinkSync(path); } catch {}
}

main().catch(() => process.exit(1));
