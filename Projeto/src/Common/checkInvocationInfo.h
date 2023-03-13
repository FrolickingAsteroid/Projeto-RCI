/**
 * @file checkInvocationInfo.h
 * @brief Header file containing declarations for functions related to invocation input
 * validation and error handling.
 *
 * This header file declares the UsrInvoc struct aswell as various error checking
 * functions. It also includes "utils.h", which contains declarations for additional
 * utility functions.
 */
#ifndef CHECKINVOCATIONINFO_H
#define CHECKINVOCATIONINFO_H

#include "utils.h"
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
