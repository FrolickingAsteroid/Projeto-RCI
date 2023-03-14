#ifndef NEWMOD_H
#define NEWMOD_H

#include "../HostStruct/nodeStructure.h"

void ReadListeningSock(Host *HostNode, char *Buffer, int NewFd);
void ExternHandle(char *Buffer, Host *HostNode);

#endif
