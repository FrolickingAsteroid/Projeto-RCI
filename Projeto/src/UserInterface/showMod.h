//===--- showmod.h ---------------------------------------------------------===//
//
// Functions for displaying forwarding table, topology, and message names.
//
//===----------------------------------------------------------------------===//
#ifndef SHOWMOD_H
#define SHOWMOD_H

#include "../HostStruct/nodeStructure.h"
// Displays the forwarding table for the host.
void ShowForwardingTable(Host *hostNode);
// Displays the network topology for the host.
void ShowTopology(Host *HostNode);
// Displays the message names for the host.
void ShowNames(Host *HostNode);
#endif //! SHOWMOD_H
