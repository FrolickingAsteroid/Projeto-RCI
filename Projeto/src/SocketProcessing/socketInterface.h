//===--- SocketInterface.h -------------------------------------------------===//
//
// Header file for SocketInterface module which handles parsing of socket messages
// and sending protocol messages.
//
//===----------------------------------------------------------------------===//
#ifndef SOCKETINTERFACE_H
#define SOCKETINTERFACE_H

#include <stdlib.h>

#include "../HostStruct/nodeStructure.h"

// Parses socket messages and handles them appropriately
void SocketInterfaceParser(char *Buffer, Host *HostNode, Node *SenderNode);
// Sends protocol messages over sockets
void SendProtocolMsg(Host *HostNode, char *msg, int SenderFd);
// Writes a message to a file descriptor
ssize_t CustomWrite(int Fd, char *Msg, size_t MsgSize);

#endif
