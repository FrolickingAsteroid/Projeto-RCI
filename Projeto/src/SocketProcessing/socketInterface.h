#ifndef SOCKETINTERFACE_H
#define SOCKETINTERFACE_H

#include "../HostStruct/nodeStructure.h"
#include "../Protocols/TCP.h"
#include "../Common/utils.h"

void SocketInterfaceParser(char *Buffer, Host *HostNode, Node *SenderNode);
void SendProtocolMsg(Host *HostNode, char *msg, int SenderFd);
#endif
