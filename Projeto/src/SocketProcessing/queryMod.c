#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include "queryMod.h"
#include "../Common/utils.h"
#include "../HostStruct/Name.h"
#include "../HostStruct/forwardingTable.h"
#include "socketInterface.h"

#define TOKENSIZE 256

/**
 * Handles incoming QUERY messages from other nodes in the network.
 * @param HostNode Pointer to the host node structure.
 * @param Buffer The received message buffer.
 * @param SenderNode Pointer to the sender node structure.
 */
void QueryHandle(Host *HostNode, char *Buffer, Node *SenderNode) {
  char Dest[TOKENSIZE] = "";
  char Orig[TOKENSIZE] = "";
  char Name[TOKENSIZE] = "";
  char Query[TOKENSIZE << 2] = "";

  // Parse the QUERY message
  if (sscanf(Buffer, "QUERY %s %s %s", Dest, Orig, Name) < 3) {
    // DO SOMETHING
  }

  /*! TODO: Parse QUERY*/

  // Update the forwarding table with QUERY info
  InsertInForwardingTable(HostNode, atoi(SenderNode->Id), atoi(Orig));

  // If the QUERY reached the target destination, search for the content
  if (strcmp(Dest, HostNode->HostId) == 0) {
    if (NameExists(HostNode, Name)) {
      // If content exists, invert the order and send the CONTENT message back
      SendContent(SenderNode->Fd, Orig, Dest, Name);
      return;

    } else {
      // Otherwise, send a NOCONTENT message
      SendNoContent(SenderNode->Fd, Orig, Dest, Name);
      return;
    }
  }
  // Write QUERY message
  sprintf(Query, "QUERY %s %s %s\n", Dest, HostNode->HostId, Name);

  // Check if path to destiny is known
  Node *Neigh = CheckForwardingTable(HostNode, Dest);
  if (Neigh != NULL) {
    if (write(Neigh->Fd, Query, 1024) == -1) {
      // DO SOMETHING
    }

  } else {
    // Forward the QUERY message to the all nodes
    SendProtocolMsg(HostNode, Query, SenderNode->Fd);
  }
}

/**
 * Sends a CONTENT message to the given node.
 * @param neighFd The file descriptor of the neighboring node.
 * @param Dest The destination node's ID.
 * @param Orig The origin node's ID.
 * @param Name The content name.
 */
void SendContent(int neighFd, char *Dest, char *Orig, char *Name) {
  char msg[TOKENSIZE << 1] = "";

  // Construct the CONTENT message
  sprintf(msg, "CONTENT %s %s %s\n", Dest, Orig, Name);

  // Write the CONTENT message to the neighboring node
  if (write(neighFd, msg, 512) == -1) {
    // DO SOMETHING
  }
}

/**
 * Sends a NOCONTENT message to the given node.
 * @param neighFd The file descriptor of the neighboring node.
 * @param Dest The destination node's ID.
 * @param Orig The origin node's ID.
 * @param Name The content name.
 */
void SendNoContent(int neighFd, char *Dest, char *Orig, char *Name) {
  char msg[TOKENSIZE << 1] = "";

  // Construct the NOCONTENT message
  sprintf(msg, "NOCONTENT %s %s %s\n", Dest, Orig, Name);

  // Write the NOCONTENT message to the neighboring node
  if (write(neighFd, msg, 512) == -1) {
    // DO SOMETHING
  }
}

void ContentHandle(Host *HostNode, char *Buffer, int ContentFlag, int SenderFd) {
  char Dest[TOKENSIZE] = "";
  char Orig[TOKENSIZE] = "";
  char Name[TOKENSIZE] = "";

  // Received CONTENT message
  if (ContentFlag) {
    sscanf(Buffer, "CONTENT %s %s %s", Dest, Orig, Name);

    // Chek if msg reached destiny
    if (strcmp(HostNode->HostId, Dest) == 0) {
      fprintf(stdout, GRN "🗹 SUCCESS > " RESET "Message was found in %s\n", Orig);
      return;
    }
  } else {
    sscanf(Buffer, "NOCONTENT %s %s %s", Dest, Orig, Name);

    if (strcmp(HostNode->HostId, Dest) == 0) {
      fprintf(stdout, RED "☒ FAILURE > " RESET "Message was not found in %s\n", Orig);
      return;
    }
  }

  // Check if path to destiny is known
  Node *Neigh = CheckForwardingTable(HostNode, Dest);
  if (Neigh != NULL) {
    if (write(Neigh->Fd, Buffer, 1024) == -1) {
      // DO SOMETHING
    }

  } else {
    // Forward the CONTENT/NOCONTENT message to the all nodes
    SendProtocolMsg(HostNode, Buffer, SenderFd);
  }
}
