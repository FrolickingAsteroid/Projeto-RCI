//===------------ UDP.h -----------------------------------------------------===//
//
// This file provides an interface for UDP communication, offering a simple
// function to send messages to a server and receive responses.
//
//===----------------------------------------------------------------------===//
#ifndef UDP_H
#define UDP_H

#include "../HostStruct/nodeStructure.h"

char *UDPClient(Host *HostNode, char *msg);

#endif
