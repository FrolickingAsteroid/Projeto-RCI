//===-- checkinvocationinfo.h - User invocation info validation -----------===//
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

UsrInvoc *InitUsrInfo();
UsrInvoc *InvocCheck(int argc, char *argv[]);

char *CheckValidAdress(char *IP);
int CheckValidPort(char *PORT);

#endif
