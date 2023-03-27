#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "showMod.h"
#include "../Common/utils.h"

/**
 * @brief Display the network topology information for the given host node.
 *
 * This function prints the external node, internal nodes, and backup node information
 * for the given host node. It displays the node IDs, IP addresses, and TCP ports.
 *
 * @param hostNode: Pointer to the Host structure representing the local host node.
 */

void ShowTopology(Host *hostNode) {

  // Check if node is in a network
  if (hostNode->Net == NULL) {
    fprintf(stderr,
            RED "🚩 WARNING > " RESET "Not registered in a network, can not show topology\n");
    return;
  }

  printf(BLU "\nHost Topology: ID %2s NET %3s\n", hostNode->HostId, hostNode->Net);

  // Print external node
  printf(GRN "+-------------------------+\n");
  printf("|       External Node     |\n");
  printf("+-------------------------+\n" RESET);
  if (hostNode->Ext != NULL) {
    printf("%s %s %d\n", hostNode->Ext->Id, hostNode->Ext->IP, hostNode->Ext->TCPort);
  } else {
    printf("None\n");
  }

  // Print internal nodes
  printf(GRN "+-------------------------+\n");
  printf("|     Internal Nodes      |\n");
  printf("+-------------------------+\n" RESET);
  if (hostNode->NodeList == NULL) {
    printf("None\n");
  } else {
    for (Node *temp = hostNode->NodeList; temp != NULL; temp = temp->next) {
      printf("%s %s %d\n", temp->Id, temp->IP, temp->TCPort);
    }
  }

  // Print backup node
  printf(GRN "+-------------------------+\n");
  printf("|      Backup Node        |\n");
  printf("+-------------------------+\n" RESET);
  if (hostNode->Bck != NULL) {
    printf("%s %s %d\n\n", hostNode->Bck->Id, hostNode->Bck->IP, hostNode->Bck->TCPort);

  } else if (hostNode->Ext == NULL) {
    printf("None\n\n");

  } else {
    printf("%s %s %d\n\n", hostNode->HostId, hostNode->InvocInfo->HostIP,
           hostNode->InvocInfo->HostTCP);
  }
}

/**
 * @brief Display the forwarding table for the given host node.
 *
 * This function prints the forwarding table of the host node, showing the
 * relationship between neighbors and destinations. The table is formatted
 * with headers for better readability.
 *
 * @param hostNode Pointer to the Host structure representing the local host node.
 */
void ShowForwardingTable(Host *hostNode) {

  // Check if node is in a network
  if (hostNode->Net == NULL) {
    fprintf(stderr, RED "🚩 WARNING > " RESET
                        "Not registered in a network, can not show forwarding table\n");
    return;
  }
  // Print table headers
  printf(GRN "+-----------+-------------+\n");
  printf("| neighbour | destination |\n");
  printf("+-----------+-------------+\n" RESET);

  for (int i = 0; i < 100; i++) {
    if (hostNode->ForTable[i] != -1) {
      printf("|     %02d    |     %02d      |\n", hostNode->ForTable[i], i);
      printf("+-----------+-------------+\n");
    }
  }
}

/**
 * @brief Display the list of names and their content in the given host node.
 *
 * This function iterates through the NameList of the host node and prints the
 * content of each name. The names are prefixed with a header for better readability.
 *
 * @param hostNode Pointer to the Host structure representing the local host node.
 */
void ShowNames(Host *HostNode) {
  // Print header
  printf(GRN "+----------------------+\n");
  printf("|    Names: Content    |\n");
  printf("+----------------------+\n" RESET);

  // Check if NameList is not empty
  if (HostNode->NameList != NULL) {
    int itemNumber = 1; // Initialize item number counter

    // Iterate through NameList and print the content of each name with item number
    for (Name *Current = HostNode->NameList; Current != NULL; Current = Current->next) {
      printf(GRN "%d." RESET " %s\n", itemNumber, Current->Content);
      itemNumber++; // Increment item number counter
    }
  }
}
