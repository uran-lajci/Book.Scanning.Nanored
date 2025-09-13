#!/usr/bin/env bash
# run_d.sh — Linux bash equivalent of run_d.bat, with a 10-minute per-instance timeout

set -euo pipefail

# Require GNU coreutils 'timeout'
if ! command -v timeout >/dev/null 2>&1; then
  echo "Error: 'timeout' command not found. Please install coreutils." >&2
  exit 1
fi

# Go to repo root (script is in bat_scripts/)
SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" &>/dev/null && pwd)"
cd "$SCRIPT_DIR/.."

# Compile
g++ -O3 cpp_scripts/D.cpp -o d_prog

# Ensure output directory exists
mkdir -p output/d

echo "Processing all instances with D variant..."
shopt -s nullglob
found=0
for f in instances/*.txt; do
  found=1
  filename="$(basename "$f" .txt)"
  out="output/d/$filename.out"
  scoref="output/d/$filename.score"

  # Run with a 10-minute timeout (SIGTERM at 10m, SIGKILL 10s later if needed)
  if timeout -k 10s 10m ./d_prog < "$f" > "$out" 2> "$scoref"; then
    if read -r score < "$scoref"; then
      echo "  Score for $filename: $score"
    else
      echo "  Score for $filename: (no score)"
    fi
  else
    status=$?
    if [[ $status -eq 124 ]]; then
      echo "  $filename can not be solved within 10 minutes"
    else
      echo "  $filename failed (exit code $status)"
    fi
    # Remove partial outputs for timed-out/failed runs
    rm -f "$out" "$scoref"
  fi
done
shopt -u nullglob

if [[ $found -eq 0 ]]; then
  echo "No instance files found in instances/."
fi

rm -f d_prog
echo "D outputs generated in output/d!"
