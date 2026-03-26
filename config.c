#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "config.h"
#include <linux/limits.h>

static void trim(char *str) {
    size_t len = strlen(str);
    while (len > 0 && (str[len - 1] == '\n' || str[len - 1] == '\r' || isspace((unsigned char)str[len - 1]))) {
        str[--len] = '\0';
    }

    char *start = str;
    while (*start && isspace((unsigned char)*start)) {
        start++;
    }

    if (start != str) {
        memmove(str, start, strlen(start) + 1);
    }
}

int load_config(const char *path, Config *config) {
    FILE *file = fopen(path, "r");
    if (file == NULL) {
        perror("fopen");
        return -1;
    }

    memset(config, 0, sizeof(Config));

    enum {
        SEC_NONE,
        SEC_PATHS,
        SEC_INCLUDE,
        SEC_EXCLUDE,
        SEC_TARGET
    } section = SEC_NONE;

    char line[MAX_LINE_LEN];

    while (fgets(line, sizeof(line), file) != NULL) {
        trim(line);

        if (line[0] == '\0' || line[0] == '#') {
            continue;
        }

        if (strcmp(line, "[paths]") == 0) {
            section = SEC_PATHS;
            continue;
        }
        if (strcmp(line, "[include]") == 0) {
            section = SEC_INCLUDE;
            continue;
        }
        if (strcmp(line, "[exclude]") == 0) {
            section = SEC_EXCLUDE;
            continue;
        }
        if (strcmp(line, "[target]") == 0) {
            section = SEC_TARGET;
            continue;
        }

        switch (section) {
            case SEC_PATHS:
                if (config->path_count < MAX_PATHS) {
                    strncpy(config->paths[config->path_count], line, PATH_MAX - 1);
                    config->paths[config->path_count][PATH_MAX - 1] = '\0';
                    config->path_count++;
                }
                break;

            case SEC_INCLUDE:
                if (config->include_count < MAX_PATTERNS) {
                    strncpy(config->includes[config->include_count], line, PATH_MAX - 1);
                    config->includes[config->include_count][PATH_MAX - 1] = '\0';
                    config->include_count++;
                }
                break;

            case SEC_EXCLUDE:
                if (config->exclude_count < MAX_PATTERNS) {
                    strncpy(config->excludes[config->exclude_count], line, PATH_MAX - 1);
                    config->excludes[config->exclude_count][PATH_MAX - 1] = '\0';
                    config->exclude_count++;
                }
                break;

            case SEC_TARGET: {
                char key[128];
                char value[PATH_MAX];

                if (sscanf(line, "%127[^=]=%1023[^\n]", key, value) == 2) {
                    trim(key);
                    trim(value);

                    if (strcmp(key, "host") == 0) {
                        strncpy(config->host, value, sizeof(config->host) - 1);
                    } else if (strcmp(key, "user") == 0) {
                        strncpy(config->user, value, sizeof(config->user) - 1);
                    } else if (strcmp(key, "path") == 0) {
                        strncpy(config->target_path, value, sizeof(config->target_path) - 1);
                    }
                }
                break;
            }

            default:
                break;
        }
    }

    fclose(file);
    return 0;
}

void print_config(const Config *config) {
    int i;

    printf("Paths:\n");
    for (i = 0; i < config->path_count; i++) {
        printf("  %s\n", config->paths[i]);
    }

    printf("Includes:\n");
    for (i = 0; i < config->include_count; i++) {
        printf("  %s\n", config->includes[i]);
    }

    printf("Excludes:\n");
    for (i = 0; i < config->exclude_count; i++) {
        printf("  %s\n", config->excludes[i]);
    }

    printf("Target:\n");
    printf("  host=%s\n", config->host);
    printf("  user=%s\n", config->user);
    printf("  path=%s\n", config->target_path);
}