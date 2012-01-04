#ifndef _UTILS_H
#define _UTILS_H

#include <sys/types.h>
#define TV_TO_USEC(t) ((t).tv_usec + (long long)((t).tv_sec)*1000*1000)

uid_t uid_by_name(char *name);
gid_t gid_by_name(char *name);

void print_exit_status(int status);
void print_rusage();


#endif /*_UTILS_H*/
