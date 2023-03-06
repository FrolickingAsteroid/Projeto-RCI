#ifndef TCP_H
#define TCP_H

#include "checkInvocationInfo.h"
#include "nodeStructure.h"
#include "utils.h"

int TCPServer(UsrInvoc *usr);
char *TCPClient(Host *HostNode, char *msg);
#endif
