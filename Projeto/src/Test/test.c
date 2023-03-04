#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main() {
  char *miminhos = "0\0";

  char Command[16], *Arg1 = NULL, *Arg2 = NULL, *Arg3 = NULL, *Arg4 = NULL,
                    *Arg5 = NULL;

  fscanf(stdin, "%s %s %s %s %s %s", Command, Arg1, Arg2, Arg3, Arg4, Arg5);
  printf("%s %s %s %s %s %s", Command, Arg1, Arg2, Arg3, Arg4, Arg5);

  for (int i = 0; i < strlen(Command); i++) {
    if (isdigit(Command[i]) == 0)
      printf("ENRRO???\n");
  }

  return EXIT_SUCCESS;
}
