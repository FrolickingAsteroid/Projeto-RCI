#include <arpa/inet.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

#include "TCP.h"
#include "checkInvocationInfo.h"
#include "eventManager.h"

#define KMAG "\x1B[35m"
#define RESET "\x1B[0m"
#define FOREVER while (1)

int main(int argc, char *argv[]) {
  int fd;
  UsrInvoc *UsrInfo = InvocCheck(argc, argv);
  fd = TCPServer(UsrInfo);

  // Main loop

  FOREVER {
    printf(KMAG ">>> " RESET);
    fflush(stdout);
    // Event Parser and Manage
    EventManager(fd);
  }
  return EXIT_SUCCESS;
}
