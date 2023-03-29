//===--- newmod.h ----------------------------------------------------------===//
//
// Functions for handling new connections and external connections.
//
//===----------------------------------------------------------------------===//
#ifndef NEWMOD_H
#define NEWMOD_H

#include "../HostStruct/nodeStructure.h"

// Handles new connections for the host.
void HandleNewCon(Host *HostNode, NewConQueue *NewCon);
// Handles external connections for the host.
void ExternHandle(char *Buffer, Host *HostNode);

#endif
