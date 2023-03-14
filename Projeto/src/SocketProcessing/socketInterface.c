#include <stddef.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include "socketInterface.h"
#include "withdrawMod.h"

#define MAXSIZE 128

/**
 * @brief Parses the received buffer and calls the appropriate message handler function
 *
 * @param buffer: The buffer containing the received protocol
 * @param hostNode: The pointer to the host node receiving the protocol
 */
void SocketInterfaceParser(char *Buffer, Host *HostNode) {
  char Token[64] = "";

  // Parse type of message from buffer
  sscanf(Buffer, "%s", Token);

  // Call appropriate message handler function
  if (strcmp(Token, "EXTERN") == 0) {
    ExternHandle(Buffer, HostNode);
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
void SendProtocolMsg(Host *HostNode, char *msg) {
  size_t MsgLen = strlen(msg);

  for (Node *temp = HostNode->NodeList; temp != NULL; temp = temp->next) {
    if (write(temp->Fd, msg, MsgLen) == -1) {
      // if connection is not available continue
      perror("Function LeaveNetwork >> " RED "☠  write() failed");
      continue;
    };
  }

  if (HostNode->Ext != NULL) {
    if (write(HostNode->Ext->Fd, msg, MsgLen) == -1) {
      perror("Function LeaveNetwork >> " RED "☠  write() failed");
    };
  }
}
