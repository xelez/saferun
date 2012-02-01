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

#include "cgroup.h"
#include "log.h"

#include <string.h>
#include <mntent.h>
#include <signal.h>

#include <sys/param.h>

void cgroup_get_path(const char *subsystem, const char *cgroup_name, char *path)
{
    struct mntent *mntent;
    FILE *file = NULL;

    file = setmntent(MTAB, "r");
    if (!file) {
        SYSERROR("failed to open %s", MTAB);
        throw -1;
    }

    while ((mntent = getmntent(file))) {
        if (strcmp(mntent->mnt_type, "cgroup"))
            continue;
        if (!subsystem || hasmntopt(mntent, subsystem)) {
            snprintf(path, MAXPATHLEN, "%s/%s", mntent->mnt_dir, cgroup_name);
            fclose(file);
            DEBUG("using cgroup at '%s'", path);
            return;
        }
    };

    DEBUG("Failed to find cgroup for %s\n", subsystem ? subsystem : "(NULL)");
    fclose(file);
    throw -1;
}

FILE * cgroup_open(const char * path, const char *filename, const char *mode)
{
    char fullpath[MAXPATHLEN];
    snprintf(fullpath, MAXPATHLEN, "%s/%s", path, filename);

    FILE * file = fopen(fullpath, mode);
    if (!file) {
        SYSERROR("failed to open %s", fullpath);
        throw -1;
    }

    return file;
}

void cgroup_write_str(const char * path, const char * filename, const char * str)
{
    FILE * file = cgroup_open(path, filename, "w");
    
    fputs(str, file);
    int ret = ferror(file);
    fclose(file);

    if (ret) {
        ERROR("can`t write '%s' to '%s' at '%s'", str, path, filename);
        throw -1;
    }
}

void cgroup_write_ll(const char * path, const char * filename, const long long x)
{
    char str[22]; //in fact the max length of long long as string is 20
    snprintf(str, 22, "%lld", x);
    cgroup_write_str(path, filename, str);
}

void cgroup_read_ll(const char * path, const char * filename, long long * x)
{
    FILE * file = cgroup_open(path, filename, "r");
    int k = fscanf(file, "%lld", x);
    fclose(file);

    if (k != 1)
        throw -1;
}

void cgroup_kill(const char * path, int sig)
{
    FILE * file = cgroup_open(path, "tasks", "r");
    int pid;
    while (fscanf(file, "%d", &pid) == 1)
        kill(pid, sig);

    fclose(file);

    /* 
     * we don`t check for errors, because a lot of things
     * could happen while reading the file.
     *
     * For example proccess can terminate.
     */
}
