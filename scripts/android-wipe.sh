#!/usr/bin/env bash
set -euo pipefail
MODE="${1:-fastboot}"  # fastboot or adb
DEV="${2:-}"

LOG="/tmp/sentinel-android-$(date +%s).log"
echo "[*] android-wipe -> mode:$MODE dev:$DEV" | tee "$LOG"

if [ "$MODE" = "fastboot" ]; then
  echo "[*] Looking for fastboot devices" | tee -a "$LOG"
  fastboot devices 2>&1 | tee -a "$LOG" || true
  if [ -n "$DEV" ]; then
    echo "[*] Erasing userdata on $DEV" | tee -a "$LOG"
    fastboot -s "$DEV" erase userdata 2>&1 | tee -a "$LOG" || fastboot -s "$DEV" format userdata 2>&1 | tee -a "$LOG"
    SERIAL="$DEV"
  else
    # try to pick the first fastboot device (if any)
    SERIAL="$(fastboot devices 2>/dev/null | awk 'NR==1{print $1}' || true)"
    if [ -z "$SERIAL" ]; then
      echo "[!] No fastboot device found." | tee -a "$LOG"
      exit 1
    fi
    fastboot -s "$SERIAL" erase userdata 2>&1 | tee -a "$LOG" || fastboot -s "$SERIAL" format userdata 2>&1 | tee -a "$LOG"
  fi
elif [ "$MODE" = "adb" ]; then
  echo "[*] Attempting adb-based wipe (requires device with adb & permissions)" | tee -a "$LOG"
  adb devices 2>&1 | tee -a "$LOG"
  SERIAL="$(adb devices 2>/dev/null | awk 'NR==2{print $1}' || true)"
  if [ -z "$SERIAL" ]; then
    echo "[!] No adb device found." | tee -a "$LOG"
    exit 1
  fi
  echo "[*] Rebooting into recovery" | tee -a "$LOG"
  adb -s "$SERIAL" reboot recovery 2>&1 | tee -a "$LOG" || true
  # Note: automatic recovery wipe depends on recovery implementation; operator may need to finish manually
else
  echo "usage: $0 fastboot|adb [device]" | tee -a "$LOG"
  exit 1
fi

# after wipe, create attestation (best-effort)
SCRIPT_DIR="$(dirname "$(realpath "$0")")"
bash "$SCRIPT_DIR/attest.sh" "/dev/unknown-${SERIAL}" "android-${MODE}" "$LOG" || echo "[!] android attestation failed" | tee -a "$LOG"

exit 0
