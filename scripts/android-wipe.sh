#!/usr/bin/env bash
set -euo pipefail
export PATH="$PATH:/usr/local/bin:/usr/bin:/bin:/usr/sbin:/sbin"

# This script handles detection, confirmation, and wiping.
# It assumes the device is in ADB mode and will reboot it to fastboot.

LOG="/tmp/sentinel-android.log"
echo "[*] android-wipe -> starting..." > "$LOG"

SERIAL=""

echo "[*] Looking for adb devices..." | tee -a "$LOG"

# --- Start adb server and wait ---
echo "[*] Explicitly starting adb server daemon..." | tee -a "$LOG"
adb start-server 2>&1 | tee -a "$LOG"
sleep 1 # Give the server a second to find devices

adb devices 2>&1 | tee -a "$LOG"
SERIAL="$(adb devices 2>/dev/null | grep -v 'unauthorized' | awk 'NR==2{print $1}' || true)"
if [ -z "$SERIAL" ]; then
  echo "[!] No authorized adb device found." | tee -a "$LOG"
  echo "[!] Please check for an 'Allow USB debugging' prompt on your device." | tee -a "$LOG"
  exit 1
fi
echo "[*] Found adb device: $SERIAL" | tee -a "$LOG"

# --- Confirmation Step ---
echo "========================================"
echo "  Found device: $SERIAL"
echo "  Wipe will reboot to fastboot and erase."
echo "========================================"
echo -n "Type 'yes' to confirm and wipe: "

# Read directly from the terminal
read -r CONFIRM < /dev/tty

if [ "$CONFIRM" != "yes" ]; then
  echo "[!] Canceled by user." | tee -a "$LOG"
  exit 2 # Exit with code 2 for "canceled"
fi

# --- Wipe Step ---
echo "[*] WIPE CONFIRMED. Proceeding..." | tee -a "$LOG"

echo "[*] Rebooting $SERIAL into bootloader (fastboot)..." | tee -a "$LOG"
adb -s "$SERIAL" reboot bootloader 2>&1 | tee -a "$LOG" || true

echo "[*] Waiting for device to appear in fastboot..." | tee -a "$LOG"
sleep 15 

echo "[*] Device detected. Wiping all data with 'fastboot -w'..." | tee -a "$LOG"
fastboot -w 2>&1 | tee -a "$LOG"

# --- Attestation Step ---
SCRIPT_DIR="$(dirname "$(realpath "$0")")"
bash "$SCRIPT_DIR/attest.sh" "/dev/unknown-${SERIAL}" "android-wipe" "$LOG" || echo "[!] android attestation failed" | tee -a "$LOG"

echo "[*] Wipe process finished." | tee -a "$LOG"
exit 0