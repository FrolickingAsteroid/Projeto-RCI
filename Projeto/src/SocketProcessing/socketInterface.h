#ifndef SOCKETINTERFACE_H
#define SOCKETINTERFACE_H

#include <stdlib.h>

#include "../HostStruct/nodeStructure.h"

void SocketInterfaceParser(char *Buffer, Host *HostNode, Node *SenderNode);
void SendProtocolMsg(Host *HostNode, char *msg, int SenderFd);
ssize_t CustomWrite(int Fd, char *Msg, size_t MsgSize);

#endif
