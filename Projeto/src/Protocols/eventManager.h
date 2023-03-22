#ifndef EventManager_H
#define EventManager_H

#include <sys/types.h>

#include "../HostStruct/nodeStructure.h"

void EventManager(Host *HostNode);
int UpdateMaxDes(Host *HostNode);
ssize_t CustomRead(int Fd, char *Buffer, size_t BufferSize);

#endif
