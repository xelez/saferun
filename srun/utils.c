#include <pwd.h>
#include <grp.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/resource.h>
#include <stdio.h>

#include "utils.h"

uid_t uid_by_name(char *name)
{
    struct passwd *info;
    info = getpwnam(name);
    if (!info) 
        return 0;
    
    return info->pw_uid;
}

gid_t gid_by_name(char *name)
{
    struct group *info;
    info = getgrnam(name);
    if (!info) 
        return 0;
    
    return info->gr_gid;
}

void print_exit_status(int status)
{
    if (WIFEXITED(status)) {
        printf("exited, status=%d\n", WEXITSTATUS(status));
    } else if (WIFSIGNALED(status)) {
        printf("killed by signal %d\n", WTERMSIG(status));
    } else if (WIFSTOPPED(status)) {
        printf("stopped by signal %d\n", WSTOPSIG(status));
    } else if (WIFCONTINUED(status)) {
        printf("continued\n");
    }
}

void print_rusage()
{
    struct rusage ru;
    getrusage(RUSAGE_SELF, &ru);
    printf("USAGE BY SELF:\n user: %lld\n system: %lld\n\n", TV_TO_USEC(ru.ru_utime), TV_TO_USEC(ru.ru_stime));
    
    getrusage(RUSAGE_CHILDREN, &ru);
    printf("USAGE BY CHILDREN:\n user: %lld\n system: %lld\n\n", TV_TO_USEC(ru.ru_utime), TV_TO_USEC(ru.ru_stime));
}


