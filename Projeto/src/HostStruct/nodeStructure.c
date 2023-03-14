#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "forwardingTable.h"
#include "nodeStructure.h"

/**
 * @brief
 *
 * @param Fd
 * @param UsrInfo
 * @return
 */
Host *InitHostStructure(int Fd, UsrInvoc *UsrInfo) {
  Host *node = calloc(1, sizeof(Host));
  if (node == NULL) {
    DieWithSys("Function InitHostStructure: calloc() failed");
  }

  memset(node->ForTable, -1, sizeof(node->ForTable));

  node->Name = NULL;
  node->HostId = NULL;
  node->Bck = NULL;
  node->Ext = NULL;
  node->FdListen = Fd;
  node->NodeList = NULL;
  node->InvocInfo = UsrInfo;
  node->Net = NULL;
  node->type = IDLE;
  return node;
}

/**
 * @brief
 *
 * @param Net
 * @param Id
 * @param HostNode
 */
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

  InsertInForwardingTable(HostNode, atoi(Id), atoi(Id));
}

/**
 * @brief
 *
 * @param Ip
 * @param TCP
 * @param Id
 * @param fd
 * @return
 */
Node *InitNode(char *Ip, int TCP, char *Id, int Fd) {
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
  Node->Fd = Fd;
  Node->TCPort = TCP;
  Node->next = NULL;

  sprintf(Node->Id, "%s", Id);
  sprintf(Node->IP, "%s", Ip);
  return Node;
}

void FreeNode(Node *Node) {
  if (Node == NULL) {
    return;
  }
  free(Node->IP);
  free(Node->Id);

  if (Node->Fd != -1) {
    close(Node->Fd);
  }

  free(Node);
  Node = NULL;
}

void LiberateHost(Host *HostNode) {
  Node *AuxNode = NULL;
  Name *AuxName = NULL;
  FreeNode(HostNode->Ext);
  FreeNode(HostNode->Bck);

  HostNode->Ext = HostNode->Bck = NULL;
  free(HostNode->HostId), free(HostNode->Net);

  memset(HostNode->ForTable, -1, sizeof(HostNode->ForTable));

  HostNode->type = IDLE;
  HostNode->Net = NULL;
  HostNode->HostId = NULL;

  while (HostNode->NodeList != NULL) {
    AuxNode = HostNode->NodeList;
    HostNode->NodeList = HostNode->NodeList->next;
    FreeNode(AuxNode);
  }

  while (HostNode->Name != NULL) {
    AuxName = HostNode->Name;
    HostNode->Name = HostNode->Name->next;
    free(AuxName);
  }
}

void PlugIntern(char *Ip, int TCP, char *Id, int Fd, Host *HostNode) {
  Node *NewIntern = InitNode(Ip, TCP, Id, Fd);

  NewIntern->next = HostNode->NodeList;
  HostNode->NodeList = NewIntern;
}
