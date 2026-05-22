#!/run/current-system/sw/bin/bash
set -euo pipefail

#SOURCE="/home/$USER"
TARGET="echo2:~/data/backup/current"
TARGET2="nebula:~/data/backup/current"
LOGFILE="/home/klaus/backup.log"
INCLUDES="/etc/backup-include.txt"
EXCLUDES="/etc/backup-exclude.txt"

STAMP="$(date +%F_%H-%M-%S)"

mkdir -p "$(dirname "$LOGFILE")"
: > "$LOGFILE"

echo "===== Backup echo2 gestartet: $(date) =====" >> "$LOGFILE"

ssh echo2 'mkdir -p ~/data/backup/current ~/data/backup/deleted'

rsync -aHAXR -r \
  --delete-delay \
  --backup \
  --backup-dir="../deleted/'"$STAMP"'" \
  --numeric-ids \
  --info=progress2,stats \
  --human-readable \
  --exclude-from="$EXCLUDES" \
  --files-from="$INCLUDES" \
  / "$TARGET" >> "$LOGFILE" 2>&1

echo "===== Backup nebula gestartet: $(date) =====" >> "$LOGFILE"

rsync -aHAXR -r \
  --delete-delay \
  --numeric-ids \
  --info=progress2,stats \
  --human-readable \
  --exclude-from="$EXCLUDES" \
  --files-from="$INCLUDES" \
  / "$TARGET2" >> "$LOGFILE" 2>&1

ssh nebula 'sudo btrfs subvolume snapshot -r /home/klaus/data/backup/current /home/klaus/data/backup/snapshots/backup-$(date +%F_%H-%M-%S)' >> "$LOGFILE" 2>&1
ssh echo2 'find ~/data/backup/deleted -mindepth 1 -maxdepth 1 -type d -mtime +60 -exec rm -rf {} +' >> "$LOGFILE" 2>&1
echo "===== Backup fertig: $(date) =====" >> "$LOGFILE"