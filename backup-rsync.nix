{config, pkgs, ...}:

{
 systemd.services.backup-rsync = {
    description = "System-wide rsync backup";
    after = [ "network-online.target" ];
    wants = [ "network-online.target" ];

    path = with pkgs; [
      bash
      rsync
      openssh
      coreutils
    ];

    
    serviceConfig = {
      Type = "oneshot";
      ExecStart = "/etc/nixos/scripts/backup-rsync.sh";
      User = "klaus";
    };
  };

  systemd.timers.backup-rsync = {
    description = "Run rsync backup regularly";
    wantedBy = [ "timers.target" ];
    timerConfig = {
      OnBootSec = "10min";
      OnUnitActiveSec = "1h";
      Persistent = true;
    };
  };
}