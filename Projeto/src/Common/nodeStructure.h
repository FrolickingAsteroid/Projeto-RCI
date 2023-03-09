#ifndef NODESTRUCTURE_H
#define NODESTRUCTURE_H

#include "checkInvocationInfo.h"
#include "utils.h"

// Type of Join Flag
typedef enum Command { IDLE, DJOIN, JOIN } Command;

typedef struct Name {

  char Name[100];
  struct Name *next;
} Name;

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
  Name *Name;

  UsrInvoc *InvocInfo;
  Command type;

  Node *Ext;
  Node *Bck;
  Node *NodeList;
} Host;

Host *InitHostStructure(int Fd, UsrInvoc *UsrInfo);
void PlugHostNetId(char *Net, char *Id, Host *HostNode);
void PlugIntern(char *Ip, int TCP, char *Id, int Fd, Host *HostNode);
Node *InitNode(char *Ip, int TCP, char *Id, int Fd);
void FreeNode(Node *Node);
void LiberateHost(Host *HostNode);

#endif
