/*
 *
 */

#ifndef _PROFILING_H
#define _PROFILING_H

void do_some_stuff()
{
    //just stuff that works for about 1 sec
    long long s = 1; int t = 100;
    for (int i = 0; i < 100500000; ++i) {
        s = t*s + i;
    }
    printf("some_eval: %lld\n", s);
}

#endif /* _PROFILING_H */
