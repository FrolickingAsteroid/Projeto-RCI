//===-- forwarding_table.h ------------------------------------------------===//
//
// This file contains the declarations for functions related to the forwarding
// table, such as updating the table, inserting new entries, checking the table,
// and clearing it.
//
//===----------------------------------------------------------------------===//

#ifndef FORWARDING_TABLE_H
#define FORWARDING_TABLE_H

#include "nodeStructure.h"

// Update the forwarding table when a node with the given ID leaves the network
void UpdateForwardingTable(Host *HostNode, char *LeavingId);
// Insert a new entry into the forwarding table with the specified neighbor and destination
void InsertInForwardingTable(Host *HostNode, int neigh, int dest);
// Check the forwarding table for a specific destination and return the corresponding Node
Node *CheckForwardingTable(Host *HostNode, char *dest);
// Clear the forwarding table of the host
void ClearForwardingTable(Host *HostNode);

#endif /* FORWARDING_TABLE_H */
