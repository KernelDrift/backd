#!/run/current-system/sw/bin/bash
set -euo pipefail

#SOURCE="/home/$USER"
TARGET="echo2:~/data/backup"
LOGFILE="/home/klaus/backup.log"
INCLUDES="/etc/backup-include.txt"
EXCLUDES="/etc/backup-exclude.txt"

mkdir -p "$(dirname "$LOGFILE")"
#mkdir -p "$TARGET"

:> "$LOGFILE"

rsync -aHAXR -r --delete --numeric-ids --info=progress2,stats --human-readable --exclude-from="$EXCLUDES" --files-from="$INCLUDES" / "$TARGET" >> "$LOGFILE" 2>&1