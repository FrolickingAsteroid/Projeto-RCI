#ifndef TCP_H
#define TCP_H

#include "../HostStruct/nodeStructure.h"
#include <sys/types.h>

int TCPServer(UsrInvoc *usr);
ssize_t TCPClientExternConnect(Host *HostNode, char *msg, char *BootIp, char *BootTCP);
#endif
