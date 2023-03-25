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

#define TOKENSIZE 256

void GetName(Host *HostNode, char *Buffer) {
  char Dest[TOKENSIZE] = "";
  char Name[TOKENSIZE] = "";
  char Query[TOKENSIZE << 2] = "";

  if (sscanf(Buffer, "get %s %s", Dest, Name) < 2) {
    CommandNotFound("Command not found", Buffer);
  }
  if (HostNode->HostId != NULL && strcmp(Dest, HostNode->HostId) == 0) {
    if (NameExists(HostNode, Name)) {
      fprintf(stdout, GRN "🗹 SUCCESS > " RESET "Message has been found in %s\n", HostNode->HostId);
    }
  }

  sprintf(Query, "QUERY %s %s %s\n", Dest, HostNode->HostId, Name);

  // Check if path to destiny is known
  Node *Neigh = CheckForwardingTable(HostNode, Dest);
  if (Neigh != NULL) {
    if (CustomWrite(Neigh->Fd, Query, strlen(Query)) == -1) {
      // DO SOMETHING
    }
    return;
  }

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
  if (sscanf(Buffer, "create %s", Content) < 1) {
    CommandNotFound("Name missing", Buffer);
    return;
  }

  if (!CheckNumberOfArgs(Buffer, 1)) {
    return;
  }

  if (strlen(Content) > 100) {
    CommandNotFound(Content, "Name is too long");
    return;
  }

  if (!IsAlphanumeric(Content)) {
    CommandNotFound(Content, "Name can only contain alphanumeric charaters");
    return;
  }

  Name *NewName = CreateNewName(Content);
  AddNameToHost(HostNode, NewName);

  fprintf(stdout, GRN "🗹 SUCCESS > " RESET "Message has been added\n");
}

/**
 * @brief Deletes a name from the Host's NameList based on the input buffer.
 *
 * The function takes a HostNode and a Buffer as input. The Buffer should contain
 * the delete command followed by the content of the name to be deleted.
 * If the name is found in the HostNode's NameList, it is removed and a success
 * message is printed. If the name is not found, a warning message is printed.
 *
 * @param HostNode A pointer to the Host structure containing the NameList.
 * @param Buffer A string containing the delete command and the content of the name to be deleted.
 */
void DeleteName(Host *HostNode, char *Buffer) {
  char Content[TOKENSIZE];
  Name *Current = HostNode->NameList;
  Name *Del = NULL;

  if (sscanf(Buffer, "delete %s", Content) < 1) {
    CommandNotFound(Buffer, "Name missing\n");
    return;
  }

  if (Current != NULL && strcmp(Content, Current->Content) == 0) {
    HostNode->NameList = Current->next, free(Current);
    fprintf(stdout, GRN "🗹 SUCCESS > " RESET "Message has been added with success\n");
    return;

  } else if (Current != NULL) {
    while (Current->next != NULL) {

      if (strcmp(Content, Current->next->Content) == 0) {
        Del = Current->next, Current->next = Del->next, free(Del);
        fprintf(stdout, GRN "🗹 SUCCESS > " RESET "Message has been added with success\n");
        return;
      }
      Current = Current->next;
    }
  }
  printf("\x1B[31m🚩 WARNING >\x1B[0m The message '%s' was not found\n", Content);
}
