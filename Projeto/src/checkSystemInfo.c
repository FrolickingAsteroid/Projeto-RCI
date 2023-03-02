#include <arpa/inet.h>
#include <ctype.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "checkInvocationInfo.h"
#define PORT_RANGE 65535

// Usage Invocation
void Usage(char *name) {
  printf("Invocação: %s IP TCP regIP[Opcional] regUDP[Opcional]\n", name);
}

// Error Message and close -- Taken from Donahoo -- Place in Utils?
void DieWithUsr(const char *msg, const char *detail) {
  fputs(msg, stderr);
  fputs(": ", stderr);
  fputs(detail, stderr);
  fputc('\n', stderr);
  Usage("./cot");
  exit(EXIT_FAILURE);
}

// Init UsrInvoc
UsrInvoc *InitUsrInfo() {
  UsrInvoc *UsrInfo = calloc(1, sizeof(UsrInvoc));
  if (UsrInfo == NULL) {
    exit(EXIT_FAILURE);
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
    DieWithUsr(IP, "Formato IP inválido");
  };
  return IP;
}

// Check Valid PORT
int CheckValidPort(char *PORT, UsrInvoc *Usr) {

  if (IsNumber(PORT) == 0) {
    free(Usr);
    DieWithUsr(PORT, "Formato Porto Inválido");
  }
  int TCP = atoi(PORT);

  if (TCP > PORT_RANGE) {
    free(Usr);
    DieWithUsr(PORT, "Número Excede o Range Estipulado");
  }
  return TCP;
}

// Checks wether string has alphanumeric characthers
int IsNumber(char *str) {
  for (int i = 0; i < strlen(str); i++) {
    if (isdigit(str[i]) == 0)
      return 0;
  }
  return 1;
}

// Usr invocation parcer
UsrInvoc *InvocCheck(int argc, char *argv[]) {

  if (argc != 3 && argc != 5) {
    DieWithUsr("Invocação Inválida", "Número de Argumentos Incorreto");
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
