#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Name.h"
#include "../Common/utils.h"
#include "nodeStructure.h"

/**
 * @brief Create a new Name struct from the given buffer.
 *
 * @param Buffer The content to be copied into the new Name struct.
 * @return Pointer to the newly created Name struct.
 */
Name *CreateNewName(char *Buffer) {
  // Allocate memory for a new Name
  Name *NewName = (Name *)calloc(1, sizeof(Name));
  if (NewName == NULL) {
    return NULL;
  }

  // Copy the given string into the name's content and set the next pointer to NULL
  sprintf(NewName->Content, "%s", Buffer);
  NewName->next = NULL;
  // Return the new Name struct
  return NewName;
}

/**
 * @brief Add a Name struct to the host's Name list.
 *
 * @param HostNode The host whose Namelist the new name will be added to.
 * @param NewName The new Name struct to be added to the host's Name list.
 */
void AddNameToHost(Host *HostNode, Name *NewName) {

  NewName->next = HostNode->NameList;
  HostNode->NameList = NewName;
}

int NameExists(Host *HostNode, char *Name) {
  struct Name *Current = HostNode->NameList;
  while (Current != NULL) {

    if (strcmp(Current->Content, Name) == 0) {
      return 1;
    }
    Current = Current->next;
  }
  return 0;
}

/**
 * @brief Frees the memory allocated for the NameList in the HostNode.
 *
 * This function iterates through the NameList in the HostNode, deallocating
 * each Name entry and setting the NameList to NULL when completed.
 *
 * @param HostNode Pointer to the Host structure containing the NameList to be freed.
 */
void FreeNameList(Host *HostNode) {

  Name *AuxName = NULL;

  while (HostNode->NameList != NULL) {
    AuxName = HostNode->NameList;
    HostNode->NameList = HostNode->NameList->next;
    free(AuxName);
  }
  HostNode->NameList = NULL;
}

void ClearNames(Host *HostNode) {
  if (HostNode->NameList == NULL) {
    CommandNotFound("No Names registered in Host", "Unable to clear name list\n");
    return;
  }

  FreeNameList(HostNode);
  fprintf(stdout, GRN "🗹 SUCCESS > " RESET "Cleared name list\n");
}
