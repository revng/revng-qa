# shellcheck shell=bash
# Strict bash
set -euo pipefail

COMMAND_NAME="$1"
OUTPUT="${2-}"
if [[ -n "$OUTPUT" ]]; then
    _OUTPUT_ABSOLUTE=$(realpath "$OUTPUT")
fi
TEMPORARIES_LOG=$(mktemp --tmpdir tmp."$COMMAND_NAME"-temp-log.XXXXXXXXXX)

# Per-rule watchdog. The default (600s) catches the few revng
# invocations that occasionally wedge (e.g. s390x project init,
# native-dynamic recompile-isolated) so the ninja build can move
# on instead of hanging. Override via REVNG_TEST_TIMEOUT=N (env).
TIMEOUT_S="${REVNG_TEST_TIMEOUT:-600}"
_PARENT_PID=$$
# Redirect the watchdog's stdin/stdout/stderr to /dev/null. Without
# this the backgrounded subshell (and the `sleep` it execs) inherits
# the ninja-rule pipes; even after the rule's bash exits, the still-
# alive `sleep` keeps the pipe writer-side open, so ninja's read()
# on the rule's stdout never sees EOF and the build wedges.
(
    sleep "$TIMEOUT_S"
    # SIGTERM the actual workload (revng2 / revng / …): the child(ren)
    # of the parent shell. Just killing the parent bash isn't enough —
    # the workload would be reparented and keep running.
    for _c in $(ps -o pid= --ppid "$_PARENT_PID" 2>/dev/null); do
        kill -TERM "$_c" 2>/dev/null || true
    done
    kill -TERM "$_PARENT_PID" 2>/dev/null || true
    sleep 10
    # Escalate to SIGKILL for anything that ignored SIGTERM.
    for _c in $(ps -o pid= --ppid "$_PARENT_PID" 2>/dev/null); do
        kill -KILL "$_c" 2>/dev/null || true
    done
    kill -KILL "$_PARENT_PID" 2>/dev/null || true
) </dev/null >/dev/null 2>&1 & _TIMEOUT_WATCHDOG_PID=$!
disown $_TIMEOUT_WATCHDOG_PID 2>/dev/null || true

function temp() {
    NEW_FILE="$(mktemp --tmpdir tmp."$COMMAND_NAME".XXXXXXXXXX "$@")"
    echo "$NEW_FILE" >> "$TEMPORARIES_LOG"
    echo "$NEW_FILE"
}

function at_exit() {
    # Stop the watchdog before any cleanup so it doesn't fire mid-exit.
    kill "$_TIMEOUT_WATCHDOG_PID" 2>/dev/null || true
    wait "$_TIMEOUT_WATCHDOG_PID" 2>/dev/null || true

    # Cleanup all temporary files created through temp
    while IFS= read -r TEMPORARY; do
        rm -rf "$TEMPORARY"
    done < "$TEMPORARIES_LOG"
    rm -f "$TEMPORARIES_LOG"

    # If this command has any output, ensure it has been produced
    if [[ -n "$OUTPUT" && ! -e "$_OUTPUT_ABSOLUTE" ]]; then
        echo "Output not produced" > /dev/stderr
        exit 1
    fi
}

trap at_exit EXIT

if [[ $OUTPUT == */ ]]; then
    rm -rf "$OUTPUT"
    mkdir -p "$OUTPUT"
fi
