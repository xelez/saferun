/*
 * Author: Ankudinov Alexandr
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
