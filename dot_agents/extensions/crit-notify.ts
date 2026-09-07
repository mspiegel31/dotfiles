import type { ExtensionAPI } from "@oh-my-pi/pi-coding-agent";

// Parity port of crit's official opencode plugin
// (.opencode/plugins/crit.ts + lib/crit-wait-notify.js): toast when a
// blocking `crit` review wait starts, so the user notices even while the
// tool call is still running.
//
// Not ported: the system-prompt "Sharing" block gated on `crit config`'s
// share_url. That relies on before_provider_request's exact payload shape,
// which isn't published in omp's extension docs. The Sharing section lives
// unconditionally in ~/.agents/commands/crit.md instead.

const NON_WAIT_SUBCOMMANDS: Record<string, true> = {
  help: true,
  "--help": true,
  "-h": true,
  "--version": true,
  "-v": true,
  version: true,
  share: true,
  fetch: true,
  unpublish: true,
  install: true,
  config: true,
  check: true,
  pr: true,
  pull: true,
  push: true,
  comment: true,
  comments: true,
  "plan-hook": true,
  story: true,
  auth: true,
  stop: true,
  status: true,
  stats: true,
  cleanup: true,
  _serve: true,
};

function isCritWaitCommand(command: string): boolean {
  if (typeof command !== "string") return false;
  const withoutEnv = command
    .trim()
    .replace(/^(?:[A-Za-z_][A-Za-z0-9_]*=\S*\s+)+/, "");
  if (!/^(?:\.\/)?crit(?:\s|$)/.test(withoutEnv)) return false;

  const rest = withoutEnv.replace(/^(?:\.\/)?crit\s*/, "").trim();
  if (rest === "") return true;

  const first = rest.split(/\s+/)[0];
  return !NON_WAIT_SUBCOMMANDS[first];
}

export default function critNotifyExtension(pi: ExtensionAPI) {
  pi.setLabel("Crit review notifications");

  pi.on("tool_call", async (event, ctx) => {
    try {
      if (event.toolName !== "bash") return;
      if (!isCritWaitCommand(String(event.input?.command ?? ""))) return;
      if (!ctx.hasUI) return;
      ctx.ui.notify(
        "Crit is waiting for review — leave inline comments in the browser and click Finish Review.",
        "info",
      );
    } catch {}
  });
}
