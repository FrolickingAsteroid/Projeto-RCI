#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "exitMod.h"
#include "joinMod.h"
#include "../HostStruct/forwardingTable.h"

#define BUFSIZE 128

/**
 * @brief Joins a network server with the specified network and ID.
 *
 * This function retrieves command arguments and checks their validity. It then sends a
 * NODES command to the server to request a list of nodes in the network. If the list is
 * not empty, this function fetches a Djoin command and executes it. If the list is empty,
 * this function registers the host in the network.
 *
 * @param buffer: The buffer containing the command to join the network.
 * @param HostNode: The host node structure
 */
void JoinNetworkServer(char buffer[], Host *HostNode) {
  char Net[BUFSIZE] = " ", Id[BUFSIZE] = " ", msg[BUFSIZE << 2] = "";
  char *UDPAnswer = NULL, *DjoinMsg = NULL;

  // Check if node is already in a network
  if (HostNode->Net != NULL) {
    CommandNotFound("Host is already registered in a network, leave before rejoining", buffer);
    return;
  }

  // Retrieve command args and check their validity
  sscanf(buffer, "join %s %s", Net, Id);
  if (!(CheckNetAndId(Net, Id) && CheckNumberOfArgs(buffer, 2))) {
    CommandNotFound("Invalid argument format", buffer);
    return;
  }

  // Ask for NODELIST -> NODES Net
  sprintf(msg, "NODES %s", Net);
  UDPAnswer = UDPClient(HostNode, msg);

  // Wait for NODELIST, if recvfrom() returns nothing, return
  if (UDPAnswer == NULL) {
    return;
  }

  // check if Id is already in use
  CheckSingularityId(UDPAnswer, &Id);

  // Return Djoin command, NULL if list is empty
  DjoinMsg = ExternFetch(UDPAnswer, Net, Id);
  free(UDPAnswer);

  // Resets msg buffer and registers in the network
  memset(msg, 0, strlen(msg));
  sprintf(msg, "REG %s %s %s %d", Net, Id, HostNode->InvocInfo->HostIP,
          HostNode->InvocInfo->HostTCP);

  UDPAnswer = UDPClient(HostNode, msg);
  if (strcmp(UDPAnswer, "OKREG") != 0) {
    free(UDPAnswer), free(DjoinMsg); // free allocated Djoin message, can't communicate
                                     // with server
    return;
  }

  // Check if there are no nodes in the network, bypasses djoin function
  if (DjoinMsg == NULL) {
    free(UDPAnswer), PlugHostNetId(Net, Id, HostNode);
    return;
  }

  // Free UDP server allocated answer and Djoin message before leaving function
  free(UDPAnswer);
  DJoinNetworkServer(DjoinMsg, HostNode);
  free(DjoinMsg);
}

/**
 * @brief Finds a new unique ID and returns it.
 *
 * This function searches for a unique ID in a list of existing IDs
 * (`nodelist`). If the current ID (`id`) is not unique, it generates
 * a new one and checks again. This process continues until a unique
 * ID is found.
 *
 * @param nodelist: The list of registered nodes.
 * @param Id: A pointer to the ID to be checked/updated.
 */
void CheckSingularityId(char *Nodelist, char (*Id)[BUFSIZE]) {
  char DelId[BUFSIZE] = "";

  sprintf(DelId, "\n%s ", (*Id));

  if (strstr(Nodelist, DelId) == NULL) {
    return;
  }
  while (strstr(Nodelist, DelId) != NULL) {
    sprintf((*Id), "%02d", rand() % 100);
    sprintf(DelId, "\n%s ", (*Id));
  }

  printf("\x1B[31m🚩 WARNING >\x1B[0m Id already registered in the network, using Id %s\n", (*Id));
}

/**
 * @brief randomizes the process of choosing a potential extern from the list of
 * registered nodes given by the server.
 *
 * @param NODELIST list of registered nodes
 * @param buffer User input to build new Djoin command
 * @return Djoin command
 */
char *ExternFetch(char *NODELIST, char *Net, char *Id) {
  char *DjoinMsg = calloc(64, sizeof(char));
  char *array[99] = {" "};
  int i = 0;

  // Separate Nodelist into array
  array[i] = strtok(NODELIST, "\n");
  while (array[i] != NULL) {
    array[++i] = strtok(NULL, "\n");
  }
  // Return if list is empty
  if (i == 1) {
    free(DjoinMsg);
    return NULL;
  }
  // choose a node from the array as extern and make Djoin command
  char *Extern = array[(rand() % (i - 1)) + 1];
  sprintf(DjoinMsg, "djoin %s %s %s", Net, Id, Extern);
  return DjoinMsg;
}

/**
 * @brief Joins the network as a new host without registering in the server.
 *
 * This function allows a new host to join an existing network bypassing the server. It checks the
 * validity of the arguments, retrieves the network and host IDs, and connects to an external host
 * to request a backup of the network information.
 *
 * @param buffer: A buffer containing djoin command and arguments.
 * @param HostNode: A pointer to the HostNode structure.
 */
void DJoinNetworkServer(char buffer[], Host *HostNode) {
  char msg[BUFSIZE << 2] = "";
  char *TCPAnswer = NULL;
  char Net[BUFSIZE] = "", Id[BUFSIZE] = "";
  char BootIp[BUFSIZE] = "", BootId[BUFSIZE] = "", BootTCP[BUFSIZE] = "";

  // Check if node is already in a network
  if (HostNode->Net != NULL) {
    CommandNotFound("Host is already registered in a network, leave before rejoining", buffer);
    return;
  }

  // Retrieve command args
  sscanf(buffer, "djoin %s %s %s %s %s", Net, Id, BootId, BootIp, BootTCP);

  // Check wether args come from JoinNetworkServer(), if not, check validity and register
  // in the network
  if (HostNode->type == DJOIN) {
    if (!(CheckNumberOfArgs(buffer, 5) && BootArgsCheck(BootId, BootIp, BootTCP) &&
          CheckNetAndId(Net, Id))) {
      CommandNotFound("Invalid argument format", buffer);
      return;
    }
  }
  // plug Net and Id directly
  PlugHostNetId(Net, Id, HostNode);

  // check if node is alone in the network
  if (strcmp(Id, BootId) == 0) {
    return;
  }

  // init extern node
  HostNode->Ext = InitNode(BootIp, atoi(BootTCP), BootId, -1);

  // else connect with chosen extern and ask for bck
  sprintf(msg, "NEW %s %s %d\n", Id, HostNode->InvocInfo->HostIP, HostNode->InvocInfo->HostTCP);
  TCPAnswer = TCPClientExternConnect(HostNode, msg, BootIp, BootTCP);

  // if connection is not established return to userInterface and free initialized
  // extern node
  if (TCPAnswer == NULL) {
    free(TCPAnswer), LeaveNetwork(HostNode);
    return;
  }

  // lastly, if all is well,parse bck and add it to Host, aswell as Net and HostId
  sscanf(TCPAnswer, "EXTERN %s %s %s", BootId, BootIp, BootTCP);
  free(TCPAnswer);

  // if node is not an ancor, plug new back into host
  if (strcmp(BootId, HostNode->HostId) != 0) {
    HostNode->Bck = InitNode(BootIp, atoi(BootTCP), BootId, -1);
    InsertInForwardingTable(HostNode, atoi(HostNode->Ext->Id), atoi(HostNode->Bck->Id));
  }
  InsertInForwardingTable(HostNode, atoi(HostNode->Ext->Id), atoi(HostNode->Ext->Id));
}
