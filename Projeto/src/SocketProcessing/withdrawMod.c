#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "withdrawMod.h"

#define BUFSIZE 256

/**
 * @brief Handles a withdrawal event by updating the host's neighbor list
 * and notifying the appropriate neighbors.
 *
 * @param hostNode A pointer to the Host struct representing the current node.
 * @param leavingId The ID of the node that is withdrawing.
 */
void WithdrawHandle(Host *HostNode, char *LeavingId) {
  char BootIp[BUFSIZE] = "", BootId[BUFSIZE] = "", BootTCP[BUFSIZE] = "";

  char msg[BUFSIZE << 1] = "", *TCPAnswer = NULL;
  Node *Current = HostNode->NodeList;
  Node *Del = NULL;

  // if extern withdraws evaluate
  if (strcmp(LeavingId, HostNode->Ext->Id) == 0) {
    // host has a bck
    if (HostNode->Bck != NULL) {
      FreeNode(HostNode->Ext);
      HostNode->Ext = HostNode->Bck, HostNode->Bck = NULL;

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

      free(TCPAnswer);
    } else {
      FreeNode(HostNode->Ext);
      HostNode->Ext = HostNode->NodeList;
      // verify if there were only two nodes in the network
      if (HostNode->NodeList != NULL) {
        HostNode->NodeList = HostNode->NodeList->next;
      }
    }
    SendExternMsg(HostNode);
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
    // UpdateForwardingTable(HostNode, LeavingId);
  }
}

/**
 * @brief Sends an "EXTERN ext" message to all internal neighbors. Notifies the new
 * external neighbor of a change in the backup, if the leaving node was an anchor.
 *
 * @param hostNode: A pointer to the Host struct.
 *
 * @note The function handles write errors by printing an error message to stderr.
 * @note The message is sent to all internal neighbors and the new external neighbor,
 *       if applicable.
 */
void SendExternMsg(Host *HostNode) {
  char msg[256] = "";

  if (HostNode->Ext == NULL) {
    return;
  }

  sprintf(msg, "EXTERN %s %s %d", HostNode->Ext->Id, HostNode->Ext->IP,
          HostNode->Ext->TCPort);

  // send EXTERN ext to all intern neighbours only if host is not alone
  for (Node *temp = HostNode->NodeList; temp != NULL; temp = temp->next) {
    if (write(temp->Fd, msg, (size_t)strlen(msg)) == -1) {
      perror("Function WithdrawHandle >> " RED "☠  write() failed");
      continue;
    };
  }
  // check if leaving node was an ancor, notify new extern of bck change
  if (HostNode->Bck == NULL) {
    if (write(HostNode->Ext->Fd, msg, (size_t)strlen(msg)) == -1) {
      perror("Function WithdrawHandle >> " RED "☠  write() failed");
    };
  }
}

/**
 * @brief Handles the reception of an EXTERN message from a neighboring node.
 *
 * This function parses the received message to extract the ID, IP, and TCP port number of
 * the sender's new extern node. It then verifies the format of the received arguments
 * using the BootArgsCheck() function.
 * If the arguments are valid, this function updates the host's backup node by freeing the
 * old one and initializing a new one with the received information.
 *
 * @param Buffer: The message buffer received from the neighboring host.
 * @param HostNode: A pointer to the host node structure.
 */
void ExternHandle(char *Buffer, Host *HostNode) {
  char Id[BUFSIZE] = "", Ip[BUFSIZE] = "", TCP[BUFSIZE] = "";

  // scan bck info
  sscanf(Buffer, "EXTERN %s %s %s", Id, Ip, TCP);

  // check format
  if (!BootArgsCheck(Id, Ip, TCP)) {
    return;
  }

  FreeNode(HostNode->Bck);
  HostNode->Bck = NULL;

  // Update bck node
  if (strcmp(HostNode->HostId, Id) == 0) {
    HostNode->Bck = InitNode(Ip, atoi(TCP), Id, -1);
  }
}
