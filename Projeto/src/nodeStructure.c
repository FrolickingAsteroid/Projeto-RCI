#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "nodeStructure.h"

Host *InitHostStructure(int Fd, UsrInvoc *UsrInfo) {
  Host *node = calloc(1, sizeof(Host));
  if (node == NULL) {
    DieWithSys("Function InitHostStructure: calloc() failed");
  }

  node->HostId = NULL;
  node->Bck = NULL;
  node->Ext = NULL;
  node->FdListen = Fd;
  node->NodeList = NULL;
  node->InvocInfo = UsrInfo;
  node->Net = NULL;
  return node;
}

void PlugHostNetId(char *Net, char *Id, Host *HostNode) {

  // Init char dependent variables for string copy and plug
  HostNode->HostId = (char *)malloc(3 * sizeof(char));
  if (HostNode->HostId == NULL) {
    DieWithSys("Function PlugHostNetId: malloc() failed");
  }
  HostNode->Net = (char *)malloc(4 * sizeof(char));
  if (HostNode->Net == NULL) {
    DieWithSys("Function PlugHostNetId: malloc() failed");
  }
  sprintf(HostNode->HostId, "%s", Id);
  sprintf(HostNode->Net, "%s", Net);
}

Node *InitNode(char *Ip, int TCP, char *Id, int fd) {
  // Init Node struct
  Node *Node = (struct Node *)malloc(sizeof(struct Node));
  if (Node == NULL) {
    DieWithSys("Function InitNode: malloc() failed");
  }

  // Init char dependent variables for string copy and plug
  Node->Id = (char *)malloc((strlen(Id) + 1) * sizeof(char));
  if (Node->Id == NULL) {
    DieWithSys("Function InitNode: malloc() failed");
  }
  Node->IP = (char *)malloc((strlen(Ip) + 1) * sizeof(char));
  if (Node->IP == NULL) {
    DieWithSys("Function InitNode: malloc() failed");
  }
  Node->Fd = fd;
  Node->TCPort = TCP;

  sprintf(Node->Id, "%s", Id);
  sprintf(Node->IP, "%s", Ip);
  return Node;
}
