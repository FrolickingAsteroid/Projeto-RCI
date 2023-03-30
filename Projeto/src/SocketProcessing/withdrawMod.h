//===--- WithdrawHandle.h --------------------------------------------------===//
//
// Header file for WithdrawHandle module which handles building and handling of
// withdraw messages and withdrawing nodes, as well as sending external messages.
//
//===----------------------------------------------------------------------===//
#ifndef WITHDRAWHANDLE_H
#define WITHDRAWHANDLE_H

#include "../HostStruct/nodeStructure.h"

// Builds a withdraw message for the given node and sends it to the specified socket.
void BuildWithdrawMessage(Host *HostNode, char *LeavingId, int SenderFd);
// Handles a withdraw message for the given node.
void WithdrawHandle(Host *HostNode, char *LeavingId, int SenderFd);
// Receives a withdraw message from the specified neighbor
void ReceiveWithdrawMsg(Host *HostNode, char *Buffer, int SenderFd);
// Sends an external message for the given node
void SendExternMsg(Host *HostNode, int ancor);

#endif
