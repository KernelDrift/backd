#!/run/current-system/sw/bin/bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

SERVICE_SRC="$SCRIPT_DIR/backup-rsync.service"
TIMER_SRC="$SCRIPT_DIR/backup-rsync.timer"
SCRIPT_SRC="$SCRIPT_DIR/skript.sh"
SCRIPT_NIX="$SCRIPT_DIR/backup-rsync.nix"
NIX_DIR="/etc/nixos/backup-rsync.nix"
BACKUP_SRC="$SCRIPT_DIR/backup-excludes.txt"
INCLUDE_SRC="$SCRIPT_DIR/backup-includes.txt"

SERVICE_DST="/etc/systemd/system/backup-rsync.service"
TIMER_DST="/etc/systemd/system/backup-rsync.timer"
#SCRIPT_DST="/usr/local/bin/backup-rsync.sh"
SCRIPT_DST="/etc/nixos/scripts/backup-rsync.sh"


#Note if you are using a simple linux, remove the comments this should work, nixos -> not working

#sudo cp "$SERVICE_SRC" "$SERVICE_DST"
#sudo cp "$TIMER_SRC" "$TIMER_DST"

#if you use nixos
sudo mkdir -p /etc/nixos/scripts
sudo cp "$SCRIPT_NIX" "$NIX_DIR"
echo "Add this to /etc/nixos/configuration.nix:
imports = [ ./backup-rsync.nix ];"

sudo cp "$SCRIPT_SRC" "$SCRIPT_DST"

sudo chmod +x "$SCRIPT_DST"

sudo cp "$INCLUDE_SRC" "/etc/backup-include.txt"
sudo cp "$BACKUP_SRC" "/etc/backup-exclude.txt"

#sudo systemctl daemon-reload
#sudo systemctl enable --now backup-rsync.timer
echo "Rebuild the nix system now"
sudo systemctl list-timers --all