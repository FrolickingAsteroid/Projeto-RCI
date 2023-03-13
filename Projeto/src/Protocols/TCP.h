#ifndef TCP_H
#define TCP_H

#include "../Common/checkInvocationInfo.h"
#include "../Common/formatChecking.h"
#include "../HostStruct/nodeStructure.h"
#include "../Common/utils.h"

int TCPServer(UsrInvoc *usr);
char *TCPClientExternConnect(Host *HostNode, char *msg, char *BootIp, char *BootTCP);
void ReadListeningSock(Host *HostNode, char *Buffer, int NewFd);
char *SendTCPMessage(int Fd, char *msg);
#endif
