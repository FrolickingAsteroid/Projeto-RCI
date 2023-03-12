#ifndef EventManager_H
#define EventManager_H

#include "../Common/nodeStructure.h"
#include "../Protocols/TCP.h"
#include "../UserInterface/exitMod.h"

void WithdrawHandle(Host *HostNode, char *LeavingId);
void ExternHandle(char *Buffer, Host *HostNode);
#endif
