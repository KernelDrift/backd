#include <stdio.h>
#include "state.h"



int create_default_state_file(const char* path){
    FILE *file = fopen(path,"w");
    if (file == NULL){
        perror("fopen");
        return -1;
    }
    fprintf(file, "last_sync=0\n");
    fprintf(file, "status_code=0\n");
    fclose(file);
    return 0;
}

int load_sync_state(const char *path, SyncState *state) {
    FILE *file = fopen(path,"r");
    if(file==NULL){
        if(create_default_state_file(path) != 0){
            return -1;
        }else{
            file = fopen(path,"r");
              if (file == NULL) {
            perror("fopen");
            return -1;
        }
        }
    }

    state->lastsync = 0;
    state->statuscode = 0;

    char line[256];

    while(fgets(line, sizeof(line), file) != NULL){
        if(sscanf(line, "last_sync=%ld", &state->lastsync)==1){
            continue;
        }

        if (sscanf(line, "status_code=%d", &state->statuscode) == 1) {
            continue;
        }
    }


    fclose(file);
}


int save_sync_state(const char *path, const SyncState *state) {
    FILE *file = fopen(path, "w");
    if (file == NULL) {
        perror("fopen");
        return -1;
    }

    fprintf(file, "last_sync=%ld\n", state->lastsync);
    fprintf(file, "status_code=%d\n", state->statuscode);

    fclose(file);
    return 0;
}
