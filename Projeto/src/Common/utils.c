#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"

// Usage Invocation
void Usage(char *name) {
  fprintf(stderr,
          BLU "Usage\t>"
              " %s IP TCP regIP[Opcional] regUDP[Opcional]\n\n" RESET,
          name);
  fprintf(stderr,
          BLU "IP\t> " RESET " Host's address. Must follow x.x.x.x format where x "
              "lies between 0 and 255.\n");
  fprintf(stderr, BLU "TCP\t> " RESET
                      " Host's TCP port. Must be an integer between 0 and  65535.\n");
  fprintf(stderr,
          BLU "regIP\t> " RESET
              " Server's IP. Must follow the same format mentioned above for IP.\n");
  fprintf(stderr, BLU "regUDP\t> " RESET " Server's TCP port. Must follow the same "
                      "format mentioned above for TCP.\n\n");
  fprintf(stderr,
          BLU "Note\t> " RESET "regIP and reg UDP are optional args. that default\n"
              "\t  to 193.136.138.142 and 59000 respectively.\n\n");
}

// Error Message and close
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

// Checks wether string has alphanumeric characthers
int IsNumber(char *str) {
  for (int i = 0; i < (int)strlen(str); i++) {
    if (isdigit(str[i]) == 0)
      return 0;
  }
  return 1;
}

void CommandNotFound(char *Command, char *msg) {

  fputs(RED, stderr);
  fputs("🚩 WARNING > ", stderr);
  fputs(RESET, stderr);
  fputs(Command, stderr);
  fputs(": ", stderr);
  fputs(msg, stderr);
  fputs(GRN
        "\nYou can use one of the following commands:\n" RESET
        "  join net id: \tAdds a node with the given identifier to the network. "
        "If the identifier is already in use, the application will choose a unique "
        "identifier and notify the user.\n"
        "  djoin net id bootid bootIP bootTCP: \tAdds a node with the given identifier "
        "to the network, using the provided boot node for initial connection.\n"
        "  create name: \tCreates a content item with the given name.\n"
        "  delete name: \tDeletes the content item with the given name.\n"
        "  get dest name: \tSearches for the content item with the given name on the "
        "specified destination node.\n"
        "  show topology (st): \tDisplays the identifiers and contacts of the internal "
        "neighbors, external neighbor, and recovery neighbor.\n"
        "  show names (sn): \tDisplays the names of content items present on the node.\n"
        "  show routing (sr): \tDisplays the node's forwarding table.\n"
        "  leave: \tExits the node from the network.\n"
        "  exit: \tCloses the application. \n",
        stderr);
}

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
