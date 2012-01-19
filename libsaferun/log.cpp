//Author: Ankudinov Alexander

#include "log.h"

#include <stdarg.h>
#include <stdio.h>
#include <unistd.h>

/*
 * __thread makes a thread-safe variable, as http://en.wikipedia.org/wiki/Thread-local_storage#C.2B.2B
 * says, all linux compilers allow this syntax
 */
/* -1 = no logging */
static __thread int log_fd = DEFAULT_LOG_FD;

int log_priority = DEFAULT_LOG_PRIORITY;

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
