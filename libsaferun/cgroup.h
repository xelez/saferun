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

#ifndef _CGROUP_H
#define _CGROUP_H

#define MTAB "/proc/mounts"

#include <stdio.h>

void cgroup_get_path(const char *subsystem, const char *cgroup_name, char *path);

FILE * cgroup_open(const char * path, const char *filename, const char *mode);
void cgroup_write_str(const char * path, const char * filename, const char * str);
void cgroup_write_ll(const char * path, const char * filename, const long long x);
void cgroup_read_ll(const char * path, const char * filename, long long * x);

void cgroup_kill(const char * path, int sig);

#endif /* _CGROUP_H */
