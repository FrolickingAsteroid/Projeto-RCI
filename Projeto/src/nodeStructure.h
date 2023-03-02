#ifndef NODESTRUCTURE_H
#define NODESTRUCTURE_H

#include "checkInvocationInfo.h"

typedef struct Node {
  int ID;
  int FD;
  char *IP;
  int TCPort;

  Node *next;
} Node;

typedef struct Host {
  int HostID;

  UsrInvoc *InvocInfo;

  Node *Ext;
  Node *Bck;
  Node *NodeList;
} Host;

#endif
