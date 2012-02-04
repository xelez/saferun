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

#ifndef _LIB_SAFERUN_H
#define _LIB_SAFERUN_H

#ifdef __cplusplus
// For using in C libraries
extern "C" {
#endif

#include <sys/types.h>
#include <sys/resource.h>
#include <sys/param.h>
#include <sched.h>
#include <unistd.h>

/* Saferun hypervisor delay */
const int SAFERUN_HV_DELAY = 40*1000*1000; //in nanosec

/**
 * saferun_inst - information for use in library internals.
 *
 * Includes cgroup name and paths in filsystem to that
 * group in different subsystems.
 */
typedef struct saferun_inst {
    char cgname[MAXPATHLEN];
    
    char cpuacct_path[MAXPATHLEN];
    char devices_path[MAXPATHLEN];
    char memory_path[MAXPATHLEN];
} saferun_inst;

/**
 * saferun_jail - jail parameters for running the task.
 *
 * If some member is NULL, then it`s disabled.
 * So there will be no setuid/setgid if uid or gid
 * is zero.
 *
 * @todo maybe add some rlimits, see setrlimit(2)
 */
typedef struct saferun_jail {
    char *hostname; /**< change hostname to this of not NULL */
    char *chroot;   /**< change root to this dir if not NULL */
    char *chdir;    /**< do a chdir if not NULL (after chroot if chroot is not NULL) */
    
    uid_t uid; /**< if not zero, then run as this UID */
    gid_t gid; /**< if not zero, left only this group */
} saferun_jail;

/**
 * saferun_limits - limits, that will affect the jail.
 *
 * Memory is limited by cgroup.memory subsystem, there maybe some special effects because of that.
 */
typedef struct saferun_limits {
    long rtime;    /**< real time, in milliseconds */
    long time;     /**< user+system time, in milliseconds */
    long long mem; /**< in bytes */
} saferun_limits;

/**
 * saferun_result - task running result.
 */
typedef enum saferun_result {
    _OK = 0, /**< Clean exit, no errors */
    _RE = 1, /**< Runtime error */
    _TL = 2, /**< Time limit exceeded */
    _ML = 3, /**< Memory limit exceeded */
    _SV = 4  /**< Security Violation, never returned */
} saferun_result;

/**
 * saferun_stat - task running statistics.
 *
 * @see saferun_limits for details on most of the fields
 *
 * @todo add more details of rtime measuring
 */
typedef struct saferun_stat {
    long rtime;           /**< in milliseconds, not very precision */
    long time;            /**< in milliseconds */
    long long mem;        /**< in bytes*/
    long long start_time; /**< in microseconds, since epoch */

    int status; /**< status code, returned by waitpid function, @see waitpid(2) for details */

    saferun_result result; /**< @see saferun_result */
} saferun_stat;

/**
 * saferun_task - structure describing a task.
 * 
 * Includes all information for running.
 *
 * If some *_fd field is bigger or equal to zero, then the task
 * stdin/stdout/stderr will be reditected to this fd.
 */
typedef struct saferun_task {
    saferun_jail   *jail;   /**< @see saferun_jail */
    saferun_limits *limits; /**< @see saferun_limits */
    
    char **argv;   /**< argv for passing to execv */
    int stdin_fd;  
    int stdout_fd;
    int stderr_fd;
} saferun_task;

/**
 * Run task.
 *
 * @param inst : See saferun_inst
 * @param task : See saferun_task
 * @param stat : See saferun_stat
 *
 * @return -1 if there were some library errors(for example, no rights to create new cgroup).
 *         Returns 0 otherwise.
 * 
 * @note Error description is written to log_fd, you can set it to what you want by
 * saferun_set_logging().
 */
int saferun_run(const saferun_inst *inst, const saferun_task *task, saferun_stat *stat);

/**
 * Initialize the library.
 *
 * @param cgroup_name
 *     The name of cgroup to create and use for measuring time and limiting memory.
 * @return NULL if errors, or pointer to saferun_inst otherwise.
 *
 * @note It`s a good idea to call saferun_set_loggin() first.
 */
saferun_inst* saferun_init(const char *cgroup_name);

/**
 * Finilize the library
 *
 * @note Now it just frees memory.
 * @return always 0.
 */
int saferun_fini(saferun_inst * inst);

void saferun_set_logging(int fd, int priority);

#ifdef __cplusplus
} // extern "C"
#endif

#endif /*_LIB_SAFERUN_H */
