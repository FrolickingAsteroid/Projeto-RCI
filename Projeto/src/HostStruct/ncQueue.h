#ifndef NCQUEUE_H
#define NCQUEUE_H

#include "nodeStructure.h"

NewConQueue *InitNCQueue(int NewFd);
void PlugNC(int Fd, Host *HostNode, char *Buffer);
void RemoveNC(Host *HostNode, int Newfd);
void FreeNCList(Host *HostNode);

#endif // !NCQUEUE_H
