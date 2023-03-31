#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "checkInvocationInfo.h"
#include "utils.h"

#define PORT_RANGE 65535 // Maximum port number

/**
 *
 * @brief Allocates memory for a new UsrInvoc struct and initializes its fields.
 *
 * This function allocates memory for a new UsrInvoc struct and initializes its
 * fields. The IP and UDP port of the node server are hardcoded in this function as
 * requested. If memory allocation fails, the function calls DieWithSys() to
 * terminate the program.
 *
 * @return A pointer to the newly created UsrInvoc struct
 */
UsrInvoc *InitUsrInfo() {
  // Allocate memory for UsrInvoc struct
  UsrInvoc *UsrInfo = malloc(sizeof(UsrInvoc));

  // Check if allocation was successful
  if (UsrInfo == NULL) {
    DieWithSys("Function InitUsrInfo >> malloc() failed", NULL);
  }

  // Initialize fields of UsrInvoc struct
  UsrInfo->RegIP = "193.136.138.142";
  UsrInfo->RegUDP = 59000;

  // Return UsrInvoc struct
  return UsrInfo;
}

/**
 * @brief Checks if the provided IP address is valid and returns it.
 *
 * @param IP The IP address to be checked.
 * @return The validated IP address.
 */
char *CheckValidAdress(char *IP) {
  struct sockaddr_in sa;

  // Check if the provided IP is valid
  if (inet_pton(AF_INET, IP, &(sa.sin_addr)) != 1) {
    return NULL;
  }
  // Return the validated IP address
  return IP;
}

/**
 *
 * @brief Check if a given string is a valid TCP port number.
 *
 * This function checks if the input string represents a valid TCP port number
 * within the range of possible values (0 to PORT_RANGE). If the input is not a valid
 * number or the number is outside the valid range, it calls the Usage() and
 * DieWithUsr() functions to print an error message and terminate the program.
 *
 * @param PORT: The string representing the TCP port number to check.
 *
 * @return An integer value representing the TCP port number, if valid.
 */
int CheckValidPort(char *PORT) {
  if (!IsNumber(PORT)) {
    return -1;
  }

  int TCP = atoi(PORT);
  if (TCP > PORT_RANGE || TCP < 0) {
    return -1;
  }
  return TCP;
}

/**
 * @brief Parses user invocation arguments and returns a UsrInvoc structure containing
 *relevant information.
 *
 *he function receives the number of arguments and a pointer to an
 *array of argument strings. If the number of arguments is not correct, the function
 *terminates the program. Otherwise, it creates a UsrInvoc struct and initializes it with
 *default values. It then checks the validity of the IP and TCP port by calling the
 *CheckValidAddress and CheckValidPort functions, respectively.
 *
 * @param argc Number of arguments passed.
 * @param argv Array of string arguments passed.
 *
 * @return Pointer to UsrInvoc structure.
 */
UsrInvoc *InvocCheck(int argc, char *argv[]) {
  // Check number of arguments
  if (argc != 3 && argc != 5) {
    Usage(argv[0]);
    DieWithUsr("Invalid Invocation", "Wrong number of arguments");
  }
  // Initialize UsrInfo with default values
  UsrInvoc *UsrInfo = InitUsrInfo();

  // Parse and check Host IP and TCP port
  UsrInfo->HostIP = CheckValidAdress(argv[1]);
  if (UsrInfo->HostIP == NULL) {
    free(UsrInfo);

    // If the IP is not valid, print usage and exit with an error message
    Usage("./cot");
    DieWithUsr(argv[1], "Invalid IP format");
  }

  UsrInfo->HostTCP = CheckValidPort(argv[2]);
  if (UsrInfo->HostTCP == -1) {
    free(UsrInfo);

    // If the IP is not valid, print usage and exit with an error message
    Usage("./cot");
    DieWithUsr(argv[2], "Invalid IP format");
  }

  // Parse and check server IP and UDP port if provided
  if (argc == 5) {
    UsrInfo->RegIP = CheckValidAdress(argv[3]);
    if (UsrInfo->RegIP == NULL) {
      free(UsrInfo);

      // If the IP is not valid, print usage and exit with an error message
      Usage("./cot");
      DieWithUsr(argv[3], "Invalid IP format");
    }

    UsrInfo->RegUDP = CheckValidPort(argv[4]);
    if (UsrInfo->RegUDP == -1) {
      free(UsrInfo);

      // If the IP is not valid, print usage and exit with an error message
      Usage("./cot");
      DieWithUsr(argv[4], "Invalid IP format");
    }
  }

  return UsrInfo;
}
