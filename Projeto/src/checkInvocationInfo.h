#ifndef CHECKSYSTEM_H
#define CHECKSYSTEM_H

typedef struct UsrInvoc {
  char *HostIP;
  int HostTCP;
  char *RegIP;
  int RegUDP;
} UsrInvoc;

UsrInvoc *InitUsrInfo();
UsrInvoc *InvocCheck(int argc, char *argv[]);

void Usage(char *name);
char *CheckValidAdress(char *IP, UsrInvoc *Usr);
int IsNumber(char *str);
int CheckValidPort(char *PORT, UsrInvoc *Usr);

void DieWithUsr(const char *msg, const char *detail);

#endif
