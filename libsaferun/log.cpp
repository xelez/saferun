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

#include "log.h"

#include <stdarg.h>
#include <stdio.h>
#include <unistd.h>

/*
 * __thread makes a thread-safe variable, as http://en.wikipedia.org/wiki/Thread-local_storage#C.2B.2B
 * says, all linux compilers allow this syntax
 */
static __thread int log_fd = DEFAULT_LOG_FD; /* -1 == no logging */
static __thread int log_priority = DEFAULT_LOG_PRIORITY;

void log_set_logging(int fd, int priority)
{
    //we will duplicate log fd because it can be redirected later
    log_fd = dup(fd);
    log_priority = priority;
}

void log_print(int priority, const char *format, ...)
{
    if (priority < log_priority || log_fd < 0)
        return;

    va_list va_arg;
    va_start(va_arg, format);
    vdprintf(log_fd, format, va_arg);
    va_end(va_arg);
}
