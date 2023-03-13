#ifndef WITHDRAWHANDLE_H
#define WITHDRAWHANDLE_H

#include "../Common/nodeStructure.h"
#include "../Protocols/TCP.h"
#include "../UserInterface/exitMod.h"

void WithdrawHandle(Host *HostNode, char *LeavingId);
void ExternHandle(char *Buffer, Host *HostNode);
void SendWithdrawMsg(Host *HostNode, char *msg);
void UpdateForwardingTable(Host *HostNode, char *LeavingId);
#endif
