#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>

#include "getMod.h"

#include "../Common/utils.h"
#include "../Common/formatChecking.h"

#include "../HostStruct/Name.h"
#include "../HostStruct/forwardingTable.h"

#include "../SocketProcessing/socketInterface.h"

#define TOKENSIZE 256 // Protocol size arguments
#define MSGSIZE 1024  // Msg to send size

/**
 * @brief Handles the 'get' command to search for a message with a given name.
 *
 * This function processes the 'get' command, which searches for a message with the specified name
 * in the network. The function first checks if the host is registered in a network. If the name is
 * being searched in the current host, it checks if the message exists. If the message exists in the
 * host, a success message is displayed, otherwise, a failure message is displayed.
 *
 * If the destination is not the current host, the function checks if there's a known path to the
 * destination. If there's a known path, it sends the query to the neighbor. If the path is unknown,
 * it sends the query to all neighbors.
 *
 * @param HostNode: A pointer to the host for which to process the 'get' command.
 * @param Buffer: The input command string, including the 'get' command and its arguments.
 */
void GetName(Host *HostNode, char *Buffer) {
  char Dest[TOKENSIZE] = "";
  char Name[TOKENSIZE] = "";
  char Query[MSGSIZE] = "";

  // if Host is not registered in a network return
  if (HostNode->HostId == NULL) {
    CommandNotFound("Host does not belong to a network, register first", Buffer);
    return;
  }

  // parse command
  if (sscanf(Buffer, "get %s %s\n", Dest, Name) < 2) {
    CommandNotFound("Invalid argument invocation, 'get' must have 2 input arguments", Buffer);
    fprintf(stderr, YEL "> type 'help' for more information\n" RESET);
    return;
  }
  // check dest format
  if (strlen(Dest) != 2 || IsNumber(Dest) == 0) {
    CommandNotFound("Invalid argument invocation, 'Dest' must be a two digit number", Buffer);
    fprintf(stderr, YEL "> type 'help' for more information\n" RESET);
    return;
  }
  // check name validity
  if (!NameParser(Name)) {
    return;
  }

  // check if name is being searched in host
  if (strcmp(Dest, HostNode->HostId) == 0) {
    if (NameExists(HostNode, Name)) {
      fprintf(stdout, GRN "\n🗹 SUCCESS > " RESET "Name has been found in %s\n\n", HostNode->HostId);
    } else {
      fprintf(stdout, RED "\n☒ FAILURE > " RESET "Name was not found in %s\n\n", HostNode->HostId);
    }
    return;
  }

  sprintf(Query, "QUERY %s %s %s\n", Dest, HostNode->HostId, Name);

  // Check if path to destiny is known
  Node *Neigh = CheckForwardingTable(HostNode, Dest);
  if (Neigh != NULL) {
    if (CustomWrite(Neigh->Fd, Query, strlen(Query)) == -1) {
      return;
    }
    return;
  }
  // if path is unknown, send msg to all neighbours
  SendProtocolMsg(HostNode, Query, -1);
}

/**
 * @brief Create a name and add it to the host's name list.
 *
 * @param HostNode A pointer to the Host structure.
 * @param Buffer A pointer to the buffer containing the command.
 */
void CreateName(Host *HostNode, char *Buffer) {
  char Content[TOKENSIZE];
  // Parse args
  if (sscanf(Buffer, "create %s\n", Content) < 1) {
    CommandNotFound("Invalid argument invocation, 'create' must have 1 input argument", Buffer);
    fprintf(stderr, YEL "> type 'help' for more information\n" RESET);
    return;
  }

  // check Name validity
  if (!NameParser(Content)) {
    return;
  }
  // create a new name item and add it to host
  Name *NewName = CreateNewName(Content);
  if (NewName == NULL) {
    DieWithSys("Function CreateNewName >>" RED " calloc() failed", HostNode);
  }

  AddNameToHost(HostNode, NewName);

  fprintf(stdout, GRN "\n🗹 SUCCESS > " RESET "Name has been added to host\n\n");
}

/**
 * @brief Deletes a name from the Host's NameList based on the input buffer.
 *
 * The function takes a HostNode and a Buffer as input. The Buffer should contain
 * the delete command followed by the content of the name to be deleted.
 * If the name is found in the HostNode's NameList, it is removed and a success
 * message is printed. If the name is not found, a warning message is printed.
 *
 * @param HostNode: A pointer to the Host structure containing the NameList.
 * @param Buffer: A string containing the delete command and the content of the name to be deleted.
 */
void DeleteName(Host *HostNode, char *Buffer) {
  char Content[TOKENSIZE];
  Name *Current = HostNode->NameList, *Del = NULL;

  // Parse input args
  if (sscanf(Buffer, "delete %s\n", Content) < 1) {
    CommandNotFound("Invalid argument invocation, 'delete' must have 1 input argument", Buffer);
    fprintf(stderr, YEL "> type 'help' for more information\n" RESET);
    return;
  }

  // Check name validity
  if (!NameParser(Content)) {
    return;
  }

  // search for name and delete it from the list
  if (Current != NULL && strcmp(Content, Current->Content) == 0) {
    HostNode->NameList = Current->next, free(Current);
    fprintf(stdout, GRN "\n🗹 SUCCESS > " RESET "Name has been deleted with success\n\n");
    return;

  } else if (Current != NULL) {
    while (Current->next != NULL) {

      if (strcmp(Content, Current->next->Content) == 0) {
        Del = Current->next, Current->next = Del->next, free(Del);
        fprintf(stdout, GRN "\n🗹 SUCCESS > " RESET "Name has been deleted with success\n\n");
        return;
      }
      Current = Current->next;
    }
  }

  fprintf(stderr, RED "\n(!) WARNING > " RESET "Name '%s' was not found\n", Content);
}
