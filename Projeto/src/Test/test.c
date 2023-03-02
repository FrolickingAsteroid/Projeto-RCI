#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
  char *miminhos = "0";
  int hasLetters = strchr(miminhos, '[a-zA-Z]') != NULL;
  if (hasLetters == 1) {
    printf("Enro\n");
    exit(EXIT_FAILURE);
  }
  int numb = atoi(miminhos);
  printf("is this right? %d\n", numb);
  return 0;
}
