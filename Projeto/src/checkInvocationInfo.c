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
    DieWithSys("Function InitUsrInfo >>" RED "☠  calloc() failed");
  }
  UsrInfo->RegIP = "193.136.138.142";
  UsrInfo->RegUDP = 59000;
  return UsrInfo;
}

// Check Valid Adress
char *CheckValidAdress(char *IP) {
  struct sockaddr_in sa;

  if (inet_pton(AF_INET, IP, &(sa.sin_addr)) != 1) {
    DieWithUsr(IP, "Invalid IP format");
  }
  return IP;
}

// Check Valid PORT
int CheckValidPort(char *PORT) {

  if (IsNumber(PORT) == 0) {
    Usage("./cot");
    DieWithUsr(PORT, "Invalid TCP port format");
  }
  int TCP = atoi(PORT);

  if (TCP > PORT_RANGE || TCP < 0) {
    Usage("./cot");
    DieWithUsr(PORT, "TCP port exceeds range of possible values");
  }
  return TCP;
}

// Usr invocation parcer
UsrInvoc *InvocCheck(int argc, char *argv[]) {

  if (argc != 3 && argc != 5) {
    Usage(argv[0]);
    DieWithUsr("Invalid Invocation", "Wrong number of arguments");
  }

  UsrInvoc *UsrInfo = InitUsrInfo();

  UsrInfo->HostIP = CheckValidAdress(argv[1]);
  UsrInfo->HostTCP = CheckValidPort(argv[2]);

  if (argc == 5) {
    UsrInfo->RegIP = CheckValidAdress(argv[3]);
    UsrInfo->RegUDP = CheckValidPort(argv[4]);
  }

  return UsrInfo;
}
