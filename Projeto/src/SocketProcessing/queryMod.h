#ifndef QUERYMOD_H
#define QUERYMOD_H

#include "../HostStruct/nodeStructure.h"
void QueryHandle(Host *HostNode, char *Buffer, Node *SenderNode);
void SendContent(int neighFd, char *Dest, char *Orig, char *Name);
void SendNoContent(int neighFd, char *Dest, char *Orig, char *Name);
void ContentHandle(Host *HostNode, char *Buffer, int ContentFlag, Node *SenderNode);
#endif
