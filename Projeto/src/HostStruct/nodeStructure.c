#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "nodeStructure.h"
#include "forwardingTable.h"
#include "CirBuffer.h"
#include "ncQueue.h"
#include "Name.h"

#include "../Common/utils.h"

/**
 * @brief Initializes a Host structure with the given parameters.
 *
 * @param Fd: File descriptor for the listening socket.
 * @param UsrInfo: A pointer to a user invocation structure.
 *
 * @return A pointer to the initialized Host structure.
 */
Host *InitHostStructure(int Fd, UsrInvoc *UsrInfo) {
  Host *node = calloc(1, sizeof(Host));
  if (node == NULL) {
    DieWithSys("Function InitHostStructure: calloc() failed");
  }

  memset(node->ForTable, -1, sizeof(node->ForTable));

  node->HostId = NULL;
  node->Bck = NULL;
  node->Ext = NULL;
  node->FdListen = Fd;
  node->NameList = NULL;
  node->NodeList = NULL;
  node->NClist = NULL;
  node->InvocInfo = UsrInfo;
  node->Net = NULL;
  node->type = IDLE;
  return node;
}

/**
 * @brief Assigns the network ID and host ID to the given Host structure.
 *
 * @param Net: A pointer to a string representing the network ID.
 * @param Id: A pointer to a string representing the host ID.
 * @param HostNode: A pointer to the Host structure to be updated.
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
 * @brief Initializes a Node structure with the given parameters.
 *
 * @param Ip: A pointer to a string representing the IP address.
 * @param TCP: The TCP port number.
 * @param Id: A pointer to a string representing the node ID.
 * @param Fd: The file descriptor for the node's socket.
 *
 * @return A pointer to the initialized Node structure.
 */
Node *InitNode(char *Ip, int TCP, char *Id, int Fd) {
  // Init Node struct
  Node *Node = (struct Node *)malloc(sizeof(struct Node));
  if (Node == NULL) {
    DieWithSys("Function InitNode: malloc() failed");
  }

  // Init char dependent variables for string copy and plug aswell as CircularBuffer
  Node->Id = (char *)malloc((strlen(Id) + 1) * sizeof(char));
  if (Node->Id == NULL) {
    DieWithSys("Function InitNode: malloc() failed");
  }
  Node->IP = (char *)malloc((strlen(Ip) + 1) * sizeof(char));
  if (Node->IP == NULL) {
    DieWithSys("Function InitNode: malloc() failed");
  }
  Node->Cb = (CircularBuffer *)malloc(sizeof(CircularBuffer));
  if (Node->Cb == NULL) {
    DieWithSys("Function InitNode: malloc() failed");
  }

  CbInit(Node->Cb);
  Node->Fd = Fd;
  Node->TCPort = TCP;
  Node->next = NULL;

  sprintf(Node->Id, "%s", Id);
  sprintf(Node->IP, "%s", Ip);
  return Node;
}

/**
 * @brief Frees the memory associated with a Node structure.
 *
 * @param Node: A pointer to the Node structure to be freed.
 */
void FreeNode(Node *Node) {
  if (Node == NULL) {
    return;
  }

  free(Node->IP);
  free(Node->Id);
  free(Node->Cb);

  if (Node->Fd != -1) {
    close(Node->Fd);
  }

  free(Node);
  Node = NULL;
}

/**
 * @brief Frees the memory associated with a Host structure and resets its state.
 *
 * @param HostNode: A pointer to the Host structure to be freed.
 */
void LiberateHost(Host *HostNode) {
  FreeNode(HostNode->Ext);
  FreeNode(HostNode->Bck);

  HostNode->Ext = HostNode->Bck = NULL;
  free(HostNode->HostId), free(HostNode->Net);

  memset(HostNode->ForTable, -1, sizeof(HostNode->ForTable));

  HostNode->type = IDLE;
  HostNode->Net = NULL;
  HostNode->HostId = NULL;

  FreeNodeList(HostNode);
  FreeNameList(HostNode);
  FreeNCList(HostNode);
}

/**
 * @brief Frees the memory associated with a list of Node structures in a Host.
 *
 * @param HostNode: A pointer to the Host structure containing the list to be freed.
 */
void FreeNodeList(Host *HostNode) {
  Node *AuxNode = NULL;
  while (HostNode->NodeList != NULL) {
    AuxNode = HostNode->NodeList, HostNode->NodeList = HostNode->NodeList->next;
    FreeNode(AuxNode);
  }
  HostNode->NodeList = NULL;
}

/**
 * @brief Adds a new internal node to the host's list of nodes.
 *
 * @param Ip: A pointer to a string representing the IP address.
 * @param TCP: The TCP port number.
 * @param Id: A pointer to a string representing the node ID.
 * @param Fd: The file descriptor for the node's socket.
 * @param HostNode: A pointer to the Host structure.
 */
void PlugIntern(char *Ip, int TCP, char *Id, int Fd, Host *HostNode) {
  Node *NewIntern = InitNode(Ip, TCP, Id, Fd);

  NewIntern->next = HostNode->NodeList;
  HostNode->NodeList = NewIntern;
}

/**
 * @brief Removes an internal node from the host's list of nodes based on the leaving node ID.
 *
 * @param HostNode: A pointer to the Host structure.
 * @param LeavingId: A pointer to a string representing the ID of the leaving node.
 */
void RemoveIntern(Host *HostNode, char *LeavingId) {

  Node *Current = HostNode->NodeList;
  Node *Del = NULL;
  // if intern withdraws remove it from the list
  if (strcmp(LeavingId, HostNode->NodeList->Id) == 0) {
    HostNode->NodeList = Current->next;
    FreeNode(Current);

  } else {
    while (Current != NULL) {
      if (strcmp(LeavingId, Current->next->Id) == 0) {
        Del = Current->next, Current->next = Del->next;
        FreeNode(Del);
        break;
      }
      Current = Current->next;
    }
  }
}
