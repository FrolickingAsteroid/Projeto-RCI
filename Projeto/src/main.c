#include <stdio.h>
#include <stdlib.h>

#include "TCP.h"
#include "checkInvocationInfo.h"
#include "eventManager.h"
#include "nodeStructure.h"
#include "utils.h"

#define FOREVER while (1)

int main(int argc, char *argv[]) {
  UsrInvoc *UsrInfo = InvocCheck(argc, argv);
  int Fd = TCPServer(UsrInfo);
  Host *HostNode = InitHostStructure(Fd, UsrInfo);

  // Main loop

  FOREVER {
    printf(KMAG ">>> " RESET);
    fflush(stdout);
    // Event Parser and Manage
    EventManager(HostNode);
    // Event Parcer //
  }
  return EXIT_SUCCESS;
}
