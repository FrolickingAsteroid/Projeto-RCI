//===-- udp.h ------------------------------------------------------------===//
//
// This header file declares functions for managing UDP client communication.
// The functions allow sending messages to a server and receiving responses
// using the User Datagram Protocol (UDP).
//
//===----------------------------------------------------------------------===//

#ifndef UDP_H
#define UDP_H

#include "../HostStruct/nodeStructure.h"

// Sends a message to a server using UDP and returns the received response
char *UDPClient(Host *HostNode, char *msg);

#endif // UDP_H
