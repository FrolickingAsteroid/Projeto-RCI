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

// Error Message and close -- Taken from Donahoo -- Place in Utils?
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
  for (int i = 0; i < strlen(str); i++) {
    if (isdigit(str[i]) == 0)
      return 0;
  }
  return 1;
}

void CommandNotFound(char *Command) {
  fprintf(stderr, RED "WARNING >> %s> " RESET "Command not found: Invalid Command\n",
          Command);
}
