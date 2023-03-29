#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "userInterface.h"
#include "joinMod.h"
#include "exitMod.h"
#include "showMod.h"
#include "getMod.h"

#include "../Common/utils.h"

#include "../HostStruct/forwardingTable.h"
#include "../HostStruct/Name.h"

/**
 * @brief Clears routing or names information based on the given sub-command.
 *
 * This function clears either the forwarding table or the names list of the host
 * based on the provided sub-command in the buffer. If the sub-command is not
 * recognized, an error message is displayed.
 *
 * @param HostNode: A pointer to the host whose information should be cleared.
 * @param Buffer: A string containing the sub-command (either "routing" or "names").
 */
static void ClrParser(Host *HostNode, char *Buffer) {
  char SubCommand[128] = "";
  // Extract the sub-command from the buffer
  if (sscanf(Buffer, "clear %s\n", SubCommand) < 1) {
    CommandNotFound("Command not found", Buffer);
    return;
  }
  // Call the appropriate function based on the sub-command
  if (strcmp(SubCommand, "routing") == 0) {
    ClearForwardingTable(HostNode);

  } else if (strcmp(SubCommand, "names") == 0) {
    FreeNameList(HostNode);

  } else {
    // If the sub-command is not recognized, display an error message
    CommandNotFound("Command not found", Buffer);
  }
}

/**
 * @brief Parse and execute the "show" command, displaying information about the network, routing
 * table or name list.
 *
 * This function parses the "show" command with its sub-command and calls the appropriate function
 * to display the requested information. It handles "show topology", "show routing" and "show names"
 * sub-commands.
 *
 * @param HostNode: Pointer to the Host structure representing the local host node.
 * @param Buffer: Pointer to a character array containing the command and its sub-command.
 */
static void ShowParser(Host *HostNode, char *Buffer) {
  char SubCommand[128] = "";
  // Extract the sub-command from the buffer
  if (sscanf(Buffer, "show %s\n", SubCommand) < 1) {
    CommandNotFound("Command not found", Buffer);
    return;
  }
  // Call the appropriate function based on the sub-command
  if (strcmp(SubCommand, "topology") == 0) {
    ShowTopology(HostNode);

  } else if (strcmp(SubCommand, "routing") == 0) {
    ShowForwardingTable(HostNode);

  } else if (strcmp(SubCommand, "names") == 0) {
    ShowNames(HostNode);
  } else {
    // If the sub-command is not recognized, display an error message
    CommandNotFound("Command not found", Buffer);
  }
}

/**
 * @brief Parses user input command and calls processing functions.
 *
 * This function parses the user input command from the given buffer and executes the
 * corresponding function based on the command. The supported commands are "join",
 * "djoin", "clear", "leave", "exit","show", "st", "sr", "sn", "create", "delete", "get" and "help".
 *
 * @param buffer The buffer containing the user input command.
 * @param HostNode A pointer to the host node struct.
 */
void UserInterfaceParser(char *buffer, Host *HostNode) {
  char Command[128];

  // Parse Command from buffer
  if (sscanf(buffer, "%s", Command) < 1) {
    CommandNotFound("Command not found", " ");
    return;
  }

  // Chain of if/if-else for each command sent
  if (strcmp(Command, "join") == 0) {
    HostNode->type = JOIN; // place flag before entering join function
    JoinNetworkServer(buffer, HostNode);

  } else if (strcmp(Command, "djoin") == 0) {
    HostNode->type = DJOIN; // place flag before entering join function
    DJoinNetworkServer(buffer, HostNode);

  } else if (strcmp(Command, "clr") == 0) {
    clear();
    printf(BLU "# User Interface Activated " GRN "🗹 \n" RESET);

  } else if (strcmp(Command, "clear") == 0) {
    ClrParser(HostNode, buffer);

  } else if (strcmp(Command, "cr") == 0) {
    ClearForwardingTable(HostNode);

  } else if (strcmp(Command, "cn") == 0) {
    FreeNameList(HostNode);

  } else if (strcmp(Command, "leave") == 0) {
    LeaveNetwork(HostNode);

  } else if (strcmp(Command, "exit") == 0) {
    ExitProgram(HostNode);

  } else if (strcmp(Command, "st") == 0) {
    ShowTopology(HostNode);

  } else if (strcmp(Command, "sr") == 0) {
    ShowForwardingTable(HostNode);

  } else if (strcmp(Command, "sn") == 0) {
    ShowNames(HostNode);

  } else if (strcmp(Command, "show") == 0) {
    ShowParser(HostNode, buffer);

  } else if (strcmp(Command, "get") == 0) {
    GetName(HostNode, buffer);

  } else if (strcmp(Command, "create") == 0) {
    CreateName(HostNode, buffer);

  } else if (strcmp(Command, "delete") == 0) {
    DeleteName(HostNode, buffer);

  } else if (strcmp(Command, "help") == 0) {
    InterfaceUsage();

  } else if (strcmp(Command, "verbose") == 0) {
    Verbose = 1;

  } else if (strcmp(Command, "quiet") == 0) {
    Verbose = 0;

  } else {
    CommandNotFound("Command not found", buffer);
    return;
  }
}
