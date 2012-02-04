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

#ifndef _LOG_H
#define _LOG_H

#include <string.h>
#include <errno.h>

#include "log_priorities.h"

#if !NDEBUG || USE_PROFILING 
#define DEFAULT_LOG_PRIORITY SAFERUN_LOG_TRACE;
#else
#define DEFAULT_LOG_PRIORITY SAFERUN_LOG_INFO;
#endif

// set default logging fd to stderr
#define DEFAULT_LOG_FD 2

#define LOG_PRINT(priority, level, format, ...) do {\
    log_print(priority, "Saferun " level ": " format "\n", ##__VA_ARGS__); \
} while (0)

#define TRACE(format, ...) LOG_PRINT(SAFERUN_LOG_TRACE, "TRACE", "%s:%d in %s - " format, __FILE__, __LINE__, __func__, ##__VA_ARGS__)
#define DEBUG(format, ...) LOG_PRINT(SAFERUN_LOG_DEBUG, "DEBUG", "%s:%d in %s - " format, __FILE__, __LINE__, __func__, ##__VA_ARGS__)

#define INFO(format, ...)  LOG_PRINT(SAFERUN_LOG_INFO,  "INFO",    format, ##__VA_ARGS__)
#define WARN(format, ...)  LOG_PRINT(SAFERUN_LOG_WARN,  "WARNING", format, ##__VA_ARGS__)
#define ERROR(format, ...) LOG_PRINT(SAFERUN_LOG_ERROR, "ERROR",   format, ##__VA_ARGS__)

#define SYSERROR(format, ...) ERROR("%s - " format, strerror(errno), ##__VA_ARGS__)
#define SYSWARN(format, ...)   WARN("%s - " format, strerror(errno), ##__VA_ARGS__)

void log_set_logging(int fd, int priority);
void log_print(int priority, const char * format, ...);

#endif /*_LOG_H*/

