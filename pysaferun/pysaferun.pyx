#
# Copyright 2012 Alexander Ankudinov
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

"""Saferun library

This module provides a mechanism for execution of programs in secured
environment with resource limiting.

Note:
    This library usually needs root privelegies, because it uses cgroups
    and a lot of other stuff. See libsaferun documentation.

Usage:
    >>> import pysaferun
    >>> import sys
    >>> inst = pysaferun.Instance("sample_group", pysaferun.LOG_DEBUG, sys.stderr)
    >>> jail = pysaferun.Jail(hostname = "temp")
    >>> limits = pysaferun.Limits(real_time=2000, time = 1000, memory = 256*1024**2)
    >>> task = pysaferun.Task(inst, jail, limits, ('hostname', ))
    >>> info = task.run()
    >>> if info is not None and info['result'] == pysaferun.OK: print 'OK'
    ... 
    OK
    >>> 
"""

__author__ = ( 'Alexander Ankudinov <xelez0@gmail.com>', )
__copyright__ = ( 'Copyright (c) 2012 Alexander Ankudinov', )
__license__ = 'Apache 2.0'
__url__ = 'http://github.com/xelez/saferun/'
__version__ = '0.1'

cimport libsaferun
from libsaferun cimport *
from libc cimport stdlib
import sys

LOG_TRACE = 0
LOG_DEBUG = 1
LOG_INFO  = 2
LOG_WARN  = 3
LOG_ERROR = 4

OK = 0
RE = 1
TL = 2
ML = 3
SV = 4

cdef class Instance:
    """Instance(cgroup_name, log_level=LOG_INFO, log_file=sys.stderr)

    Arguments:

    cgroup_name -- name of cgroup to create and use for running task
    log_level   -- log message with log level more than this
    log_file    -- file where to log
    """ 
    cdef saferun_inst *inst
    cdef bytes cgname
    cdef file log_file
    cdef int log_level

    def __cinit__(self, cgroup_name, log_level=LOG_INFO, log_file=sys.stderr):
        self.cgname, self.log_file, self.log_level = cgroup_name, log_file, log_level

        saferun_set_logging(get_fd(self.log_file), self.log_level)
        self.inst = saferun_init(self.cgname)

    def __dealloc__(self):
        saferun_fini(self.inst)

cdef class Jail:
    """Jail(chroot=None, chdir=None, hostname=None, uid=0, gid=0)

    Arguments:

    chroot   -- dir to chroot
    chdir    -- dir to chdir
    hostname -- change hostname to this
    uid, gid -- change uid and gid to this
    """
    cdef saferun_jail _jail
    cdef bytes chroot, chdir, hostname

    def __cinit__(self, chroot=None, chdir=None, hostname=None, uid=0, gid=0):
        self.hostname, self.chroot, self.chdir = hostname, chroot, chdir
        
        self._jail = saferun_jail(NULL, NULL, NULL, uid = uid, gid = gid)
        if hostname is not None:
            self._jail.hostname = self.hostname
        if chdir is not None:
            self._jail.chdir  = self.chdir
        if chroot is not None:
            self._jail.chroot = self.chroot

cdef class Limits:
    """Limits(time = 1000, real_time = 2000, memory = 64*1024*1024)
    """
    cdef saferun_limits _limits
    
    def __cinit__(self, time = 1000, real_time = 2000, memory = 64*1024*1024):
        self._limits = saferun_limits(rtime=real_time, time=time, mem=memory)

cdef class Task:
    """Task(instance, jail, limits, argv)
    """
    cdef Instance inst
    cdef Jail jail
    cdef Limits limits
    cdef tuple argv
    cdef char **_argv

    def __cinit__(self, instance, jail, limits, argv):
        self.inst, self.jail, self.limits, self.argv = instance, jail, limits, argv

        #converting argv
        cdef Py_ssize_t count = len(self.argv)
        self._argv = <char **>stdlib.malloc(sizeof(char*) * (count + 1))
        for i, value in enumerate(self.argv):
            self._argv[i] = value
        self._argv[count] = NULL

    def __dealloc__(self):
        stdlib.free(self._argv)

    def run(self, stdin=None, stdout=None, stderr=None):
        """Run task in secured environment.

        If stdin, stdout or stderr is not None and is a file object,
        then stdin, stdout or stderr of program is redirected to it.
        """
        cdef saferun_stat stat
        cdef saferun_task task = saferun_task(
                &self.jail._jail,
                &self.limits._limits,
                self._argv,
                get_fd(stdin),
                get_fd(stdout),
                get_fd(stderr))
        
        cdef int error = saferun_run(self.inst.inst, &task, &stat)
        if error != 0:
            return None

        return stat
