#define STATUS_FILE "sync_state.txt"

typedef struct
{
   long lastsync;
   int statuscode;
} SyncState;

int load_sync_state(const char *path, SyncState *state);
int save_sync_state(const char *path, const SyncState *state);