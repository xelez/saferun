#ifndef _log_h
#define _log_h

#include <stdarg.h>
#include <stdio.h>
#include <sys/time.h>

#define SAFERUN_LOG(level, format, ...) do {                           \
    fprintf(stderr, "Saferun " level ": " format "\n", ##__VA_ARGS__); \
} while (0)

#define TRACE(format, ...) SAFERUN_LOG("TRACE", format, ##__VA_ARGS__)
#define DEBUG(format, ...) SAFERUN_LOG("DEBUG", format, ##__VA_ARGS__)
#define INFO(format, ...) SAFERUN_LOG("INFO", format, ##__VA_ARGS__)
#define NOTICE(format, ...) SAFERUN_LOG("NOTICE", format, ##__VA_ARGS__)
#define WARN(format, ...) SAFERUN_LOG("WARN", format, ##__VA_ARGS__)
#define ERROR(format, ...) SAFERUN_LOG("ERROR", format, ##__VA_ARGS__)
#define CRIT(format, ...) SAFERUN_LOG("CRIT", format, ##__VA_ARGS__)
#define ALERT(format, ...) SAFERUN_LOG("ALERT", format, ##__VA_ARGS__)
#define FATAL(format, ...) SAFERUN_LOG("FATAL", format, ##__VA_ARGS__)
#define SYSERROR(format, ...) SAFERUN_LOG("SYSERROR", format, ##__VA_ARGS__)

#endif /*_log_h*/

