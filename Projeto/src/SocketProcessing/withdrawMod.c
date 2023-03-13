#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "withdrawMod.h"

#define BUFSIZE 128

void WithdrawHandle(Host *HostNode, char *LeavingId) {
  char BootIp[BUFSIZE] = "", BootId[BUFSIZE] = "", BootTCP[BUFSIZE] = "";

  char msg[256] = "", *TCPAnswer = NULL;
  Node *Current = HostNode->NodeList;
  Node *Del = NULL;

  // if extern withdraws evaluate
  if (strcmp(LeavingId, HostNode->Ext->Id) == 0) {
    // host has a bck
    if (HostNode->Bck != NULL) {
      FreeNode(HostNode->Ext);
      HostNode->Ext = HostNode->Bck;
      HostNode->Bck = NULL;

      // connect with new extern and ask for bck
      sprintf(msg, "NEW %s %s %d\n", HostNode->HostId, HostNode->InvocInfo->HostIP,
              HostNode->InvocInfo->HostTCP);
      // convert port in order to pass argument
      sprintf(BootTCP, "%d", HostNode->Ext->TCPort);
      TCPAnswer = TCPClientExternConnect(HostNode, msg, HostNode->Ext->IP, BootTCP);

      // if connection is not established
      if (TCPAnswer == NULL) {
        // do something -> ask
      }
      sscanf(TCPAnswer, "EXTERN %s %s %s", BootId, BootIp, BootTCP);

      // if node is not an ancor, plug new bck into host
      if (strcmp(BootId, HostNode->HostId) != 0) {
        HostNode->Bck = InitNode(BootIp, atoi(BootTCP), BootId, -1);
      }
    } else {
      FreeNode(HostNode->Ext);
      HostNode->Ext = HostNode->NodeList;
      // verify if there were only two nodes in the network
      if (HostNode->NodeList != NULL) {
        HostNode->NodeList = HostNode->NodeList->next;
      }
    }
    // send EXTERN ext to all intern neighbours only if host is not alone
    if (HostNode->Ext != NULL) {
      memset(msg, 0, sizeof(msg));
      sprintf(msg, "EXTERN %s %s %d", HostNode->Ext->Id, HostNode->Ext->IP,
              HostNode->Ext->TCPort);

      for (Node *temp = HostNode->NodeList; temp != NULL; temp = temp->next) {
        // send message
        if (write(temp->Fd, msg, (size_t)strlen(msg)) == -1) {
          // if connection is not available continue
          perror("Function WithdrawHandle >> " RED "☠  write() failed");
          continue;
        };
      }
    }
    free(TCPAnswer);
  }

  // if intern withdraws remove it from the list
  // first in the list
  else if (strcmp(LeavingId, HostNode->NodeList->Id) == 0) {

    HostNode->NodeList = Current->next;
    FreeNode(Current);

    // any other place
  } else {

    while (Current != NULL) {
      if (strcmp(LeavingId, Current->next->Id) == 0) {
        Del = Current->next;
        Current->next = Del->next;
        FreeNode(Del);
        break;
      }
      Current = Current->next;
    }

    // Update Forwarding table and send withdraw to all neighbours
    UpdateForwardingTable(HostNode, LeavingId);
  }
}

void ExternHandle(char *Buffer, Host *HostNode) {
  char Id[BUFSIZE] = "", Ip[BUFSIZE] = "", TCP[BUFSIZE] = "";

  // scan bck info
  sscanf(Buffer, "EXTERN %s %s %s", Id, Ip, TCP);

  // check format
  if (!BootArgsCheck(Id, Ip, TCP)) {
    // do something
  }

  // Update bck node
  FreeNode(HostNode->Bck);
  HostNode->Bck = InitNode(Ip, atoi(TCP), Id, -1);
}

/**
 * @brief Send a WITHDRAW message to all of the host's neighbors
 *
 * This function sends a WITHDRAW message to all of the host's neighbors.
 * The message contains the  LeavingId, which will be used to identify the node that
 * is leaving the network. If the connection to a neighbor or extern is not available,
 * an error message will be printed and the function will continue with the next neighbor.
 *
 * @param HostNode:
 * */
void SendWithdrawMsg(Host *HostNode, char *msg) {

  for (Node *temp = HostNode->NodeList; temp != NULL; temp = temp->next) {
    // send message
    if (write(temp->Fd, msg, (size_t)strlen(msg)) == -1) {
      // if connection is not available continue
      perror("Function LeaveNetwork >> " RED "☠  write() failed");
      continue;
    };
  }

  if (HostNode->Ext != NULL) {
    // send Withdraw to extern node
    if (write(HostNode->Ext->Fd, msg, (size_t)strlen(msg)) == -1) {
      // if connection is not available continue
      perror("Function LeaveNetwork >> " RED "☠  write() failed");
    };
  }
}

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

  for (int i = 0; i < sizeof(HostNode->ForTable); i++) {
    if (HostNode->ForTable[i] == atoi(LeavingId)) {
      HostNode->ForTable[i] = -1;

      sprintf(withdraw, "WITHDRAW %d\n", i);
      strcat(msg, withdraw);
    }
  }
  SendWithdrawMsg(HostNode, msg);
}
