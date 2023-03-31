#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include "queryMod.h"

#include "../Common/utils.h"
#include "../Common/formatChecking.h"

#include "../HostStruct/Name.h"
#include "../HostStruct/forwardingTable.h"

#include "socketInterface.h"

#define TOKENSIZE 256 // Size of protocol args
#define MSGSIZE 512   // Size of msg to send

/**
 * @brief Checks the validity of destination, origin, and name parameters.
 *
 *  This helper function checks if the destination and origin strings have the correct format
 * (2 characters in length and are numbers). It also checks if the name string is valid
 * using the NameParser function.

 * @param Dest: Pointer to the destination string.
 * @param Orig: Pointer to the origin string.
 * @param Name: Pointer to the name string.
 * @return Returns 1 if all parameters are valid, otherwise returns 0.
 *
 */
static int CheckValidity(char *Dest, char *Orig, char *Name) {
  // check dest and orig format
  if (strlen(Dest) != 2 || IsNumber(Dest) == 0 || strlen(Orig) != 2 || IsNumber(Orig) == 0) {
    return 0;
  }
  // check name validity
  if (!NameParser(Name)) {
    return 0;
  }
  return 1;
}
/**
 * Handles incoming QUERY messages from other nodes in the network.
 *
 * @param HostNode Pointer to the host node structure.
 * @param Buffer The received message buffer.
 * @param SenderNode Pointer to the sender node structure.
 */
void QueryHandle(Host *HostNode, char *Buffer, Node *SenderNode) {
  char Dest[TOKENSIZE] = "";
  char Orig[TOKENSIZE] = "";
  char Name[TOKENSIZE] = "";

  // Parse the QUERY message
  if (sscanf(Buffer, "QUERY %s %s %s\n", Dest, Orig, Name) < 3) {
    return;
  }

  // Argument checker
  if (!CheckValidity(Dest, Orig, Name)) {
    return;
  }

  ServerAnswer(Buffer, "Neighbouring TCP connection request");

  // Update the forwarding table with QUERY info
  InsertInForwardingTable(HostNode, atoi(SenderNode->Id), atoi(SenderNode->Id));
  InsertInForwardingTable(HostNode, atoi(SenderNode->Id), atoi(Orig));

  // If the QUERY reached the target destination, search for the content
  if (strcmp(Dest, HostNode->HostId) == 0) {
    if (NameExists(HostNode, Name)) {
      // If content exists, invert the order and send the CONTENT message back
      SendContent(SenderNode->Fd, Orig, Dest, Name);
    } else {
      // Otherwise, send a NOCONTENT message
      SendNoContent(SenderNode->Fd, Orig, Dest, Name);
    }
    return;
  }

  // Check if path to destiny is known
  Node *Neigh = CheckForwardingTable(HostNode, Dest);
  if (Neigh != NULL) {
    if (CustomWrite(Neigh->Fd, Buffer, strlen(Buffer)) == -1) {
      return;
    }

  } else {
    // Forward the QUERY message to the all nodes
    SendProtocolMsg(HostNode, Buffer, SenderNode->Fd);
  }
}

/**
 * Sends a CONTENT message to the given node.
 *
 * @param neighFd The file descriptor of the neighboring node.
 * @param Dest The destination node's ID.
 * @param Orig The origin node's ID.
 * @param Name The content name.
 */
void SendContent(int neighFd, char *Dest, char *Orig, char *Name) {
  char msg[MSGSIZE] = "";

  // Construct the CONTENT message
  sprintf(msg, "CONTENT %s %s %s\n", Dest, Orig, Name);

  // Write the CONTENT message to the neighboring node
  if (CustomWrite(neighFd, msg, strlen(msg)) == -1) {
    return;
  }
}

/**
 * Sends a NOCONTENT message to the given node.
 *
 * @param neighFd The file descriptor of the neighboring node.
 * @param Dest The destination node's ID.
 * @param Orig The origin node's ID.
 * @param Name The content name.
 */
void SendNoContent(int neighFd, char *Dest, char *Orig, char *Name) {
  char msg[MSGSIZE] = "";

  // Construct the NOCONTENT message
  sprintf(msg, "NOCONTENT %s %s %s\n", Dest, Orig, Name);

  // Write the NOCONTENT message to the neighboring node
  if (CustomWrite(neighFd, msg, strlen(msg)) == -1) {
    return;
  }
}

/**
 * @brief Handles the content message by parsing, validating, and forwarding the message
 *        based on the ContentFlag.
 *
 * This function parses the message from the buffer based on the ContentFlag. If the message
 * is valid and reaches its destination, a success or failure message is printed based on
 * the ContentFlag. If the destination is known, the message is forwarded to the destination
 * node. If the destination is unknown, the message is forwarded to all connected nodes.

 * @param HostNode: Pointer to the host node structure.
 * @param Buffer: Pointer to the buffer containing the received message.
 * @param ContentFlag: Flag indicating whether the message contains content (1) or not (0).
 * @param SenderNode: Pointer to the sender node structure.
 */
void ContentHandle(Host *HostNode, char *Buffer, int ContentFlag, Node *SenderNode) {
  char Dest[TOKENSIZE] = "";
  char Orig[TOKENSIZE] = "";
  char Name[TOKENSIZE] = "";

  // Parse the message based on the ContentFlag
  if (ContentFlag) {
    if (sscanf(Buffer, "CONTENT %s %s %s\n", Dest, Orig, Name) != 3) {
      return;
    }
  } else {
    if (sscanf(Buffer, "NOCONTENT %s %s %s\n", Dest, Orig, Name) != 3) {
      return;
    }
  }

  // Argument checker
  if (!CheckValidity(Dest, Orig, Name)) {
    return;
  }
  ServerAnswer(Buffer, "Neighbouring TCP connection answer");

  // Insert into forwarding table the new path
  InsertInForwardingTable(HostNode, atoi(SenderNode->Id), atoi(SenderNode->Id));
  InsertInForwardingTable(HostNode, atoi(SenderNode->Id), atoi(Orig));

  // Check if the message reached its destination
  if (strcmp(HostNode->HostId, Dest) == 0) {
    if (ContentFlag) {
      // fix prompt print
      clear_stdout_line();
      fprintf(stdout, GRN "\n🗹 SUCCESS > " RESET "Name was found in %s\n\n", Orig);
      if (!Verbose) {
        prompt();
      }
    } else {
      clear_stdout_line();
      fprintf(stdout, RED "\n☒ FAILURE > " RESET "Name was not found in %s\n\n", Orig);
      if (!Verbose) {
        prompt();
      }
    }
    return;
  }

  // Check if the path to the destination is known
  Node *Neigh = CheckForwardingTable(HostNode, Dest);
  if (Neigh != NULL) {
    if (CustomWrite(Neigh->Fd, Buffer, strlen(Buffer)) == -1) {
      return;
    }
  } else {
    // Forward the CONTENT/NOCONTENT message to all nodes
    SendProtocolMsg(HostNode, Buffer, SenderNode->Fd);
  }
}
