#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "socketInterface.h"
#include "withdrawMod.h"
#include "../UserInterface/joinMod.h"

#define BUFSIZE 256

/**
 * @brief Handles a withdrawal event by updating the host's neighbor list
 * and notifying the appropriate neighbors.
 *
 * @param hostNode A pointer to the Host struct representing the current node.
 * @param leavingId The ID of the node that is withdrawing.
 */
void WithdrawHandle(Host *HostNode, char *LeavingId, int SenderFd) {
  char BootTCP[BUFSIZE] = "";

  Node *Current = HostNode->NodeList;
  Node *Del = NULL;

  // Update Forwarding table and send withdraw to all neighbours
  UpdateForwardingTable(HostNode, LeavingId);
  BuildWithdrawMessage(HostNode, LeavingId, SenderFd);

  // if extern withdraws evaluate
  if (strcmp(LeavingId, HostNode->Ext->Id) == 0) {
    // host has a bck
    if (HostNode->Bck != NULL) {
      FreeNode(HostNode->Ext);
      HostNode->Ext = HostNode->Bck, HostNode->Bck = NULL;
      InsertInForwardingTable(HostNode, atoi(HostNode->Ext->Id), atoi(HostNode->Ext->Id));

      // convert port in order to pass argument
      sprintf(BootTCP, "%d", HostNode->Ext->TCPort);
      // Connect to extern and ask for bck
      SendNewMsg(HostNode, HostNode->HostId, HostNode->Ext->IP, BootTCP);
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
  else if (strcmp(LeavingId, HostNode->NodeList->Id) == 0) {
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
  sprintf(msg, "EXTERN %s %s %d", HostNode->Ext->Id, HostNode->Ext->IP, HostNode->Ext->TCPort);

  // send EXTERN ext to all intern neighbours only if host is not alone
  for (Node *current = HostNode->NodeList; current != NULL; current = current->next) {
    if (write(current->Fd, msg, (size_t)strlen(msg)) == -1) {
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

void BuildWithdrawMessage(Host *HostNode, char *LeavingId, int SenderFd) {
  char msg[256] = "";

  sprintf(msg, "WITHDRAW %s\n", LeavingId);
  SendProtocolMsg(HostNode, msg, SenderFd);
}

void ReceiveWithdrawMsg(Host *HostNode, char *Buffer, int SenderFd) {
  char LeavingId[64] = "";

  ServerAnswer(Buffer, "Neighbouring TCP connection");
  if (sscanf(Buffer, "WITHDRAW %s", LeavingId) < 1) {
    return;
  }

  if (strlen(LeavingId) != 2 || !IsNumber(LeavingId)) {
    return;
  }
  UpdateForwardingTable(HostNode, LeavingId);
  SendProtocolMsg(HostNode, Buffer, SenderFd);
}
