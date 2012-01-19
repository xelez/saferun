#ifndef _LOG_H
#define _LOG_H

#include <string.h>
#include <errno.h>

/* predefined priorities. */
enum {
    LOG_PRIORITY_TRACE,
    LOG_PRIORITY_DEBUG,
    LOG_PRIORITY_INFO,
    LOG_PRIORITY_WARN,
    LOG_PRIORITY_ERROR,
};

#if !NDEBUG || USE_PROFILING 
#define DEFAULT_LOG_PRIORITY LOG_PRIORITY_TRACE;
#else
#define DEFAULT_LOG_PRIORITY LOG_PRIORITY_INFO;
#endif

#define LOG_PRINT(priority, level, format, ...) do {\
    log_print(priority, "Saferun " level ": " format "\n", ##__VA_ARGS__); \
} while (0)

#define TRACE(format, ...) LOG_PRINT(LOG_PRIORITY_TRACE, "TRACE", "%s:%d in %s - " format, __FILE__, __LINE__, __func__, ##__VA_ARGS__)
#define DEBUG(format, ...) LOG_PRINT(LOG_PRIORITY_DEBUG, "DEBUG", "%s:%d in %s - " format, __FILE__, __LINE__, __func__, ##__VA_ARGS__)

#define INFO(format, ...)  LOG_PRINT(LOG_PRIORITY_INFO,  "INFO",    format, ##__VA_ARGS__)
#define WARN(format, ...)  LOG_PRINT(LOG_PRIORITY_WARN,  "WARNING", format, ##__VA_ARGS__)
#define ERROR(format, ...) LOG_PRINT(LOG_PRIORITY_ERROR, "ERROR",   format, ##__VA_ARGS__)

#define SYSERROR(format, ...) ERROR("%s - " format, strerror(errno), ##__VA_ARGS__)
#define SYSWARN(format, ...)   WARN("%s - " format, strerror(errno), ##__VA_ARGS__)

void log_set_logging(int fd, int priority);
void log_print(int priority, const char * format, ...);

#endif /*_LOG_H*/

