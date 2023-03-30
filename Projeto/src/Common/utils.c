#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"

#include "../HostStruct/Name.h"

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

void DieWithUsr(const char *msg, const char *detail) {
  fputs(RED, stderr);
  fputs("(X) FATAL ERROR: ", stderr);
  fputs(msg, stderr);
  fputs(": ", stderr);
  fputs(RESET, stderr);
  fputs(detail, stderr);
  fputc('\n', stderr);
  exit(EXIT_FAILURE);
}

void PerrorWrapper(const char *msg) {
  fputs(RED, stderr);
  fputs("\n(X) ERROR: ", stderr);
  fputs(RESET, stderr);
  perror(msg);
}

void DieWithSys(const char *msg, Host *HostNode) {

  PerrorWrapper(msg);

  if (HostNode != NULL) {
    LiberateHost(HostNode);
    FreeNameList(HostNode);
    free(HostNode);
  }

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
    if (!isalnum(str[i]) && str[i] != '.') {
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
  fputs("\n(!!!) WARNING > ", stderr);
  fputs(RESET, stderr);
  fputs(msg, stderr);
  fputs(": ", stderr);
  fputs(RED, stderr);
  fputs(Command, stderr);
  fputs(RESET, stderr);
}

/**
 * @brief Prints a server answer to stdout.
 *
 * @param Answer: The server answer to print.
 * @param place: The location where the answer came from.
 */
void ServerAnswer(char *Answer, char *place) {
  int len = (int)strlen(Answer);

  if (Verbose && Answer != NULL) {

    char *Buffer = strdup(Answer);
    if (Buffer == NULL) {
      DieWithSys("Function ServerAnswer >> " RED "strdup() failed", NULL);
    }
    if (Buffer[len - 1] == '\n') {
      Buffer[len - 1] = 0;
    }
    fprintf(stdout, GRN "\n%s" RESET, place);
    fputs(GRN ":" RESET, stdout);
    fputs("\n", stdout);
    fputs(Buffer, stdout);
    fputs(GRN "\n---\n" RESET, stdout);

    free(Buffer);
  }
}

void prompt() {
  printf(KMAG ">>> " RESET);
  fflush(stdout);
}

void clear_stdout_line() {
  printf("\33[2K\r");
  fflush(stdout);
}

/**
 * @brief Displays usage information for the user interface.
 *
 * The function prints a list of available commands and their corresponding
 * descriptions to the console.
 */
void InterfaceUsage() {
  printf(GRN "+-----------------------------------------------------------------------------+\n");
  printf("|                                 Usage Guide                                 |\n");
  printf("+--------------------------------+--------------------------------------------+\n" RESET);
  printf("+--------------------------------+--------------------------------------------+\n");
  printf("| help:                          | Displays the usage guide, showing          |\n");
  printf("|                                | available commands and their descriptions. |\n");
  printf("+--------------------------------+--------------------------------------------+\n");
  printf("| join net id:                   | Registers a node with the given identifier |\n");
  printf("| " GRN "join <000-999> <00-99>" RESET
         "         | to the network.                            |\n");
  printf("+--------------------------------+--------------------------------------------+\n");
  printf("| djoin net id bootid bootIP     | Adds a node with the given identifier to   |\n");
  printf("| bootTCP:                       | the network, using the provided boot node  |\n");
  printf("| " GRN "djoin <000-999> <00-99>" RESET
         "        | for initial connection.                    |\n");
  printf("| " GRN "<IPV4> <0-65535>" RESET
         "               |                                            |\n");
  printf("+--------------------------------+--------------------------------------------+\n");
  printf("| create name:                   | Creates a content item with the given name:|\n");
  printf("| " GRN "create file.txt" RESET
         "                | Must be alphanumeric and may contain '.'   |\n");
  printf("+--------------------------------+--------------------------------------------+\n");
  printf("| delete name:                   | Deletes the content item with the given    |\n");
  printf("| " GRN "delete file.txt" RESET
         "                | name: Must exits in Host.                  |\n");
  printf("+--------------------------------+--------------------------------------------+\n");
  printf("| get dest name:                 | Searches for the content item with the     |\n");
  printf("| " GRN "get <00-99> file.txt" RESET
         "           | given name on the specified destination    |\n");
  printf("|                                | node.                                      |\n");
  printf("+--------------------------------+--------------------------------------------+\n");
  printf("| show topology (st):            | Displays the identifiers and contacts of   |\n");
  printf("|                                | the internal neighbors, external neighbor, |\n");
  printf("|                                | and recovery neighbor.                     |\n");
  printf("+--------------------------------+--------------------------------------------+\n");
  printf("| show names (sn):               | Displays the names of content items        |\n");
  printf("|                                | present on the node.                       |\n");
  printf("+--------------------------------+--------------------------------------------+\n");
  printf("| show routing (sr):             | Displays the node's forwarding table.      |\n");
  printf("+--------------------------------+--------------------------------------------+\n");
  printf("| clear routing (cr):            | Clears the node's forwarding table.        |\n");
  printf("+--------------------------------+--------------------------------------------+\n");
  printf("| clear names (cn):              | Clears the names of content items present  |\n");
  printf("|                                | on the node.                               |\n");
  printf("+--------------------------------+--------------------------------------------+\n");
  printf("| leave:                         | Exits the node from the network.           |\n");
  printf("+--------------------------------+--------------------------------------------+\n");
  printf("| exit:                          | Closes the application.                    |\n");
  printf("+--------------------------------+--------------------------------------------+\n");
  printf("| verbose:                       | Enables verbose mode, displaying           |\n");
  printf("|                                | additional information during program      |\n");
  printf("|                                | execution.                                 |\n");
  printf("+--------------------------------+--------------------------------------------+\n");
  printf("| quiet:                         | Enables quiet mode, suppressing            |\n");
  printf("|                                | non-critical output.                       |\n");
  printf("+--------------------------------+--------------------------------------------+\n");
  printf("| clr:                           | Clears the terminal screen.                |\n");
  printf("+--------------------------------+--------------------------------------------+\n");
}
