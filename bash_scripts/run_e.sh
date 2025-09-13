#!/usr/bin/env bash
# run_e.sh — Linux bash equivalent of run_e.bat

set -euo pipefail

# Go to repo root (script is in bat_scripts/)
SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" &>/dev/null && pwd)"
cd "$SCRIPT_DIR/.."

# Compile
g++ -O3 cpp_scripts/E.cpp -o e_prog

# Ensure output directory exists
mkdir -p output/e

echo "Processing all instances with E variant..."
shopt -s nullglob
found=0
for f in instances/*.txt; do
  found=1
  filename="$(basename "$f" .txt)"
  ./e_prog < "$f" > "output/e/$filename.out" 2> "output/e/$filename.score"
  if read -r score < "output/e/$filename.score"; then
    echo "  Score for $filename: $score"
  else
    echo "  Score for $filename: (no score)"
  fi
done
shopt -u nullglob

if [[ $found -eq 0 ]]; then
  echo "No instance files found in instances/."
fi

rm -f e_prog
echo "E outputs generated in output/e!"
