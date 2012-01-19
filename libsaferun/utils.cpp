#include "log.h"
#include "utils.h"

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <grp.h>

#include <sys/types.h>
#include <sys/capability.h>
#include <sys/time.h>
#include <sched.h>
#include <signal.h>
#include <dirent.h>

long long get_rtime()
{
    timeval t;
    gettimeofday(&t, NULL);
    return TV_TO_USEC(t);
}

pid_t saferun_clone(int (*fn)(void *), void *arg, int flags)
{
    long stack_size = sysconf(_SC_PAGESIZE);
    void *stack = alloca(stack_size) + stack_size;
    pid_t ret;
 
#ifdef __ia64__
    ret = __clone2(fn, stack,
            stack_size, flags | SIGCHLD, arg);
#else
    ret = clone(fn, stack, flags | SIGCHLD, arg);
#endif
    if (ret < 0) {
        ERROR("Failed to clone(0x%x): %s", flags, strerror(errno));
        throw -1;
    }

    return ret;
}

/*
 * Closes all fs`s except 0, 1, 2 and fd_to_ignore
 * They are for stdin, stdout, stderr and socket fd for syncing.
 */
void setup_close_all_fd(int fd_to_ignore)
{
    struct dirent dirent, *direntp;
    int fd, fddir;
    DIR *dir;

    dir = opendir("/proc/self/fd");
    if (!dir) {
        SYSWARN("failed to open dir /proc/self/fd/");
        WARN("can`t check for inherited fds");
        return; //Not a critical error
    }

    fddir = dirfd(dir);

    while (!readdir_r(dir, &dirent, &direntp)) {
        char procpath[64];
        char path[PATH_MAX];

        if (!direntp)
            break;

        if (!strcmp(direntp->d_name, "."))
            continue;

        if (!strcmp(direntp->d_name, ".."))
            continue;

        fd = atoi(direntp->d_name);

        if (fd == 0 || fd == 1 || fd == 2
                || fd == fddir || fd == fd_to_ignore)
            continue;

        /* found inherited fd, closing it */
        if (close(fd) == -1) {
            SYSWARN("can`t close fd %d", fd);
            WARN("Trying to close 2 more times");
            close(fd); close(fd);
        }
    }

    if (closedir(dir)) {
        SYSERROR("failed to close directory");
        throw -1;
    }
}

void redirect_fd(int fd, int to_fd)
{
    if (fd == to_fd || fd < 0)
        return;

    if (close(to_fd) < 0) {
        SYSERROR("Can`t close old fd: %d", fd);
        throw -1;
    }

    if (dup2(fd, to_fd) == -1) {
        SYSERROR("can`t redirect fd %d to %d", fd, to_fd);
        throw -1;
    }
}

void setup_drop_caps()
{
    cap_t empty;
    empty = cap_init();
    if (!empty) {
            SYSERROR("cap_init() failed");
            throw -1;
    }

    if ( capsetp(0, empty) ) {
            SYSERROR("capsetp() failed");
            throw -1;
    }
    
    if ( cap_free(empty) ) {
            SYSERROR("cap_free() failed");
            throw -1;
    }

    DEBUG("capabilities has been dropped");
}

void setup_hostname(const char *name)
{
    if (!name) return;
    if (sethostname(name, strlen(name)) == -1) {
        SYSERROR("can`t set hostname");
        throw -1;
    }
}

void setup_chroot(const char *dir)
{
    if (!dir) return;
    if (chroot(dir) == -1) {
        SYSERROR("can`t chroot");
        throw -1;
    }
}

void setup_chdir(const char *dir)
{
    if (!dir) return;
    if (chdir(dir) == -1) {
        SYSERROR("can`t chdir");
        throw -1;
    }
}

void setup_uidgid(uid_t uid, gid_t gid)
{
    // First setting gid, because if we set uid first,
    // we wouldn`t have rights for seting gid
    int ret1, ret2, ret3;
    const gid_t gid_list[] = {gid};
    if (gid > 0) {
        ret1 = setgroups(1, gid_list);
        ret2 = setgid(gid);
    }
    if (uid > 0) ret3 = setuid(uid);
    if (ret1 == -1 || ret2 == -1 || ret3 == -1) {
        ERROR("can`t set uid and gid");
        throw -1;
    }
}


