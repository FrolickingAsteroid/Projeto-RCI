#ifndef WITHDRAWHANDLE_H
#define WITHDRAWHANDLE_H

#include "../HostStruct/nodeStructure.h"

void BuildWithdrawMessage(Host *HostNode, char *LeavingId, int SenderFd);
void WithdrawHandle(Host *HostNode, char *LeavingId, int SenderFd);
void ReceiveWithdrawMsg(Host *HostNode, char *Buffer, int SenderFd);
void SendExternMsg(Host *HostNode);
#endif
