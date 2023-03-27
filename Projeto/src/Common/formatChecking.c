#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "formatChecking.h"
#include "utils.h"

/**
 * @brief Checks if the given network and identifier are valid.
 *
 * A valid network must be a three-digit number between 0 and 999, and a valid
 * identifier must be a two-digit number between 0 and 99. The function returns
 * 1 if both arguments are valid, and 0 otherwise.
 *
 * @param net The network to check.
 * @param id The identifier to check.
 *
 * @return 1 if both arguments are valid, 0 otherwise.
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

/**
 * @brief Checks if the given boot arguments are valid.
 *
 * The function checks that the `BootId` argument is a two-digit number between 0
 * and 99, the `BootTCP` argument is a valid TCP port number between 0 and 65535,
 * and the `BootIp` argument is a valid IPv4 address. The function returns 1 if
 * all three arguments are valid, and 0 otherwise.
 *
 * @param BootId The boot identifier to check.
 * @param BootIp The boot IP address to check.
 * @param BootTCP The boot TCP port number to check.
 *
 * @return 1 if all input arguments are valid, 0 otherwise.
 **/
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

/**
 * @brief Validates the name string.
 *
 * This function checks if the provided name is valid, based on size and content.
 * The name should not exceed 100 characters, and should only contain alphanumeric characters and
 * dots.
 *
 * @param Content: The input name string.
 *
 * @return Returns 1 if the name is valid, 0 otherwise.
 */
int NameParser(char *Content) {
  // check name size
  if (strlen(Content) > 100) {
    CommandNotFound("Name is too long", Content);
    return 0;
  }
  // check name contents
  if (!IsAlphanumeric(Content)) {
    CommandNotFound("Name can only contain alphanumeric charaters and dots", Content);
    return 0;
  }

  return 1;
}
