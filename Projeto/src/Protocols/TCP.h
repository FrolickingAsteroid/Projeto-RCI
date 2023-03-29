//===-- tcp.h -------------------------------------------------------------===//
//
// This header file declares functions for creating and managing TCP server and
// client connections. These functions handle the creation of TCP sockets,
// binding, listening, and connecting to other nodes using TCP.
//
//===----------------------------------------------------------------------===//

#ifndef TCP_H
#define TCP_H

#include "../HostStruct/nodeStructure.h"
#include <sys/types.h>

// Creates a TCP server socket with the provided user invocation parameters
int TCPServer(UsrInvoc *usr);
// Establishes a connection to an external node using TCP
ssize_t TCPClientExternConnect(Host *HostNode, char *msg, char *BootIp, char *BootTCP);

#endif // TCP_H
