#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "forwardingTable.h"
#include "nodeStructure.h"

#include "../Common/utils.h"

#define TABLESIZE 100 // number of routig table entries
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

/**
 * @brief Inserts an entry in the forwarding table of a host.
 *
 * This function adds or updates an entry in the forwarding table of the given host node,
 * mapping a destination node to the next hop (neighbor) in the route.
 *
 * @param HostNode: A pointer to the host node containing the forwarding table.
 * @param neigh: The node ID of the next hop (neighbour) in the route.
 * @param dest: The node ID of the destination.
 */
void InsertInForwardingTable(Host *HostNode, int neigh, int dest) {
  HostNode->ForTable[dest] = neigh;
}

/**
 * @brief Checks the forwarding table of a host to find the next hop for a given destination.
 *
 * This function searches the forwarding table of a host node for a specified destination.
 * If found, it returns the node that is the next hop in the route to the destination.
 * If not found, it returns NULL.
 *
 * @param HostNode: A pointer to the host node containing the forwarding table.
 * @param dest: A pointer to a string representing the destination node ID.
 * @return A pointer to the next hop node in the route, or NULL if not found.
 */
Node *CheckForwardingTable(Host *HostNode, char *dest) {
  int neigh = HostNode->ForTable[atoi(dest)];
  if (neigh != -1) {

    if (neigh == atoi(HostNode->Ext->Id)) {
      return HostNode->Ext;

    } else {
      for (Node *Current = HostNode->NodeList; Current != NULL; Current = Current->next) {
        if (neigh == atoi(Current->Id)) {
          return Current;
        }
      }
    }
  }
  return NULL;
}

/**
 * @brief Clears the forwarding table of a host and adds itself as the first entry.
 *
 * This function resets the forwarding table of a host node by setting all entries to -1.
 * After clearing the table, it inserts the host itself as the first entry.
 *
 * @param HostNode: A pointer to the host node containing the forwarding table.
 */
void ClearForwardingTable(Host *HostNode) {
  if (HostNode->Net == NULL) {
    fprintf(stderr,
            RED "🚩 WARNING > " RESET "Not registered in a network, no forwarding table to clear\n");
  }
  // Reset all entries in the forwarding table to -1
  memset(HostNode->ForTable, -1, sizeof(HostNode->ForTable));

  // Insert itself in forwarding table after delete
  InsertInForwardingTable(HostNode, atoi(HostNode->HostId), atoi(HostNode->HostId));
}
