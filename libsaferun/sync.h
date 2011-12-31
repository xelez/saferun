#ifndef _SYNC_H
#define _SYNC_H

int  sync_init(int sv[2]);
void sync_free(int sv[2]);

int  sync_wait(int fd);
void sync_wake(int fd, int sequence);


#endif /*_SYNC_H*/
