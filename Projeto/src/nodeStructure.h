#ifndef NODESTRUCTURE_H
#define NODESTRUCTURE_H

#include "checkInvocationInfo.h"
#include "utils.h"

typedef struct Node {
  char *Id;
  int Fd;
  char *IP;
  int TCPort;

  struct Node *next;
} Node;

typedef struct Host {
  char *HostId;
  int FdListen;

  char *Net;

  UsrInvoc *InvocInfo;

  Node *Ext;
  Node *Bck;
  Node *NodeList;
} Host;

Host *InitHostStructure(int Fd, UsrInvoc *UsrInfo);
void PlugHostNetId(char *Net, char *Id, Host *HostNode);
Node *InitNode(char *Ip, int TCP, char *Id, int fd);
#endif
