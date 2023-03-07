#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void count(char *buffer) {
  char *token = strtok(buffer, "\n");
  printf("buffer: %s\n", buffer);
  int i = 0;
  for (char *aux = buffer; aux[i]; aux[i] == '\0' ? i++ : *aux++) {
    /* ... */
    printf("i = %d\n", i);
  }
  int j = (int)(rand() % i);
  printf("j = %d", j);
  for (; j > 0 && token != NULL; token = strtok(NULL, "\n"), j--) {
    printf("j = %d", j);
  }
  printf("i = %d\n", i);
  printf("token: %s\n", token);
}

int main() {
  char teste[] = "NODESLIST net\nID1 IP1 TCP1\nID2 IP2 TCP2\nID3 IP3 TCP3\n";
  count(teste);
  return EXIT_SUCCESS;
}
