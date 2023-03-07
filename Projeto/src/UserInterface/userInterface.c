#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "userInterface.h"

/**
 * @brief Parses User Input Command and calls processing functions
 *
 * @param buffer -> user string written to STDIN_NO
 */
void UserInterfaceParser(char buffer[], Host *HostNode) {
  char Command[6];
  int FlagJoin = 0;

  // Parse Command from buffer
  sscanf(buffer, "%s", Command);

  // Chain of if/if-else for each command sent
  if (strcmp(Command, "join") == 0) {
    JoinNetworkServer(buffer, HostNode);

  } else if (strcmp(Command, "djoin") == 0) {
    DJoinNetworkServer(buffer, HostNode, FlagJoin);

  } else if (strcmp(Command, "clear") == 0) {
    clear();
    printf(BLU "# User Interface Activated " GRN "🗹 \n" RESET);
  }

  else {
    CommandNotFound("Command not found", buffer);
    return;
  }
}
