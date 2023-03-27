//===--- getmod.h --------------------------------------------------------------===//
//
// Functions for handling get, create, and delete commands related to message names.
//
//===-------------------------------------------------------------------------===//
#ifndef GETMOD_H
#define GETMOD_H

#include "../HostStruct/nodeStructure.h"
/// Handles the 'get' command to search for a message with a given name.
void GetName(Host *HostNode, char *Buffer);
/// Handles the 'create' command to create a new message with a given name.
void CreateName(Host *HostNode, char *Buffer);
/// Handles the 'delete' command to delete a message with a given name.
void DeleteName(Host *HostNode, char *Buffer);
#endif
