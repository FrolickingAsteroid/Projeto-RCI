#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "newMod.h"
#include "withdrawMod.h"

#include "../Common/formatChecking.h"
#include "../Common/utils.h"

#include "../HostStruct/forwardingTable.h"
#include "../HostStruct/ncQueue.h"
#include "../HostStruct/CirBuffer.h"

#include "../Protocols/eventManager.h"
#include "../SocketProcessing/socketInterface.h"

#define TOKENSIZE 256 // Size of protocol args
#define MSGSIZE 1024  // message size

/**
 * @brief Reads input from a newly established socket and performs the following tasks:
 *
 * - Parses the input to extract the new ID, IP, and TCP port.
 * - Checks the validity of the input using the BootArgsCheck() function.
 * - If the host node is alone in the network, creates an anchor
 *   and adds the new node as an external node to the host structure.
 * - If the host node has an external node already, sends a message to the new socket with
 *   the external node's ID, IP, and TCP port and adds the new node as an internal node to
 *   the host structure.
 *
 * @param HostNode: A pointer to the host node structure.
 * @param NewCon: A pointer to a new connection waiting to be processed
 */
void HandleNewCon(Host *HostNode, NewConQueue *NewCon) {
  char Buffer[TOKENSIZE] = "", msg[MSGSIZE] = "";
  char NewId[TOKENSIZE] = "", NewIp[TOKENSIZE] = "", NewTCP[TOKENSIZE] = "";

  // Read the data from the circular buffer
  if (!CbRead(NewCon->Cb, Buffer, sizeof(Buffer) - 1)) {
    return;
  }

  // Parse the input arguments
  if (sscanf(Buffer, "NEW %s %s %s\n", NewId, NewIp, NewTCP) == 0) {
    close(NewCon->NewFd), RemoveNC(HostNode, NewCon->NewFd);
    return;
  }

  // Validate the input arguments
  if (!(BootArgsCheck(NewId, NewIp, NewTCP))) {
    close(NewCon->NewFd), RemoveNC(HostNode, NewCon->NewFd);
    return;
  }

  // print answer in verbose mode
  ServerAnswer(Buffer, "New TCP connection request");

  // Prepare response message
  if (HostNode->Ext == NULL) {
    sprintf(msg, "EXTERN %s %s %s\n", NewId, NewIp, NewTCP);
  } else {
    sprintf(msg, "EXTERN %s %s %d\n", HostNode->Ext->Id, HostNode->Ext->IP, HostNode->Ext->TCPort);
  }

  // Send response message
  if (CustomWrite(NewCon->NewFd, msg, strlen(msg)) == -1) {
    close(NewCon->NewFd), RemoveNC(HostNode, NewCon->NewFd);
    return;
  }

  // Update host node data structure
  if (HostNode->Ext == NULL) {
    HostNode->Ext = InitNode(NewIp, atoi(NewTCP), NewId, NewCon->NewFd);
  } else {
    PlugIntern(NewIp, atoi(NewTCP), NewId, NewCon->NewFd, HostNode);
  }

  // Insert new connection in the forwarding table
  InsertInForwardingTable(HostNode, atoi(NewId), atoi(NewId));

  // Remove the new connection from the temporary new connections list
  RemoveNC(HostNode, NewCon->NewFd);
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
  char Id[TOKENSIZE] = "", Ip[TOKENSIZE] = "", PortTCP[TOKENSIZE] = "";

  // Free old bck
  FreeNode(HostNode->Bck), HostNode->Bck = NULL;

  // scan bck info
  if (sscanf(Buffer, "EXTERN %s %s %s\n", Id, Ip, PortTCP) != 3 ||
      BootArgsCheck(Id, Ip, PortTCP) == 0) {
    WithdrawHandle(HostNode, HostNode->Ext->Id, HostNode->Ext->Fd);
    return;
  }

  ServerAnswer(Buffer, "Neighbouring TCP connection answer");

  // Update bck node
  if (strcmp(HostNode->HostId, Id) != 0) {
    HostNode->Bck = InitNode(Ip, atoi(PortTCP), Id, -1);
    InsertInForwardingTable(HostNode, atoi(HostNode->Ext->Id), atoi(HostNode->Bck->Id));
  }
  InsertInForwardingTable(HostNode, atoi(HostNode->Ext->Id), atoi(HostNode->Ext->Id));
}
