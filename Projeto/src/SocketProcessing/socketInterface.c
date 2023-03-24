#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include "socketInterface.h"

#include "newMod.h"
#include "queryMod.h"
#include "withdrawMod.h"

#include "../Common/utils.h"

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
  }
  if (strcmp(Token, "QUERY") == 0) {
    QueryHandle(HostNode, Buffer, SenderNode);
  }
  if (strcmp(Token, "CONTENT") == 0) {
    ContentHandle(HostNode, Buffer, 1, SenderNode);
  }
  if (strcmp(Token, "NOCONTENT") == 0) {
    ContentHandle(HostNode, Buffer, 0, SenderNode);
  }

  else {
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
  size_t MsgLen = strlen(msg) + 1; // NULL terminator
  // send protocol to interns
  for (Node *temp = HostNode->NodeList; temp != NULL; temp = temp->next) {
    if (SenderFd == temp->Fd) {
      continue;
    }

    if (CustomWrite(temp->Fd, msg, MsgLen) == -1) {
      // if connection is not available continue
      perror("Function SendProtocolMsg >> " RED "☠  write() failed");
      continue;
    }
  }

  // send protocol to extern
  if (HostNode->Ext != NULL && SenderFd != HostNode->Ext->Fd) {
    if (CustomWrite(HostNode->Ext->Fd, msg, MsgLen) == -1) {
      perror("Function SendProtocolMsg >> " RED "☠  write() failed");
    }
  }
}

/**
 * @brief Writes a message to a specified file descriptor.
 *
 * This function attempts to write a specified number of bytes from a message
 * to a given file descriptor. It handles partial writes by continuing to write
 * until the entire message has been sent or an error occurs.
 *
 * @param Fd: The file descriptor to write the message to.
 * @param Msg: A pointer to the message buffer to be written.
 * @param MsgSize: The size of the message in bytes.
 * @return The total number of bytes written or -1 in case of an error.
 */
ssize_t CustomWrite(int Fd, char *Msg, size_t MsgSize) {
  ssize_t TotalBytes = 0, BytesSent = 0;
  const char *Buffer = Msg;

  // Loop until the entire message has been written or an error occurs
  while (TotalBytes < (ssize_t)MsgSize) {
    BytesSent = write(Fd, Buffer + TotalBytes, MsgSize - (size_t)TotalBytes);

    // Check if an error occurred during the write operation
    if (BytesSent == -1) {
      return -1;
    }
    // Add bytes to stack
    TotalBytes += BytesSent;
  }

  // Return the total number of bytes written
  return TotalBytes;
}
