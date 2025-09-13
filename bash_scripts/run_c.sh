#!/usr/bin/env bash
# run_c.sh — Linux bash equivalent of run_c.bat

set -euo pipefail

# Go to repo root (script is in bat_scripts/)
SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" &>/dev/null && pwd)"
cd "$SCRIPT_DIR/.."

# Compile
g++ -O3 cpp_scripts/C.cpp -o c_prog

# Ensure output directory exists
mkdir -p output/c

echo "Processing all instances with C variant..."
shopt -s nullglob
found=0
for f in instances/*.txt; do
  found=1
  filename="$(basename "$f" .txt)"
  ./c_prog < "$f" > "output/c/$filename.out" 2> "output/c/$filename.score"
  if read -r score < "output/c/$filename.score"; then
    echo "  Score for $filename: $score"
  else
    echo "  Score for $filename: (no score)"
  fi
done
shopt -u nullglob

if [[ $found -eq 0 ]]; then
  echo "No instance files found in instances/."
fi

rm -f c_prog
echo "C outputs generated in output/c!"
