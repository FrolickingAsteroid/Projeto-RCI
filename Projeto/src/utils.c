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
  Usage("./cot");
  exit(EXIT_FAILURE);
}

void DieWithSys(const char *msg) {
  perror(msg);
  exit(EXIT_FAILURE);
}
