#include <stdio.h>
#include <stdlib.h>

#include "TCP.h"
#include "checkInvocationInfo.h"
#include "eventManager.h"
#include "utils.h"

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
    // Event Parcer //
  }
  return EXIT_SUCCESS;
}
