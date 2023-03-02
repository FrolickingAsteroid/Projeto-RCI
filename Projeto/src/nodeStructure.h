#ifndef NODESTRUCTURE_H
#define NODESTRUCTURE_H

typedef struct Node {
  int ID;
  int FD;
  char *IP;
  int TCPort;

  Node *next;
} Node;

typedef struct Host {
  int HostID;
  char *HostIP;
  int HostTCP;
  int FdUDP;

  char *RegIP;
  int RegTCP;

  Node *Ext;
  Node *Bck;
  Node *NodeList;
} Host;

#endif
