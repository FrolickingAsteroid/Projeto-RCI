#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "exitMod.h"
#include "userInterface.h"

/**
 * @brief Parses User Input Command and calls processing functions
 *
 * @param buffer -> user string written to STDIN_NO
 */
void UserInterfaceParser(char buffer[], Host *HostNode) {
  char Command[6];

  // Parse Command from buffer
  sscanf(buffer, "%s", Command);

  // Chain of if/if-else for each command sent
  if (strcmp(Command, "join") == 0) {
    HostNode->type = JOIN; // place flag before entering join function
    JoinNetworkServer(buffer, HostNode);

  } else if (strcmp(Command, "djoin") == 0) {
    HostNode->type = DJOIN; // place flag before entering join function
    DJoinNetworkServer(buffer, HostNode);

  } else if (strcmp(Command, "clear") == 0) {
    clear();
    printf(BLU "# User Interface Activated " GRN "🗹 \n" RESET);

  } else if (strcmp(Command, "leave") == 0) {
    LeaveNetwork(HostNode);

  } else if (strcmp(Command, "exit") == 0) {
    ExitProgram(HostNode);
  }

  else {
    CommandNotFound("Command not found", buffer);
    return;
  }
}
