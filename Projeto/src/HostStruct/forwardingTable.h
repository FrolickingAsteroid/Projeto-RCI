#ifndef FORWARDING_TABLE_H
#define FORWARDING_TABLE_H

#include "nodeStructure.h"
#include "../SocketProcessing/socketInterface.h"

void UpdateForwardingTable(Host *HostNode, char *LeavingId);
void InsertInForwardingTable(Host *HostNode, int neigh, int dest);
Node *CheckForwardingTable(Host *HostNode, char *dest);

#endif /* FORWARDING_TABLE_H */
