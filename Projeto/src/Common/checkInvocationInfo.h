//===-- checkinvocationinfo.h ---------------------------------------------===//
//
// This header file provides functions for validating user invocation parameters
// such as IP addresses and port numbers, aswell as defining the UsrInvo struct
//
//===----------------------------------------------------------------------===//
#ifndef CHECKINVOCATIONINFO_H
#define CHECKINVOCATIONINFO_H

/**
 * @brief Struct representing user invocation parameters
 */
typedef struct UsrInvoc {
  char *HostIP; /**< Host IP address */
  int HostTCP;  /**< Host TCP port number */
  char *RegIP;  /**< Registrar IP address */
  int RegUDP;   /**< Registrar UDP port number */
} UsrInvoc;

// Initializes a new UsrInvoc struct
UsrInvoc *InitUsrInfo();
// Validates user invocation parameters and returns a UsrInvoc struct
UsrInvoc *InvocCheck(int argc, char *argv[]);
// Validates the format of an IP address
char *CheckValidAdress(char *IP);
// Validates the format of a port number
int CheckValidPort(char *PORT);

#endif
