#!/bin/sh
set -eu

# HerdR installs its OMP integration in the default OMP agent directory. The
# managed personal and work profile extension directories both symlink to
# ~/.agents/extensions, so copy the official installed asset there once for
# every profile to discover.
if ! command -v herdr >/dev/null 2>&1; then
  printf '%s\n' 'herdr not found; skipping OMP integration install.' >&2
  exit 0
fi

if ! command -v omp >/dev/null 2>&1; then
  printf '%s\n' 'omp not found; skipping HerdR OMP integration install.' >&2
  exit 0
fi

# An OMP session exports PI_CODING_AGENT_DIR for its selected profile. Unset
# Pi directory overrides so HerdR's built-in installer targets ~/.omp/agent
# instead of rejecting the shared profile extension directory.
env -u PI_CODING_AGENT_DIR -u PI_CONFIG_DIR herdr integration install omp

source="$HOME/.omp/agent/extensions/herdr-omp-agent-state.ts"
destination_dir="$HOME/.agents/extensions"
destination="$destination_dir/herdr-omp-agent-state.ts"

if [ ! -f "$source" ]; then
  printf '%s\n' "HerdR reported success but did not create $source" >&2
  exit 1
fi

mkdir -p "$destination_dir"
if [ -f "$destination" ] && cmp -s "$source" "$destination"; then
  printf '%s\n' 'HerdR OMP integration is current for all managed profiles.'
  exit 0
fi

cp "$source" "$destination"
chmod 0644 "$destination"
printf '%s\n' "Installed HerdR OMP integration at $destination"
