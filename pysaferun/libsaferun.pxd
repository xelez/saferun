# /*
#  *  Copyright 2012 Alexander Ankudinov
#  *
#  *  Licensed under the Apache License, Version 2.0 (the "License");
#  *  you may not use this file except in compliance with the License.
#  *  You may obtain a copy of the License at
#  *
#  *      http://www.apache.org/licenses/LICENSE-2.0
#  *
#  *  Unless required by applicable law or agreed to in writing, software
#  *  distributed under the License is distributed on an "AS IS" BASIS,
#  *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#  *  See the License for the specific language governing permissions and
#  *  limitations under the License.
#  */

cimport posix.unistd
from posix.unistd cimport uid_t, gid_t

cdef extern from "fileobject.h":
    ctypedef class __builtin__.file [object PyFileObject]:
        pass

cdef extern from "Python.h":
    ctypedef struct FILE
    FILE* PyFile_AsFile(object)
    void  fprintf(FILE* f, char* s, char* s)

cdef extern from "stdio.h":
    int fileno(FILE *f)

cdef extern from "saferun.h":
    struct saferun_inst:
        pass

    struct saferun_jail:
        char *hostname
        char *chroot
        char *chdir
    
        uid_t uid
        gid_t gid

    struct saferun_limits:
        long rtime
        long time
        long long mem

    enum saferun_result:
        _OK = 0
        _RE = 1
        _TL = 2
        _ML = 3
        _SV = 4

    struct saferun_stat:
        long rtime
        long time
        long long mem
        long long start_time

        int status

        saferun_result result

    struct saferun_task:
        saferun_jail   *jail
        saferun_limits *limits
    
        char **argv
        int stdin_fd  
        int stdout_fd
        int stderr_fd

    int saferun_run(saferun_inst *inst, saferun_task *task, saferun_stat *stat)

    saferun_inst* saferun_init(char *cgroup_name)
    int saferun_fini(saferun_inst *inst)

    void saferun_set_logging(int fd, int priority)

cdef inline int get_fd(file f):
    if f is not None:
        return fileno(PyFile_AsFile(f))
    else:
        return -1
