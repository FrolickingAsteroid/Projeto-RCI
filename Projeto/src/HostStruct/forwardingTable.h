#ifndef FORWARDING_TABLE_H
#define FORWARDING_TABLE_H

#include "nodeStructure.h"

void UpdateForwardingTable(Host *HostNode, char *LeavingId);
void InsertInForwardingTable(Host *HostNode, int neigh, int dest);
Node *CheckForwardingTable(Host *HostNode, char *dest);
void ClearForwardingTable(Host *HostNode);

#endif /* FORWARDING_TABLE_H */
