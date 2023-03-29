//===--- QueryMod.h --------------------------------------------------------===//
//
// Header file for Query module which handles querying and content dissemination.
//
//===----------------------------------------------------------------------===//
#ifndef QUERYMOD_H
#define QUERYMOD_H

#include "../HostStruct/nodeStructure.h"

// Handles querying dissemination in the system.
void QueryHandle(Host *HostNode, char *Buffer, Node *SenderNode);
// Sends a "content" message to a neighboring node
void SendContent(int neighFd, char *Dest, char *Orig, char *Name);
// Sends a "no content" message to a neighboring node.
void SendNoContent(int neighFd, char *Dest, char *Orig, char *Name);
// Handles incoming content messages in the system.
void ContentHandle(Host *HostNode, char *Buffer, int ContentFlag, Node *SenderNode);
#endif
