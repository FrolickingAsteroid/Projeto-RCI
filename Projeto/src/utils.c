#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Usage Invocation
void Usage(char *name) {
  printf("Invocação: %s IP TCP regIP[Opcional] regUDP[Opcional]\n", name);
}

// Error Message and close -- Taken from Donahoo -- Place in Utils?
void DieWithUsr(const char *msg, const char *detail) {
  fputs(msg, stderr);
  fputs(": ", stderr);
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
