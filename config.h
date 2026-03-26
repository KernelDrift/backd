#ifndef CONFIG_H
#define CONFIG_H

#include <limits.h>

#define MAX_PATHS 32
#define MAX_PATTERNS 64
#define MAX_LINE_LEN 512

typedef struct {
    char paths[MAX_PATHS][PATH_MAX];
    int path_count;

    char includes[MAX_PATTERNS][PATH_MAX];
    int include_count;

    char excludes[MAX_PATTERNS][PATH_MAX];
    int exclude_count;

    char host[128];
    char user[128];
    char target_path[PATH_MAX];
} Config;

int load_config(const char *path, Config *config);
void print_config(const Config *config);

#endif