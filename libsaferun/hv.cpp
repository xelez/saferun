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

#include <sys/wait.h>
#include <signal.h>
#include <time.h>

#include "saferun.h"
#include "cgroup.h"
#include "utils.h"
#include "log.h"

/**
 * Checks process exit status.
 *
 * Sets stat->result to _RE if Runtime Error occured.
 *
 * @param status  process exit status
 * @param stat    statistics to update
 */
void check_exit_status(const int status, saferun_stat * stat)
{
    stat->status = status;
    if (stat->result != _OK)
        return;

    if (WIFSIGNALED(status) ||
            (WIFEXITED(status) && WEXITSTATUS(status)))
        stat->result = _RE;
    else
        stat->result = _OK;
}

/**
 * Checks user+system execution time of the process via cgroup subsystem files.
 *
 * Sets stat->result to _TL if time limit exceeded.
 *
 * @param inst    library instance
 * @param limits  limits to check
 * @param stat    statistics to update
 */
void check_time(const saferun_inst * inst, const saferun_limits * limits, saferun_stat * stat)
{
    long long t;
    cgroup_read_ll(inst->cpuacct_path, "cpuacct.usage", &t);
    stat->time = t / (1000*1000); //Converting from nano- to milli- seconds
    if (stat->result == _OK && stat->time > limits->time)
        stat->result = _TL;
}

/**
 * Checks real execution time of the process.
 *
 * Sets stat->result to _TL if real time limit exceeded.
 *
 * @param limits  limits to check
 * @param stat    statistics to update
 */
void check_rtime(const saferun_limits * limits, saferun_stat * stat)
{
    stat->rtime = (get_rtime() - stat->start_time) / 1000;
    if (stat->result == _OK && stat->rtime > limits->rtime)
        stat->result = _TL;
}

/**
 * Checks memory usage for process via cgroup subsystem files.
 *
 * Sets stat->result to _ML if memory limit exceeded.
 *
 * @param inst    library instance
 * @param limits  limits to check
 * @param status  process exit status
 * @param stat    statistics to update
 */
void check_memory(const saferun_inst * inst, const saferun_limits * limits, int status, saferun_stat * stat)
{
    long long failcnt, t;
    cgroup_read_ll(inst->memory_path, "memory.memsw.max_usage_in_bytes", &(stat->mem));
    cgroup_read_ll(inst->memory_path, "memory.failcnt", &failcnt);
    cgroup_read_ll(inst->memory_path, "memory.memsw.failcnt", &t);
    if (t > failcnt)
        failcnt = t;

    if (stat->result == _OK && failcnt > 0)
        if (WIFSIGNALED(status) && WTERMSIG(status) == SIGKILL)
            stat->result = _ML;
}

/**
 * Run hypervisor for process.
 *
 * Hypervisor will check execution time and memory usage.
 * It runs some checks every SAFERUN_HV_DELAY.
 * Some checks are run after process finishes.
 *
 */
void hypervisor(const saferun_inst *inst, pid_t pid, const saferun_limits * limits,
                saferun_stat * stat)
{
    // Setting up delay for hypervisor
    timespec delay;
    delay.tv_sec = 0;
    delay.tv_nsec = SAFERUN_HV_DELAY;
    
    stat->result = _OK;

    int status;
    while (1) {
        int w = waitpid(pid, &status, WNOHANG);
        if (w == -1) {
            DEBUG("Can`t wait for pid");
            throw -1;
        }
        check_time(inst, limits, stat);
        check_rtime(limits, stat);
        if (w == pid) {
            check_memory(inst, limits, status, stat);
            check_exit_status(status, stat);
            break;
        }
        
        if (stat->result != _OK) {
            kill(pid, SIGKILL);
            cgroup_kill(inst->cpuacct_path, SIGKILL);
            // One more iteration, so our process
            // wouldn`t become a zombie
            continue;
        }
        
        nanosleep(&delay, NULL);
    }

    cgroup_kill(inst->cpuacct_path, SIGKILL);
}

