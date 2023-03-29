//===------------ exitMod.h ------------------------------------------------===//
//
// This file provides functions for gracefully exiting the program and
// leaving the network, ensuring proper resource deallocation and cleanup.
//
//===----------------------------------------------------------------------===//

#ifndef EXITMOD_H
#define EXITMOD_H

#include "../HostStruct/nodeStructure.h"

// Exits the program
void ExitProgram(Host *HostNode);
// Leaves network
void LeaveNetwork(Host *HostNode);

#endif
