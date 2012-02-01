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

#ifndef _PROFILING_H
#define _PROFILING_H

#ifdef USE_PROFILING

#include "utils.h"
#include "log.h"

#define PROFILING_START() long long profiling_start_time = get_rtime()
#define PROFILING_CHECKPOINT() TRACE("checkpoint: %lld microseconds", get_rtime() - profiling_start_time)

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
void do_some_stuff() {}

#endif /* USE_PROFILING */

#endif /* _PROFILING_H */
