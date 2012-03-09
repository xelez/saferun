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

#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>

#include "log.h"

/**
 * Init sockets for syncing
 */
void sync_init(int sv[2])
{
    /* SOCK_CLOEXEC means that we don`t inherit this after exec */
    if (socketpair(AF_LOCAL, SOCK_STREAM | SOCK_CLOEXEC, 0, sv)) {
        SYSERROR("failed to create synchronization socketpair");
        throw -1;
    }
}

void sync_free(int sv[2])
{
    if (sv[0])
        close(sv[0]);
    if (sv[1])
        close(sv[1]);
    sv[0] = sv[1] = 0;
}

int sync_wait(int fd)
{
    int sync = -1;

    if (read(fd, &sync, sizeof(sync)) < 0) {
        ERROR("sync wait failure : %s", strerror(errno));
        throw -1;
    }

    return sync;
}

void sync_wake(int fd, int sequence)
{
    int sync = sequence;

    if (write(fd, &sync, sizeof(sync)) < 0) {
        ERROR("sync wake failure : %s", strerror(errno));
        throw -1;
    }
}
