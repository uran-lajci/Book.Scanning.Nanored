#!/usr/bin/env bash
# run_bf.sh — Linux bash equivalent of run_bf.bat

set -euo pipefail

# Go to repo root (script is in bat_scripts/)
SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" &>/dev/null && pwd)"
cd "$SCRIPT_DIR/.."

# Compile
g++ -O3 cpp_scripts/BF.cpp -o bf_prog

# Ensure output directory exists
mkdir -p output/bf

echo "Processing all instances with BF variant..."
shopt -s nullglob
found=0
for f in instances/*.txt; do
  found=1
  filename="$(basename "$f" .txt)"
  ./bf_prog < "$f" > "output/bf/$filename.out" 2> "output/bf/$filename.score"
  if read -r score < "output/bf/$filename.score"; then
    echo "  Score for $filename: $score"
  else
    echo "  Score for $filename: (no score)"
  fi
done
shopt -u nullglob

if [[ $found -eq 0 ]]; then
  echo "No instance files found in instances/."
fi

rm -f bf_prog
echo "BF outputs generated in output/bf!"
