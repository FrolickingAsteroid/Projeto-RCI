#ifndef NEWMOD_H
#define NEWMOD_H

#include "../HostStruct/nodeStructure.h"

void HandleNewCon(Host *HostNode, NewConQueue *NewCon);
void ExternHandle(char *Buffer, Host *HostNode);

#endif
