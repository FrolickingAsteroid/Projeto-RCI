#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "newMod.h"
#include "../Common/formatChecking.h"
#include "../HostStruct/forwardingTable.h"

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
void ReadListeningSock(Host *HostNode, char *Buffer, int NewFd) {
  char msg[MAXSIZE << 2] = "";
  char NewId[MAXSIZE] = "", NewIp[MAXSIZE] = "", NewTCP[MAXSIZE] = "";

  // read info from new established socket
  if (read(NewFd, Buffer, MAXSIZE) == -1) {
    perror("Function TCPClientExternConnect >> " RED "☠  recv() failed");
    return;
  }
  // fetch input args and check validity
  int nArgs = sscanf(Buffer, "NEW %s %s %s", NewId, NewIp, NewTCP);
  if (!(CheckNumberOfArgs(Buffer, nArgs) && BootArgsCheck(NewId, NewIp, NewTCP))) {
    close(NewFd);
    return;
  }

  // if host is alone in the network send back the received information: create an ancor
  if (HostNode->Ext == NULL) {
    sprintf(msg, "EXTERN %s %s %s", NewId, NewIp, NewTCP);
    // send message back to intern
    if (write(NewFd, msg, (size_t)strlen(msg)) == -1) {
      perror("Function ReadListeningSock >> " RED "☠  write() failed");
      return;
    }

    // plug extern into structure and set host as bck -> NULL
    HostNode->Ext = InitNode(NewIp, atoi(NewTCP), NewId, NewFd);

  } else {
    sprintf(msg, "EXTERN %s %s %d", HostNode->Ext->Id, HostNode->Ext->IP, HostNode->Ext->TCPort);
    // send message to potential extern
    if (write(NewFd, msg, (size_t)strlen(msg)) == -1) {
      perror("Function ReadListeningSock >> " RED "☠  write() failed");
      return;
    }
    // set new intern
    PlugIntern(NewIp, atoi(NewTCP), NewId, NewFd, HostNode);
  }
  // insert in forwarding table
  InsertInForwardingTable(HostNode, atoi(NewId), atoi(NewId));
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

  FreeNode(HostNode->Bck);
  HostNode->Bck = NULL;

  // Update bck node
  if (strcmp(HostNode->HostId, Id) != 0) {
    HostNode->Bck = InitNode(Ip, atoi(PortTCP), Id, -1);
    InsertInForwardingTable(HostNode, atoi(HostNode->Ext->Id), atoi(HostNode->Bck->Id));
  }
  InsertInForwardingTable(HostNode, atoi(HostNode->Ext->Id), atoi(HostNode->Ext->Id));
}
