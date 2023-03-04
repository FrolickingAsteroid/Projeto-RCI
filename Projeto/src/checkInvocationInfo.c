#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "checkInvocationInfo.h"
#include "utils.h"

#define PORT_RANGE 65535

// Init UsrInvoc
UsrInvoc *InitUsrInfo() {
  UsrInvoc *UsrInfo = calloc(1, sizeof(UsrInvoc));
  if (UsrInfo == NULL) {
    DieWithSys("Function InitUsrInfo >> calloc() failed");
  }
  UsrInfo->RegIP = "193.136.138.142";
  UsrInfo->RegUDP = 59000;
  return UsrInfo;
}

// Check Valid Adress
char *CheckValidAdress(char *IP, UsrInvoc *Usr) {
  struct sockaddr_in sa;

  if (inet_pton(AF_INET, IP, &(sa.sin_addr)) != 1) {
    free(Usr);
    DieWithUsr(IP, "Invalid IP format");
  };
  return IP;
}

// Check Valid PORT
int CheckValidPort(char *PORT, UsrInvoc *Usr) {

  if (IsNumber(PORT) == 0) {
    free(Usr);
    DieWithUsr(PORT, "Invalid TCP port format");
  }
  int TCP = atoi(PORT);

  if (TCP > PORT_RANGE) {
    free(Usr);
    DieWithUsr(PORT, "TCP port exceeds range of possible values");
  }
  return TCP;
}

// Usr invocation parcer
UsrInvoc *InvocCheck(int argc, char *argv[]) {

  if (argc != 3 && argc != 5) {
    DieWithUsr("Invalid Invocation", "Wrong number of arguments");
    Usage(argv[0]);
  }

  UsrInvoc *UsrInfo = InitUsrInfo();

  UsrInfo->HostIP = CheckValidAdress(argv[1], UsrInfo);
  UsrInfo->HostTCP = CheckValidPort(argv[2], UsrInfo);

  if (argc == 5) {
    UsrInfo->RegIP = CheckValidAdress(argv[3], UsrInfo);
    UsrInfo->RegUDP = CheckValidPort(argv[4], UsrInfo);
  }

  return UsrInfo;
}
