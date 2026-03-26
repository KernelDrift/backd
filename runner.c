#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <limits.h>

#include "config.h"
#include "runner.h"
#include <linux/limits.h>

#define MAX_ARGV 256

static int run_single_path_sync(const Config *config, const char *source_path) {
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork");
        return -1;
    }

    if (pid == 0) {
        char target[PATH_MAX];
        char target_with_source[PATH_MAX];
        char *argv[MAX_ARGV];
        char exclude_args[MAX_PATTERNS][PATH_MAX + 32];

        int argc = 0;
        int i;

        snprintf(target, sizeof(target), "%s@%s:%s",
                 config->user,
                 config->host,
                 config->target_path);

        const char *last_slash = strrchr(source_path, '/');
        const char *basename = last_slash ? last_slash + 1 : source_path;

        if (basename[0] == '\0') {
            basename = "backup";
        }

        snprintf(target_with_source, sizeof(target_with_source), "%s/%s/", target, basename);

        argv[argc++] = "rsync";
        argv[argc++] = "-a";
        argv[argc++] = "--delete";
        argv[argc++] = "--info=progress2";

        for (i = 0; i < config->exclude_count; i++) {
            snprintf(exclude_args[i], sizeof(exclude_args[i]),
                     "--exclude=%s", config->excludes[i]);
            argv[argc++] = exclude_args[i];
        }

        argv[argc++] = (char *)source_path;
        argv[argc++] = target_with_source;
        argv[argc] = NULL;

        execvp("rsync", argv);
        perror("execvp");
        _exit(127);
    }

    int status;
    if (waitpid(pid, &status, 0) < 0) {
        perror("waitpid");
        return -1;
    }

    if (WIFEXITED(status)) {
        return WEXITSTATUS(status);
    }

    return -1;
}

int run_sync(const Config *config) {
    int final_code = 0;

    for (int i = 0; i < config->path_count; i++) {
        int code = run_single_path_sync(config, config->paths[i]);

        if (code != 0) {
            final_code = code;
        }
    }

    return final_code;
}