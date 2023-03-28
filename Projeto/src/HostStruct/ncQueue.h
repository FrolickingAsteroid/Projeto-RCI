//===------------------- NCQueue.h -----------------------------------------===//
//
// This file defines the interface for the new connection (NC) queue, which is
// responsible for managing new connections within the host node. Functions include
// initializing the queue, adding/removing connections, and cleaning up inactive
// connections.
//
//===----------------------------------------------------------------------===//

#ifndef NCQUEUE_H
#define NCQUEUE_H

#include "nodeStructure.h"

// Initialize a new connection queue
NewConQueue *InitNCQueue(int NewFd);
// Add a new connection to the host node
void PlugNC(int Fd, Host *HostNode, char *Buffer);
// Remove a new connection from the host node
void RemoveNC(Host *HostNode, int Newfd);
// Free the new connection list of the host node
void FreeNCList(Host *HostNode);
// Clean up inactive connections in the host node
void CleanInactiveConnections(Host *HostNode);

#endif // !NCQUEUE_H
