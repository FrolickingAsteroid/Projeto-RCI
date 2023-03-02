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
  printf("Invocation: %s IP TCP regIP[Opcional] regUDP[Opcional]\n", name);
}

// Init UsrInvoc
UsrInvoc *InitUsrInfo() {
  UsrInvoc *UsrInfo = calloc(1, sizeof(UsrInvoc));
  if (UsrInfo == NULL) {
    exit(EXIT_FAILURE);
  }
  return UsrInfo;
}

// Check Valid Adress
int CheckValidAdress(char *IP) {
  /*! TODO: Redo, needs struct.
   */
  int result = inet_pton(AF_INET, IP, (struct sockaddr_in *)NULL);
  return result;
}

// Chceck Valid PORT
int CheckValidPort(char *PORT) {

  /*! TODO: Check if only requirement is to be a number --> PORT_RANGER*/

  int j = 0;
  while (j < strlen(PORT)) {
    if (isdigit(PORT[j]) == 0)
      return 0;
    j++;
  }
  return 1;
}

UsrInvoc *InvocCheck(int argc, char *argv[]) {

  if (argc != 3 && argc != 5) {
    printf("Invalid Invocation format\n");
    Usage(argv[0]);
    exit(EXIT_FAILURE);
  }

  UsrInvoc *UsrInfo = InitUsrInfo();

  if ((CheckValidAdress(argv[1]) || CheckValidPort(argv[2])) != 1) {
    printf("Invalid Invocation format\n");
    usage(argv[0]);
    exit(1);
  }

  if ((CheckValidPort(argv[3]) || CheckValidPort(argv[4])) != 1) {
    printf("Invalid PORT format\n");
    usage(argv[0]);
    exit(1);
  }
}
