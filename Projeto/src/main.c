#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>

#include "Common/checkInvocationInfo.h"
#include "HostStruct/nodeStructure.h"
#include "Common/utils.h"
#include "Protocols/TCP.h"
#include "Protocols/eventManager.h"

#define FOREVER while (1)

int main(int argc, char *argv[]) {
  UsrInvoc *UsrInfo = InvocCheck(argc, argv);
  int Fd = TCPServer(UsrInfo);
  Host *HostNode = InitHostStructure(Fd, UsrInfo);

  // ignore SIGPIPE
  signal(SIGPIPE, SIG_IGN);

  srand((unsigned int)time(0)); // seed the rand function

  // Main loop
  clear();
  fprintf(stdout, BLU "# User Interface Activated " GRN "🗹 \n" RESET);

  // Init Keyboard prompt
  printf(KMAG ">>> " RESET);
  fflush(stdout);

  FOREVER {

    // Event Parser and Manage
    EventManager(HostNode);
  }
  return EXIT_SUCCESS;
}
