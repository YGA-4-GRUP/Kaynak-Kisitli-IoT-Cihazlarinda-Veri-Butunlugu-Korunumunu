#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
BIN_NAME="iot-mitm-omnet"

cd "$ROOT_DIR"
mkdir -p results/figures

if [[ ! -f "Makefile" ]]; then
  opp_makemake -f --deep -O out -o "$BIN_NAME" -Isrc
fi
make

configs=(
  "WPAN-ChaCha"
  "WPAN-ASCON"
  "WPAN-SIMON"
  "BLE-ChaCha"
  "BLE-ASCON"
  "BLE-SIMON"
)

for cfg in "${configs[@]}"; do
  echo "Running $cfg"
  if [[ -n "${RUN_NUMBER:-}" ]]; then
    ./"$BIN_NAME" -u Cmdenv --cmdenv-express-mode=true -n src:simulations -f simulations/omnetpp.ini -c "$cfg" -r "$RUN_NUMBER"
  else
    ./"$BIN_NAME" -u Cmdenv --cmdenv-express-mode=true -n src:simulations -f simulations/omnetpp.ini -c "$cfg"
  fi
done

python3 scripts/analyze_results.py
python3 scripts/plot_results.py
