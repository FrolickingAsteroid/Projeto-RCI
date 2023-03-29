//===-- name.h --------------------------------------------------------------------------------===//
//
// This file contains the declarations for functions related to name manipulation such as creating
// a new name and adding it to the host's name list.
//
//===-----------------------------------------------------------------------------------------===//

#ifndef NAME_H
#define NAME_H

#include "../HostStruct/nodeStructure.h"

// Create a new Name struct with the provided content string
Name *CreateNewName(char *bucks);

// Add a new Name struct to the host's NameList
void AddNameToHost(Host *HostNode, Name *NewName);

// Check if a name exists in the host's NameList
int NameExists(Host *HostNode, char *Name);

// Free the memory allocated to the host's NameList
void FreeNameList(Host *HostNode);

void ClearNames(Host *HostNode);

#endif // NAME_H
