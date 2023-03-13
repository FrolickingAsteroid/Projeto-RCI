#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "exitMod.h"
#include "showMod.h"
#include "userInterface.h"

/**
 * @brief Parses user input command and calls processing functions.
 *
 * This function parses the user input command from the given buffer and executes the
 * corresponding function based on the command. The supported commands are "join",
 * "djoin", "clear", "leave", "exit", and "st" (...).
 *
 * @param buffer The buffer containing the user input command.
 * @param HostNode A pointer to the host node struct.
 */
void UserInterfaceParser(char buffer[], Host *HostNode) {
  char Command[64];

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

  } else if (strcmp(Command, "st") == 0) {
    ShowTopology(HostNode);

  } else {
    CommandNotFound("Command not found", buffer);
    return;
  }
}
