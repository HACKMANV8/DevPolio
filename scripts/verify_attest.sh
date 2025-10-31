#!/usr/bin/env bash
set -euo pipefail
ATT="${1:-}"
if [ -z "$ATT" ]; then
  echo "Usage: $0 <attestation.json>"
  exit 1
fi
PUB="${ATT%.json}.pub.pem"
SIG="${ATT%.json}.sig"
if [ ! -f "$ATT" ] || [ ! -f "$PUB" ] || [ ! -f "$SIG" ]; then
  echo "Missing files. Need: $ATT , ${ATT%.json}.sig , ${ATT%.json}.pub.pem"
  exit 2
fi
echo "[*] verifying $ATT with $PUB"
openssl dgst -sha256 -verify "$PUB" -signature "$SIG" "$ATT" && echo "Signature OK" || (echo "Signature FAILED" && exit 3)
echo "----- attestation -----"
jq . "$ATT" || cat "$ATT"
