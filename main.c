#include "state.h"
#include <stdio.h>
#include "config.h"
#include "runner.h"
#include <time.h>

#define CONFIG_FILE "backupd.conf"
#define STATUS_FILE "sync_state.txt"


   int should_run_scan(const SyncState *state, int interval_days) {
    time_t now = time(NULL);
    long interval = interval_days * 24L * 60L * 60L;

    if (state->lastsync == 0)
        return 1;

    return ((long)now - state->lastsync) >= interval;
}

int main(int argc, char**argv){
   SyncState state;
   Config config;


     if (load_config(CONFIG_FILE, &config) != 0) {
        printf("Config konnte nicht geladen werden.\n");
        return 1;
    }

    //print_config(&config);


    if (load_sync_state(STATUS_FILE, &state) != 0) {
        printf("State file konnte nicht geladen werden.\n");
        return 1;
    }

    if(should_run_scan(&state,3) == 0){
        printf("Kein Sync notwendig");
        return 0;
    }

    time_t now = time(NULL);

    state.lastsync = (long)now;
    if (save_sync_state(STATUS_FILE, &state) != 0) {
    printf("State file konnte nicht gespeichert werden.\n");
    return 1;
    }

    printf("Geladener Status:\n");
    printf("last_sync=%ld\n", state.lastsync);
    printf("status_code=%d\n", state.statuscode);

    int result = run_sync(&config);

    printf("rsync exit code: %d\n", result);

    state.statuscode = result;

    if (save_sync_state(STATUS_FILE, &state) != 0) {
    printf("State file konnte nicht gespeichert werden.\n");
    return 1;
    }

    return 0;
}