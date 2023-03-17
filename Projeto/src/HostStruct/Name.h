//===-- name.h ------------------------------------------------------------===//
//
// This file contains the declarations for functions related to name manipulation such has creating
// a new name and adding it to the host's name list.
//
//===----------------------------------------------------------------------===//

#ifndef NAME_H
#define NAME_H

#include "../HostStruct/nodeStructure.h"

// Creates a new name with the given content.
Name *CreateNewName(char *bucks);

// Adds a new name to the host's name list.
void AddNameToHost(Host *HostNode, Name *NewName);

int NameExists(Host *HostNode, char *Name);

#endif // NAME_H
