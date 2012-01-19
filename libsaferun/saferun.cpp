//Author: Ankudinov Alexander

#include "log.h"
#include "cgroup.h"
#include "utils.h"
#include "sync.h"
#include "saferun.h"
#include "hv.h"
#include "profiling.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <sys/param.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/resource.h>

#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <errno.h>

struct clone_data
{
    const saferun_task *task;
    int fd;
};

void setup_cgroup(const saferun_inst *inst, const saferun_limits *limits)
{
    mkdir(inst->cpuacct_path, 0777);
    mkdir(inst->devices_path, 0777);
    mkdir(inst->memory_path, 0777);

    cgroup_write_str(inst->devices_path, "devices.deny", "a");
    cgroup_write_ll(inst->memory_path, "memory.limit_in_bytes", limits->mem);
    cgroup_write_ll(inst->memory_path, "memory.memsw.limit_in_bytes", limits->mem);

    //Not sure about this, see kernel-doc/cgroups/memory.txt
//    cgroup_write_ll(inst->memory_path, "memory.move_charge_at_immigrate", 3);
}

void fini_cgroup(const saferun_inst *inst)
{
    PROFILING_START();

    // memory.force_empty takes 0.5 seconds, too long and not really needed
    //cgroup_write_ll(inst->memory_path, "memory.force_empty", 0);
    PROFILING_CHECKPOINT();
    rmdir(inst->cpuacct_path);
    rmdir(inst->memory_path);
    rmdir(inst->devices_path);

    PROFILING_CHECKPOINT();
}

void task_to_cgroup(const saferun_inst *inst, pid_t pid)
{
    cgroup_write_ll(inst->memory_path, "tasks", pid);
    cgroup_write_ll(inst->devices_path, "tasks", pid);
    cgroup_write_ll(inst->cpuacct_path, "tasks", pid);
}

int do_start(void *_data)
{
    clone_data * data = (clone_data*) _data;
    const saferun_task *task = data->task;
    const saferun_jail *jail = task->jail;

    try {
        redirect_fd(task->stdin_fd, 0);
        redirect_fd(task->stdout_fd, 1);
        redirect_fd(task->stderr_fd, 2);
        
        //set close-on-exec flag to all fds, except 0, 1, 2
        setup_inherited_fds();
        
        setup_hostname(jail->hostname);
        setup_chroot(jail->chroot);
        setup_chdir(jail->chdir);
        setup_uidgid(jail->uid, jail->gid);

        setup_drop_caps();
    }
    catch(...) {
        sync_wake(data->fd, SYNC_MAGIC_FAIL);
        return -1;
    }
    
    try {
        sync_wake(data->fd, SYNC_MAGIC_1);
        sync_wait(data->fd);
    }
    catch(...) {
        return -1;
    }

    execvp(task->argv[0], task->argv);
    
    //This code runs, so an error occured
    ERROR("Can`t exec %s: %s", task->argv[0], strerror(errno));
    sync_wake(data->fd, SYNC_MAGIC_FAIL);

    return 0;
}

int saferun_run(const saferun_inst *inst, const saferun_task *task, saferun_stat *stat)
{
    PROFILING_START();
    const int clone_flags = CLONE_NEWUTS | CLONE_NEWPID | CLONE_NEWIPC | CLONE_NEWNET;
    int sv[2];
    int ret = 0;
    int sync_res = -1;
    pid_t pid;

    sv[0] = sv[1] = 0;

    if (!inst || !task || !task->jail || !task->limits || !stat)
        return -1;

    clone_data data;
    data.task = task;

    try {
        setup_cgroup(inst, task->limits);
        sync_init(sv);
        data.fd = sv[1];
        
        pid = saferun_clone(do_start, &data, clone_flags);
        //closing second socket as not needed in this thread
        close(sv[1]);
        sv[1] = 0;

        sync_res = sync_wait(sv[0]);
        if (sync_res != SYNC_MAGIC_1)
            throw -1;

        task_to_cgroup(inst, pid);
        stat->start_time = get_rtime();

        sync_wake(sv[0], SYNC_MAGIC_2);

        sync_res = sync_wait(sv[0]);
        if (sync_res == SYNC_MAGIC_FAIL) {
            // if other end is closed, sync_wait
            // will just read nothing and return -1
            DEBUG("Caught error on exec");
            throw -1;
        }
        
        hypervisor(inst, pid, task->limits, stat);
        
    }
    catch (...) {
        if (pid > 0) kill(pid, SIGKILL);
        try {
            cgroup_kill(inst->cpuacct_path, SIGKILL);
            cgroup_kill(inst->memory_path, SIGKILL);
        } catch(...) {}
        ret = -1;
    }
    
    PROFILING_CHECKPOINT();
    
    try {
        sync_free(sv);
        fini_cgroup(inst);
    } catch(...) {}

    PROFILING_CHECKPOINT();
    return ret;
}

/*
 * Initialize the library.
 *
 * Finds cgroups, finds all needed paths
 * Returns pointer to struct for using with all other functions
 */
saferun_inst* saferun_init(const char *cgroup_name)
{
    if (!cgroup_name)
        return NULL;

    saferun_inst * inst = new saferun_inst;

    try {
        strcpy(inst->cgname, cgroup_name);

        cgroup_get_path("cpuacct", inst->cgname, inst->cpuacct_path);
        cgroup_get_path("memory",  inst->cgname, inst->memory_path);
        cgroup_get_path("devices", inst->cgname, inst->devices_path);
    }
    catch (...) {
        delete inst;
        inst = NULL;
    }

    return inst;
}

/*
 * Unititialize the library.
 * 
 * Just frees memory.
 */
int saferun_fini(saferun_inst * inst)
{
    if (!inst)
        return -1;
    delete inst;
    return 0;
}

void saferun_set_logging(int fd, int priority)
{
    log_set_logging(fd, priority);
}

