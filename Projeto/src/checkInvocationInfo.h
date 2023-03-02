#ifndef CHECKSYSTEM_H
#define CHECKSYSTEM_H

typedef struct UsrInvoc {
  char *IP;
  int TCP;
  char *regIP;
  int regUDP;
} UsrInvoc;

UsrInvoc *InitUsrInfo();
UsrInvoc *InvocCheck(int argc, char *argv[]);

void Usage(char *name);
int CheckValidAdress(char *IP);
int CheckValidPort(char *PORT);

#endif
