#ifndef CHECKSYSTEM_H
#define CHECKSYSTEM_H

#include "utils.h"

typedef struct UsrInvoc {
  char *HostIP;
  int HostTCP;
  char *RegIP;
  int RegUDP;
} UsrInvoc;

UsrInvoc *InitUsrInfo();
UsrInvoc *InvocCheck(int argc, char *argv[]);

char *CheckValidAdress(char *IP);
int CheckValidPort(char *PORT);

void DieWithUsr(const char *msg, const char *detail);

#endif
