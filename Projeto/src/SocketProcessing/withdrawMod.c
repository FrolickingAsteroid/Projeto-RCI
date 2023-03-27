#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "withdrawMod.h"

#include "socketInterface.h"

#include "../Common/utils.h"
#include "../Common/formatChecking.h"

#include "../HostStruct/forwardingTable.h"

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
      if (!SendNewMsg(HostNode, HostNode->HostId, HostNode->Ext->IP, BootTCP)) {
        // remove new extern if connection fails
        WithdrawHandle(HostNode, HostNode->Ext->Id, HostNode->Ext->Fd);
      }
    } else {
      FreeNode(HostNode->Ext);
      HostNode->Ext = HostNode->NodeList;
      // verify if there were only two nodes in the network
      if (HostNode->NodeList != NULL) {
        HostNode->NodeList = HostNode->NodeList->next;
      }
    }
    if (HostNode->Ext != NULL) {
      SendExternMsg(HostNode);
    }
  }

  // if intern withdraws remove it from the list
  else {
    RemoveIntern(HostNode, LeavingId);
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
  sprintf(msg, "EXTERN %s %s %d\n", HostNode->Ext->Id, HostNode->Ext->IP, HostNode->Ext->TCPort);

  // send EXTERN ext to all intern neighbours only if host is not alone
  for (Node *current = HostNode->NodeList; current != NULL; current = current->next) {
    if (CustomWrite(current->Fd, msg, (size_t)strlen(msg)) == -1) {
      perror("Function SendExternMsg >> " RED "☠  write() failed");
      continue;
    };
  }
  // check if leaving node was an ancor, notify new extern of bck change
  if (HostNode->Bck == NULL) {
    if (CustomWrite(HostNode->Ext->Fd, msg, (size_t)strlen(msg)) == -1) {
      perror("Function SendExternMsg >> " RED "☠  write() failed");
    };
  }
}

/**
 * @brief Builds a withdraw message and sends it to the specified sender.
 *
 * This function constructs a withdraw message containing the leaving node ID and brodcasts it using
 * the SendProtocolMsg function.
 *
 * @param HostNode: A pointer to the host node.
 * @param LeavingId: A pointer to a string representing the ID of the leaving node.
 * @param SenderFd: The file descriptor of the sender to whom the message shall not be sent.
 */
void BuildWithdrawMessage(Host *HostNode, char *LeavingId, int SenderFd) {
  char msg[256] = "";

  sprintf(msg, "WITHDRAW %s\n", LeavingId);
  SendProtocolMsg(HostNode, msg, SenderFd);
}

/**
 * @brief Processes a received withdraw message and updates the forwarding table.
 *
 * This function parses a received withdraw message containing the ID of a leaving node.
 * If the message is valid, it updates the forwarding table and forwards the message
 * to the other connected nodes using the SendProtocolMsg function.
 *
 * @param HostNode A pointer to the host node.
 * @param Buffer A pointer to the buffer containing the received message.
 * @param SenderFd The file descriptor of the sender who sent the withdraw message.
 */
void ReceiveWithdrawMsg(Host *HostNode, char *Buffer, int SenderFd) {
  char LeavingId[64] = "";

  if (sscanf(Buffer, "WITHDRAW %s\n", LeavingId) < 1) {
    return;
  }

  if (strlen(LeavingId) != 2 || !IsNumber(LeavingId)) {
    return;
  }

  ServerAnswer(Buffer, "Neighbouring TCP connection");
  UpdateForwardingTable(HostNode, LeavingId);
  SendProtocolMsg(HostNode, Buffer, SenderFd);
}
