#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Name.h"
#include "../Common/utils.h"

/**
 * Create a new Name struct from the given buffer.
 * @param Buffer The content to be copied into the new Name struct.
 * @return Pointer to the newly created Name struct.
 */
Name *CreateNewName(char *Buffer) {
  // Allocate memory for a new Name
  Name *NewName = (Name *)calloc(1, sizeof(Name));
  if (NewName == NULL) {
    DieWithSys("Function CreateNewName: calloc() failed");
  }

  // Copy the given string into the name's content and set the next pointer to NULL
  sprintf(NewName->Content, "%s", Buffer);
  NewName->next = NULL;
  // Return the new Name struct
  return NewName;
}

/**
 * Add a Name struct to the host's Name list.
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
