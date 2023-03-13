#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "showMod.h"

/**
 * @brief  Prints the topology associated with the host
 * @note   ID and network of the host; IDs of external, internal, and backup neighbors
 * @param  *hostNode: pointer to the struct with host information
 * @retval None
 */
void ShowTopology(Host *hostNode) {
  // Print external node
  printf(GRN "External Node\n" RESET);
  if (hostNode->Ext != NULL) {
    printf("%s %s %d\n", hostNode->Ext->Id, hostNode->Ext->IP, hostNode->Ext->TCPort);
  } else {
    printf("None\n");
  }

  // Print internal nodes
  printf(GRN "Internal Nodes\n" RESET);
  if (hostNode->NodeList == NULL) {
    printf("None\n");
  } else {
    for (Node *temp = hostNode->NodeList; temp != NULL; temp = temp->next) {
      printf("%s %s %d\n", temp->Id, temp->IP, temp->TCPort);
    }
  }

  // Print backup node
  printf(GRN "Backup Node\n" RESET);
  if (hostNode->Bck != NULL) {
    printf("%s %s %d\n", hostNode->Bck->Id, hostNode->Bck->IP, hostNode->Bck->TCPort);
  } else {
    printf("None\n");
  }
}
