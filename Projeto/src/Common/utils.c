#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"

/**
  @brief Displays the usage of the program with the required and optional arguments.
  @param name the name of the program executable
*/
void Usage(char *name) {
  fprintf(stderr,
          BLU "Usage\t>"
              " %s IP TCP regIP[Opcional] regUDP[Opcional]\n\n" RESET,
          name);
  fprintf(stderr, BLU "IP\t> " RESET " Host's address. Must follow x.x.x.x format where x "
                      "lies between 0 and 255.\n");
  fprintf(stderr,
          BLU "TCP\t> " RESET " Host's TCP port. Must be an integer between 0 and  65535.\n");
  fprintf(stderr, BLU "regIP\t> " RESET
                      " Server's IP. Must follow the same format mentioned above for IP.\n");
  fprintf(stderr, BLU "regUDP\t> " RESET " Server's TCP port. Must follow the same "
                      "format mentioned above for TCP.\n\n");
  fprintf(stderr, BLU "Note\t> " RESET "regIP and reg UDP are optional args. that default\n"
                      "\t  to 193.136.138.142 and 59000 respectively.\n\n");
}

// Error Message and close -- might need tweeking
void DieWithUsr(const char *msg, const char *detail) {
  fputs(RED, stderr);
  fputs("[☠ ] ", stderr);
  fputs(msg, stderr);
  fputs(": ", stderr);
  fputs(RESET, stderr);
  fputs(detail, stderr);
  fputc('\n', stderr);
  exit(EXIT_FAILURE);
}

void DieWithSys(const char *msg) {
  perror(msg);
  exit(EXIT_FAILURE);
}

/**
 * @brief Checks if a string contains only numerical digits.
 *
 * The function checks whether the input string `str` contains only numerical
 * digits. It returns 1 if the string contains only digits, and 0 otherwise.
 *
 * @param str: A string to check for numerical digits.
 *
 * @return 1 if the input string contains only numerical digits, 0 otherwise.
 */
int IsNumber(char *str) {
  for (int i = 0; i < (int)strlen(str); i++) {
    if (isdigit(str[i]) == 0)
      return 0;
  }
  return 1;
}

/**
 * @brief Prints a warning message to stderr indicating that a command was not found.
 *
 * The function prints a warning message to the standard error output stream (stderr)
 * indicating that the specified command was not found. The message includes the name
 * of the command and an additional appropriate message.
 *
 * @param Command: A string containing the name of the command that was not found.
 * @param msg: A string containing an appropriate message to include in the output.
 */
void CommandNotFound(char *Command, char *msg) {

  fputs(RED, stderr);
  fputs("🚩 WARNING > ", stderr);
  fputs(RESET, stderr);
  fputs(Command, stderr);
  fputs(": ", stderr);
  fputs(msg, stderr);
}

// ----------debug-----------------//
void ServerAnswer(char *UDPAnswer) {
  int len = (int)strlen(UDPAnswer);
  if (UDPAnswer[len - 1] == '\n') {
    UDPAnswer[len - 1] = 0;
  }
  fputs(GRN "Server answer:" RESET, stdout);
  fputs("\n", stdout);
  fputs(UDPAnswer, stdout);
  fputs(GRN "\n---\n" RESET, stdout);
}
// ------------------------------ //

/**
 * @brief Displays usage information for the user interface.
 *
 * The function prints a list of available commands and their corresponding
 * descriptions to the console.
 */
void InterfaceUsage() {
  printf("%sYou can use one of the following commands:%s\n\n", GRN, RESET);
  printf("  %-40sAdds a node with the given identifier to the network. If the identifier\n",
         "join net id:");
  printf("  %-40sAdds a node with the given identifier to the network, using the "
         "provided boot node for initial connection.\n",
         "djoin net id bootid bootIP bootTCP:");
  printf("  %-40sCreates a content item with the given name.\n", "create name:");
  printf("  %-40sDeletes the content item with the given name.\n", "delete name:");
  printf("  %-40sSearches for the content item with the given name on the specified "
         "destination node.\n",
         "get dest name:");
  printf("  %-40sDisplays the identifiers and contacts of the internal neighbors, "
         "external neighbor, and recovery neighbor.\n",
         "show topology (st):");
  printf("  %-40sDisplays the names of content items present on the node.\n", "show names (sn):");
  printf("  %-40sDisplays the node's forwarding table.\n", "show routing (sr):");
  printf("  %-40sExits the node from the network.\n", "leave:");
  printf("  %-40sCloses the application.\n\n", "exit:");
}
