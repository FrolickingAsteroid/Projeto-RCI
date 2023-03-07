#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "Common/checkInvocationInfo.h"
#include "Common/nodeStructure.h"
#include "Common/utils.h"
#include "Protocols/TCP.h"
#include "Protocols/eventManager.h"

#define FOREVER while (1)

int main(int argc, char *argv[]) {
  UsrInvoc *UsrInfo = InvocCheck(argc, argv);
  int Fd = TCPServer(UsrInfo);
  Host *HostNode = InitHostStructure(Fd, UsrInfo);

  srand((unsigned int)time(0)); // seed the rand function

  // Main loop
  clear();
  printf(BLU "# User Interface Activated " GRN "🗹 \n" RESET);

  FOREVER {
    printf(KMAG ">>> " RESET);
    fflush(stdout);
    // Event Parser and Manage
    EventManager(HostNode);
  }
  return EXIT_SUCCESS;
}
