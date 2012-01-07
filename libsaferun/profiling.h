/*
 *
 */

#ifndef _PROFILING_H
#define _PROFILING_H

#ifdef USE_PROFILING

#include "utils.h"
#include "log.h"

#define PROFILING_START() long long profiling_start_time = get_rtime()
#define PROFILING_CHECKPOINT() TRACE("%s():%d checkpoint: %lld microseconds", __FUNCTION__, __LINE__, get_rtime() - profiling_start_time)

void do_some_stuff()
{
    //just stuff that works for about 1 sec
    long long s = 1; int t = 100;
    for (int i = 0; i < 100500000; ++i) {
        s = t*s + i;
    }
    printf("some_eval: %lld\n", s);
}

#else /* USE_PROFILING */

#define PROFILING_START()
#define PROFILING_CHECKPOINT()
void do_some_stuff() {
}

#endif /* USE_PROFILING */

#endif /* _PROFILING_H */
