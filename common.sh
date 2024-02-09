# Strict bash
set -euo pipefail

COMMAND_NAME="$1"
OUTPUT="${2-}"
TEMPORARIES_LOG=$(mktemp --tmpdir tmp."$COMMAND_NAME"-temp-log.XXXXXXXXXX)

function temp() {
    NEW_FILE="$(mktemp --tmpdir tmp."$COMMAND_NAME".XXXXXXXXXX "$@")"
    echo "$NEW_FILE" >> "$TEMPORARIES_LOG"
    echo "$NEW_FILE"
}

function at_exit() {
    # Cleanup all temporary files created through temp
    while IFS= read -r TEMPORARY; do
        rm -rf "$TEMPORARY"
    done < "$TEMPORARIES_LOG"
    rm -f "$TEMPORARIES_LOG"

    # If this command has any output, ensure it has been produced
    if [[ -n "$OUTPUT" && ! -e "$OUTPUT" ]]; then
        echo "Output not produced" > /dev/stderr
        exit 1
    fi
}

trap at_exit EXIT

if [[ $OUTPUT == */ ]]; then
    rm -rf "$OUTPUT"
    mkdir -p "$OUTPUT"
fi
