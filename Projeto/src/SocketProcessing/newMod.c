#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "newMod.h"

#include "../Common/formatChecking.h"
#include "../Common/utils.h"

#include "../HostStruct/forwardingTable.h"
#include "../HostStruct/ncQueue.h"
#include "../HostStruct/CirBuffer.h"

#include "../Protocols/eventManager.h"
#include "../SocketProcessing/socketInterface.h"

#define MAXSIZE 256
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
 * @param Buffer: new Fd request
 * @param NewFd: The file descriptor of the newly established socket.
 *
 * @return void.
 */
void HandleNewCon(Host *HostNode, NewConQueue *NewCon) {
  char Buffer[MAXSIZE] = "";
  char msg[MAXSIZE << 2] = "";
  char NewId[MAXSIZE] = "", NewIp[MAXSIZE] = "", NewTCP[MAXSIZE] = "";

  if (!CbRead(NewCon->Cb, Buffer, sizeof(Buffer))) {
    return;
  }

  // fetch input args and check validity
  if (sscanf(Buffer, "NEW %s %s %s\n", NewId, NewIp, NewTCP) == 0) {
    RemoveNC(HostNode, NewCon->NewFd);
    return;
  }

  if (!(CheckNumberOfArgs(Buffer, 3) && BootArgsCheck(NewId, NewIp, NewTCP))) {
    RemoveNC(HostNode, NewCon->NewFd);
    return;
  }

  ServerAnswer(Buffer, "New TCP connection request");

  // if host is alone in the network send back the received information: create an ancor
  if (HostNode->Ext == NULL) {
    sprintf(msg, "EXTERN %s %s %s\n", NewId, NewIp, NewTCP);
    // send message back to intern
    if (write(NewCon->NewFd, msg, strlen(msg)) == -1) {
      perror("Function HandleNewCon >> " RED "☠  write() failed");
      return;
    }

    // plug extern into structure and set host as bck -> NULL
    HostNode->Ext = InitNode(NewIp, atoi(NewTCP), NewId, NewCon->NewFd);

  } else {
    sprintf(msg, "EXTERN %s %s %d", HostNode->Ext->Id, HostNode->Ext->IP, HostNode->Ext->TCPort);
    // send message to potential extern
    if (write(NewCon->NewFd, msg, strlen(msg)) == -1) {
      perror("Function HandleNewCon >> " RED "☠  write() failed");
      return;
    }
    // set new intern
    PlugIntern(NewIp, atoi(NewTCP), NewId, NewCon->NewFd, HostNode);
  }
  // insert in forwarding table
  InsertInForwardingTable(HostNode, atoi(NewId), atoi(NewId));
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
  char Id[MAXSIZE] = "", Ip[MAXSIZE] = "", PortTCP[MAXSIZE] = "";

  // scan bck info
  sscanf(Buffer, "EXTERN %s %s %s", Id, Ip, PortTCP);

  // check format
  if (!BootArgsCheck(Id, Ip, PortTCP)) {
    return;
  }

  ServerAnswer(Buffer, "Neighbouring TCP connection answer");

  FreeNode(HostNode->Bck);
  HostNode->Bck = NULL;

  // Update bck node
  if (strcmp(HostNode->HostId, Id) != 0) {
    HostNode->Bck = InitNode(Ip, atoi(PortTCP), Id, -1);
    InsertInForwardingTable(HostNode, atoi(HostNode->Ext->Id), atoi(HostNode->Bck->Id));
  }
  InsertInForwardingTable(HostNode, atoi(HostNode->Ext->Id), atoi(HostNode->Ext->Id));
}
