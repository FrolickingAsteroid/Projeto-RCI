#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"

int Verbose = 0;

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

// Error Message and close -- might need tweaking
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
 * @brief Check if a string contains only alphanumeric characters.
 *
 * This function iterates through each character in the given string
 * and checks if all characters are alphanumeric. If a non-alphanumeric
 * character is found, it returns 0 (false). If all characters are
 * alphanumeric, it returns 1 (true).
 *
 * @param str: The input string to check for alphanumeric characters.
 * @return 1 if all characters in the string are alphanumeric, 0 otherwise.
 */
int IsAlphanumeric(char *str) {
  for (size_t i = 0; str[i] != '\0'; i++) {
    if (!isalnum(str[i])) {
      return 0;
    }
  }
  return 1;
}

/**
 * @brief Prints a warning message to stderr indicating that a command was incorrect.
 *
 * The function prints a warning message to the standard error output stream (stderr)
 * indicating that the specified command was incorrect. The message includes the name
 * of the command and an additional appropriate message.
 *
 * @param Command: A string containing the name of the command.
 * @param msg: A string containing an appropriate message to include in the output.
 */
void CommandNotFound(char *msg, char *Command) {

  fputs(RED, stderr);
  fputs("🚩 WARNING > ", stderr);
  fputs(RESET, stderr);
  fputs(msg, stderr);
  fputs(": ", stderr);
  fputs(Command, stderr);
}

void ServerAnswer(char *Answer, char *place) {
  int len = (int)strlen(Answer);
  if (Verbose && Answer != NULL) {
    if (Answer[len - 1] == '\n') {
      Answer[len - 1] = 0;
    }
    fprintf(stdout, GRN "\n%s" RESET, place);
    fputs(GRN ":" RESET, stdout);
    fputs("\n", stdout);
    fputs(Answer, stdout);
    fputs(GRN "\n---\n" RESET, stdout);
  }
}

/**
 * @brief Displays usage information for the user interface.
 *
 * The function prints a list of available commands and their corresponding
 * descriptions to the console.
 */
void InterfaceUsage() {

  printf(GRN "+---------------------------------------------------------------+\n");
  printf("|                       Usage Guide                             |\n");
  printf("+---------------------------+-----------------------------------+\n" RESET);
  printf("+---------------------------+-----------------------------------+\n");
  printf("| help:                     | Displays the usage guide, showing |\n");
  printf("|                           | available commands and their      |\n");
  printf("|                           | descriptions.                     |\n");
  printf("+---------------------------+-----------------------------------+\n");
  printf("| join net id:              | Registers a node with the given   |\n");
  printf("|                           | identifier to the network.        |\n");
  printf("+---------------------------+-----------------------------------+\n");
  printf("| djoin net id bootid bootIP| Adds a node with the given        |\n");
  printf("| bootTCP:                  | identifier to the network, using  |\n");
  printf("|                           | the provided boot node for        |\n");
  printf("|                           | initial connection.               |\n");
  printf("+---------------------------+-----------------------------------+\n");
  printf("| create name:              | Creates a content item with the   |\n");
  printf("|                           | given name.                       |\n");
  printf("+---------------------------+-----------------------------------+\n");
  printf("| delete name:              | Deletes the content item with     |\n");
  printf("|                           | the given name.                   |\n");
  printf("+---------------------------+-----------------------------------+\n");
  printf("| get dest name:            | Searches for the content item     |\n");
  printf("|                           | with the given name on the        |\n");
  printf("|                           | specified destination node.       |\n");
  printf("+---------------------------+-----------------------------------+\n");
  printf("| show topology (st):       | Displays the identifiers and      |\n");
  printf("|                           | contacts of the internal          |\n");
  printf("|                           | neighbors, external neighbor,     |\n");
  printf("|                           | and recovery neighbor.            |\n");
  printf("+---------------------------+-----------------------------------+\n");
  printf("| show names (sn):          | Displays the names of content     |\n");
  printf("|                           | items present on the node.        |\n");
  printf("+---------------------------+-----------------------------------+\n");
  printf("| show routing (sr):        | Displays the node's forwarding    |\n");
  printf("|                           | table.                            |\n");
  printf("+---------------------------+-----------------------------------+\n");
  printf("| leave:                    | Exits the node from the network.  |\n");
  printf("+---------------------------+-----------------------------------+\n");
  printf("| exit:                     | Closes the application.           |\n");
  printf("+---------------------------+-----------------------------------+\n");
  printf("| verbose:                  | Enables verbose mode, displaying  |\n");
  printf("|                           | additional information during     |\n");
  printf("|                           | program execution.                |\n");
  printf("+---------------------------+-----------------------------------+\n");
  printf("| quiet:                    | Enables quiet mode, suppressing   |\n");
  printf("|                           | non-critical output.              |\n");
  printf("+---------------------------+-----------------------------------+\n");
  printf("| clear:                    | Clears the terminal screen.       |\n");
  printf("+---------------------------+-----------------------------------+\n");
}
