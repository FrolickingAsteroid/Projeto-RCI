#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "formatChecking.h"

/**
 * @brief
 *
 * @param Net
 * @param Id
 * @return
 */
int CheckNetAndId(char *Net, char *Id) {
  // check valid number of characters
  if (strlen(Net) != 3 || strlen(Id) != 2) {
    return 0;
  }
  // check if args are numbers
  if ((IsNumber(Net) == 0) || (IsNumber(Id) == 0)) {
    return 0;
  }
  // check if args lie in the apropriate range
  if (atoi(Net) < 0 || atoi(Net) > 999 || atoi(Id) < 0 || atoi(Id) > 99) {
    return 0;
  }
  return 1;
}

int CheckNumberOfArgs(char *Buffer, int ArgNumber) {
  int count = 0;
  for (int i = 0; Buffer[i] != '\0'; i++) {
    if (Buffer[i] == ' ')
      count++;
  }

  if (count != ArgNumber) {
    return 0;
  }

  return 1;
}

int BootArgsCheck(char *BootId, char *BootIp, char *BootTCP) {
  if (IsNumber(BootId) == 0 || IsNumber(BootTCP) == 0) {
    return 0;
  }

  if (atoi(BootId) < 0 || atoi(BootId) > 99 || strlen(BootId) != 2) {
    return 0;
  }

  if (atoi(BootTCP) > 65535 || atoi(BootTCP) < 0) {
    return 0;
  }

  struct sockaddr_in sa;
  if (inet_pton(AF_INET, BootIp, &(sa.sin_addr)) != 1) {
    return 0;
  }

  return 1;
}
