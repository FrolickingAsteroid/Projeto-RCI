#ifndef TCP_H
#define TCP_H

#include "checkInvocationInfo.h"
#include "nodeStructure.h"
#include "utils.h"

int TCPServer(UsrInvoc *usr);
char *TCPClientExternConnect(Host *HostNode, char *msg, char *BootIp, char *BootId,
                             char *BootTCP);
#endif
