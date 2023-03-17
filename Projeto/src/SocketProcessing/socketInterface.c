#include <stddef.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include "socketInterface.h"
#include "withdrawMod.h"
#include "newMod.h"

#define MAXSIZE 128

/**
 * @brief Parses the received buffer and calls the appropriate message handler function
 *
 * @param buffer: The buffer containing the received protocol
 * @param hostNode: The pointer to the host node receiving the protocol
 */
void SocketInterfaceParser(char *Buffer, Host *HostNode, Node *SenderNode) {
  char Token[64] = "";

  // Parse type of message from buffer
  sscanf(Buffer, "%s", Token);

  // Call appropriate message handler function
  if (strcmp(Token, "EXTERN") == 0) {
    ExternHandle(Buffer, HostNode);
  }
  if (strcmp(Token, "WITHDRAW") == 0) {
    ReceiveWithdrawMsg(HostNode, Buffer, SenderNode->Fd);
  } else {
    return;
  }
}

/**
 * @brief Sends a protocol message to all neighbouring nodes.
 *
 * This function sends the given protocol message to all neighbouring nodes. If the
 * write() function returns an error, the function will print a message to stderr and
 * continue to send the message to the remaining nodes.
 *
 * @param HostNode: pointer to host struct.
 * @param msg The protocol message to send.
 *
 */
void SendProtocolMsg(Host *HostNode, char *msg, int SenderFd) {
  size_t MsgLen = strlen(msg);
  // send protocol to interns
  for (Node *temp = HostNode->NodeList; temp != NULL; temp = temp->next) {
    if (SenderFd == temp->Fd) {
      continue;
    }

    printf("sent to neighbour: %s\n", temp->Id);
    if (write(temp->Fd, msg, MsgLen) == -1) {
      // if connection is not available continue
      perror("Function LeaveNetwork >> " RED "☠  write() failed");
      continue;
    }
  }

  // send protocol to extern
  if (HostNode->Ext != NULL && SenderFd != HostNode->Ext->Fd) {
    if (write(HostNode->Ext->Fd, msg, MsgLen) == -1) {
      perror("Function LeaveNetwork >> " RED "☠  write() failed");
    }
  }
}
