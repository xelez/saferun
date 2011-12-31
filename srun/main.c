//Author: Ankudinov Alexander

#include <saferun.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>

struct saferun_jail jail;
struct saferun_limits limits;
struct saferun_stat stat;

int main(int argc, char *argv[])
{
    saferun_inst * inst = saferun_init("temp");
    strcpy(jail.chroot, "");
    strcpy(jail.chdir, "");
    strcpy(jail.hostname, "tratata");
    jail.uid = 1000; //me
    jail.gid = 1000; //xelez
    limits.mem = 64*1024*1024; // 64 mb
    limits.time = 1000; //1 sec
    limits.rtime = 2000; // 2 real seconds

    char * args[10];
    int i;
    for (i = 1; i < argc; ++i)
        args[i-1] = argv[i];
    args[argc - 1] = NULL;

    saferun_run(inst, args, &jail, &limits, &stat);
    
    printf("\n result = %d\n mem = %lld\n time = %ld\n rtime = %ld\n status = %d\n",
           stat.result, stat.mem, stat.time, stat.rtime, stat.status);
    if (stat.result != _OK) printf("FAIL:\n");
    saferun_fini(inst);

    int status = stat.status;

    if (WIFEXITED(status)) {
        printf("exited, status=%d\n", WEXITSTATUS(status));
    } else if (WIFSIGNALED(status)) {
        printf("killed by signal %d\n", WTERMSIG(status));
    } else if (WIFSTOPPED(status)) {
        printf("stopped by signal %d\n", WSTOPSIG(status));
    } else if (WIFCONTINUED(status)) {
        printf("continued\n");
    }

/*    printf("microseconds before exec: %lld\n", stat.start_time - tmptime);
    
    rusage ru;
    getrusage(RUSAGE_SELF, &ru);
    printf("USAGE BY SELF:\n user: %lld\n system: %lld\n\n", TV_TO_USEC(ru.ru_utime), TV_TO_USEC(ru.ru_stime));
    
    getrusage(RUSAGE_CHILDREN, &ru);
    printf("USAGE BY CHILDREN:\n user: %lld\n system: %lld\n\n", TV_TO_USEC(ru.ru_utime), TV_TO_USEC(ru.ru_stime));
*/
    return 0;
}


