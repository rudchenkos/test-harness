#include <sys/types.h>
#include <sys/event.h>
#include <sys/time.h>
#include <sys/errno.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define MAX_SOURCES 128

#define TRACE_PRINTF(...) printf(__VA_ARGS__);

static size_t nsources = 0;
static const char* source_paths[MAX_SOURCES] = { 0 };
static struct kevent kev_changelist[MAX_SOURCES] = { 0 };

static bool add_source(const char* filepath) {
    if (nsources == MAX_SOURCES) {
        printf("Cannot add %s to watch list: too many sources\n", filepath);
        return false;
    }

    TRACE_PRINTF("Watching %s ...\n", filepath);

    int fd = open(filepath, O_RDONLY | O_NONBLOCK);
    if (fd == -1) {
        printf("Cannot add %s to watch list: %s\n", filepath, strerror(errno));
        errno = 0;
        return false;
    }

    int idx = nsources++;
    source_paths[idx] = strdup(filepath);

    EV_SET(&kev_changelist[idx], fd, EVFILT_VNODE,
            EV_ADD | EV_ONESHOT,
            NOTE_DELETE | NOTE_WRITE | NOTE_EXTEND,
            0, NULL);
    return true;
}

static void add_stdin_watch() {
    int idx = nsources++;
    source_paths[idx] = NULL;
    EV_SET(&kev_changelist[idx], STDIN_FILENO, EVFILT_READ, EV_ADD | EV_ONESHOT, 0, 0, NULL);
}

static bool reopen_source_file(int fd) {
    for (size_t i = 0; i < nsources; ++i) {
        if (kev_changelist[i].ident == fd) {
            const char* filepath = source_paths[i];
            int new_fd = open(filepath, O_RDONLY | O_NONBLOCK);
            if (new_fd == -1) {
                printf("Cannot reopen %s: %s\n", filepath, strerror(errno));
                errno = 0;
                return false;
            }
            kev_changelist[i].ident = new_fd;
            close(fd);
            return true;
        }
    }
    printf("Warning: did not find changelist record on reopen\n");
    return false;
}

static bool rebuild_self(const char* selfpath) {
    pid_t make_pid = fork();
    if (make_pid == -1) {
        perror("fork");
        return false;
    }
    if (make_pid == 0) {
        printf("make %s\n", selfpath);
        execlp("make", "make", selfpath, NULL);
        perror("execlp(make)");
        abort();
    }
    int status = -1;
    if (waitpid(make_pid, &status, 0) == -1) {
        perror("waitpid");
        return false;
    }
    if (!WIFEXITED(status)) {
        printf("make did not exit normally\n");
        return false;
    }
    if (WEXITSTATUS(status) != 0) {
        printf("make finished with non-zero exit code\n");
        return false;
    }
    return true;
}

static void read_stdin() {
    // Read out all accumulated charactes to prevent immediate triggering of the
    // stdin event filter
    char buf[16] = {0};
    fgets(buf, 16, stdin);
}

void test_harness_start_watch(char** argv, const char* src) {
    int queue = kqueue();
    if (queue == -1) {
        perror("kqueue");
        exit(-1);
    }

    add_stdin_watch();
    add_source(src);

    struct kevent events[1] = { 0 };
    int rc = -1;

    while ((rc = kevent(queue, kev_changelist, nsources, events, 1, NULL)) != -1) {
        TRACE_PRINTF("Changes detected\n");

        if (events[0].ident == STDIN_FILENO) {
            read_stdin();
        }

        if (rebuild_self(argv[0])) {
            execv(argv[0], argv);
            perror("execl");
        }

        // If re-building fails we need to resume watching
        if (events[0].fflags & NOTE_DELETE) {
            TRACE_PRINTF("Deletion detected. Reopening the file\n");
            reopen_source_file(events[0].ident);
            // Rescan the sources from the root file?
        }
    }

    if (rc == -1) {
        perror("kevent");
        exit(-1);
    }
}
