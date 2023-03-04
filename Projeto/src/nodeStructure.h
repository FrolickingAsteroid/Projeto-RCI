#ifndef NODESTRUCTURE_H
#define NODESTRUCTURE_H

#include "checkInvocationInfo.h"

typedef struct Node {
  int ID;
  int FD;
  char *IP;
  int TCPort;

  struct Node *next;
} Node;

typedef struct Host {
  int HostID;
  int FdListen;

  UsrInvoc *InvocInfo;

  Node *Ext;
  Node *Bck;
  Node *NodeList;
} Host;

Host *InitHostStructure(int Fd, UsrInvoc *UsrInfo);
#endif
