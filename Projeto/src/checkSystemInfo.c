#include <arpa/inet.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "checkInvocationInfo.h"

void usage(char *name) {
  printf("Invocation: %s IP TCP regIP[Opcional] regUDP[Opcional]\n", name);
}

int CheckValidAdress(char *IP) {

  /*! TODO: Check if struct needs to be freed and/or copy*/

  struct sockaddr_in sa;
  int result = inet_pton(AF_INET, IP, &(sa.sin_addr));
  return result;
}

int CheckValidPort(char *PORT) {

  /*! TODO: Check if only requirement is to be a number*/

  int j = 0;
  while (j < strlen(PORT)) {
    if (isdigit(PORT[j]) == 0)
      return 0;
    j++;
  }
  return 1;
}

void InvocCheck(int argc, char *argv[]) {

  if (argc < 3 || argc > 5) {
    usage(argv[0]);
    exit(1);
  }

  if ((CheckValidAdress(argv[1]) || CheckValidAdress(argv[3])) != 1) {
    printf("Invalid IP format\n");
    usage(argv[0]);
    exit(1);
  }

  if ((CheckValidPort(argv[2]) || CheckValidPort(argv[4])) != 1) {
    printf("Invalid PORT format\n");
    usage(argv[0]);
    exit(1);
  }
}
