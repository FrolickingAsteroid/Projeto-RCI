#ifndef WITHDRAWHANDLE_H
#define WITHDRAWHANDLE_H

#include "../HostStruct/nodeStructure.h"
#include "../HostStruct/forwardingTable.h"
#include "../Protocols/TCP.h"
#include "../UserInterface/exitMod.h"

void WithdrawHandle(Host *HostNode, char *LeavingId);
void ExternHandle(char *Buffer, Host *HostNode);
void SendExternMsg(Host *HostNode);
#endif
