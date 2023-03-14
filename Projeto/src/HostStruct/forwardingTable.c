#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "forwardingTable.h"

#define TABLESIZE 100
/**
 * @brief Updates the forwarding table of a host when another node leaves the network.
 *
 *This function updates the forwarding table of the host pointed to by HostNode when
 * another node with ID LeavingId leaves the network. It goes through each entry in the
 * forwarding table and sets the entry to -1 if it matches the LeavingId. Then it creates
 * a Withdraw message for each affected entry and sends it to all neighbors using the
 * SendWithdrawMsg function.

 * @param HostNode: Pointer to the host node.
 * @param LeavingId: ID of the node that left the network.*
 *  */
void UpdateForwardingTable(Host *HostNode, char *LeavingId) {
  char withdraw[64] = "", msg[64] = "";

  for (int i = 0; i < TABLESIZE; i++) {
    if (HostNode->ForTable[i] == atoi(LeavingId)) {
      HostNode->ForTable[i] = -1;

      sprintf(withdraw, "WITHDRAW %d\n", i);
      strcat(msg, withdraw);
    }
  }
  SendProtocolMsg(HostNode, msg);
}
