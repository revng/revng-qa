# Strict bash
set -euo pipefail

COMMAND_NAME="$1"
OUTPUT="${2-}"
TEMPORARIES=()

function temp() {
    NEW_FILE="$(mktemp --tmpdir tmp."$COMMAND_NAME".XXXXXXXXXX "$@")"
    TEMPORARIES+=("$NEW_FILE")
    echo "$NEW_FILE"
}

function at_exit() {
    # Cleanup all temporary files created through temp
    for TEMPORARY in "${TEMPORARIES[@]}"; do
        rm -rf "$TEMPORARY"
    done

    # If this command has any output, ensure it has been produced
    if test -n "$OUTPUT" && ! test -e "$OUTPUT"; then
        echo "Output not produced" > /dev/stderr
        exit 1
    fi
}

trap at_exit EXIT

if [[ $OUTPUT == */ ]]; then
    mkdir -p "$OUTPUT"
fi
