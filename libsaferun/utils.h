/*
 *  Copyright 2012 Alexander Ankudinov
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
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
