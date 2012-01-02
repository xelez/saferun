/*
 *
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


const int HYPERVISOR_DELAY = 40*1000*1000; //in nanosec
const int SYNC_MAGIC_1 = 17;
const int SYNC_MAGIC_2 = 27;
const int SYNC_MAGIC_FAIL = 136;

/*
 * saferun_inst - information for use in library
 *
 * Includes cgroup name and paths in fs to this
 * group in different subsystems.
 */
typedef struct saferun_inst
{
    char cgname[MAXPATHLEN];
    
    char cpuacct_path[MAXPATHLEN];
    char devices_path[MAXPATHLEN];
    char memory_path[MAXPATHLEN];
} saferun_inst;

/*
 * run_jail - jail parameters, to run the task
 *
 * If some member is NULL, then it`s disabled.
 * So there will be no setuid/setgid if uid or gid
 * is zero.
 *
 * chdir is done after chroot
 */
typedef struct saferun_jail
{
    char *hostname;
    char *chroot;
    char *chdir;
    
    uid_t uid;
    gid_t gid;
    
    //may be add some rlimits, see setrlimit(2)
} saferun_jail;

/*
 * saferun_limits - limits, that will affect the jail.
 * @time:    in miliseconds
 * @mem:     in bytes
 */
typedef struct saferun_limits
{
    long rtime;    // real time, in milliseconds
    long time;     // user+system time, in milliseconds
    long long mem; // TODO: comment needed, in bytes
} saferun_limits;

/* Task results */
typedef enum saferun_result
{
    _OK = 0, /* Clean exit, no errors */
    _RE = 1, /* Runtime error */
    _TL = 2, /* Time limit exceeded */
    _ML = 3, /* Memory limit exceeded */
    _SV = 4  /* Security Violation, never returned */
} saferun_result;

typedef struct saferun_stat
{
    long rtime;           // in milliseconds
    long time;            // in milliseconds
    long long mem;        // in bytes, TODO: add comment
    long long start_time; //in microseconds, since epoch

    int status; // returned by wait, see waitpid(2)

    saferun_result result; // See task results above
} saferun_stat;

int saferun_run(const saferun_inst *inst, char *argv[], const saferun_jail *jail,
                 const saferun_limits *limits, saferun_stat *stat);

saferun_inst* saferun_init(const char *cgroup_name);

int saferun_fini(saferun_inst * inst);

#ifdef __cplusplus
} // extern "C"
#endif

#endif /*_LIB_SAFERUN_H */
