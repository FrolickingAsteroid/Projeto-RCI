#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "forwardingTable.h"
#include "nodeStructure.h"

#define TABLESIZE 100
/**
 * @brief Updates the forwarding table of a host when another node leaves the network.
 *
 *This function updates the forwarding table of the host pointed to by HostNode when
 * another node with ID LeavingId leaves the network. It goes through each entry in the
 * forwarding table and sets the entry to -1 if it matches the LeavingId.
 *
 * @param HostNode: Pointer to the host structure.
 * @param LeavingId: ID of the node that left the network.
 *  */
void UpdateForwardingTable(Host *HostNode, char *LeavingId) {
  int IntLeavingId = atoi(LeavingId);

  HostNode->ForTable[IntLeavingId] = -1;

  for (int i = 0; i < TABLESIZE; i++) {
    if (HostNode->ForTable[i] == IntLeavingId) {
      HostNode->ForTable[i] = -1;
    }
  }
}

void InsertInForwardingTable(Host *HostNode, int neigh, int dest) {
  HostNode->ForTable[dest] = neigh;
}
