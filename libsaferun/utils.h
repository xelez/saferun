/*
 *
 */

#ifndef _UTILS_H
#define _UTILS_H

#include <sys/types.h>
#include <sched.h>

#define TV_TO_USEC(t) ((t).tv_usec + (long long)((t).tv_sec)*1000*1000)

long long get_rtime();

pid_t saferun_clone(int (*fn)(void *), void *arg, int flags);

void redirect_fd(int fd, int to_fd);
void setup_inherited_fds();
void setup_drop_caps();
void setup_hostname(const char *name);
void setup_chroot(const char *dir);
void setup_chdir(const char *dir);
void setup_uidgid(uid_t uid, gid_t gid);

#endif /*_UTILS_H */
