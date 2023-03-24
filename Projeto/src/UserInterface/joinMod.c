#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "joinMod.h"

#include "../HostStruct/forwardingTable.h"
#include "../Protocols/TCP.h"
#include "../Protocols/UDP.h"
#include "../Common/formatChecking.h"
#include "../Common/utils.h"
#define BUFSIZE 128

/**
 * @brief Frees allocated memory for UDPAnswer and DjoinMsg if they are not NULL.
 *
 * This helper function frees the memory allocated for UDPAnswer and DjoinMsg if they
 * are not NULL. It is used to clean up resources when they are no longer needed.
 *
 * @param UDPAnswer: Pointer to the memory allocated for the UDP answer.
 * @param DjoinMsg: Pointer to the memory allocated for the Djoin message.
 */
static void CleanupResources(char *UDPAnswer, char *DjoinMsg) {
  if (UDPAnswer) {
    free(UDPAnswer);
  }
  if (DjoinMsg) {
    free(DjoinMsg);
  }
}

/**
 * @brief Validates a server response against an expected prefix.
 *
 * This helper function checks if the given server response is not NULL and if it
 * contains the expected prefix. If the response is valid, it returns 1, otherwise it
 * returns 0 and displays an error message.
 *
 * @param response: Pointer to the server response string.
 * @param ExpectedPrefix: Pointer to the expected prefix string.
 * @return 1 if the server response is valid, 0 otherwise.
 */
static int ValidateServerResponse(char *response, char *ExpectedPrefix) {
  if (response == NULL) {
    return 0;
  }
  if (strstr(response, ExpectedPrefix) != NULL) {
    return 1;
  }
  CommandNotFound("Server Answer", response);
  return 0;
}

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

  // Retrieve command args and check their validity
  sscanf(buffer, "join %s %s", Net, Id);
  if (!(CheckNetAndId(Net, Id) && CheckNumberOfArgs(buffer, 2))) {
    CommandNotFound("Invalid argument invocation, type 'help' for usage", buffer);
    return;
  }
  // Check if node is already in a network
  if (HostNode->Net != NULL) {
    CommandNotFound("Host is already registered in a network, leave before rejoining", buffer);
    return;
  }

  sprintf(msg, "NODES %s", Net);
  UDPAnswer = UDPClient(HostNode, msg);

  // if recvfrom() returns nothing or an error message, return
  if (!ValidateServerResponse(UDPAnswer, "NODESLIST")) {
    return;
  }

  CheckSingularityId(HostNode, UDPAnswer, &Id);

  // Return Djoin command for TCP connection
  DjoinMsg = ExternFetch(UDPAnswer, Net, Id);
  CleanupResources(UDPAnswer, NULL);

  // Resets msg buffer and registers in the network
  memset(msg, 0, strlen(msg));
  sprintf(msg, "REG %s %s %s %d", Net, Id, HostNode->InvocInfo->HostIP,
          HostNode->InvocInfo->HostTCP);

  UDPAnswer = UDPClient(HostNode, msg);

  if (!ValidateServerResponse(UDPAnswer, "OKREG")) {
    CleanupResources(UDPAnswer, DjoinMsg);
    return;
  }
  // Check if there are no nodes in the network, bypasses djoin function
  if (DjoinMsg == NULL) {
    CleanupResources(UDPAnswer, NULL);
    PlugHostNetId(Net, Id, HostNode);
    return;
  }

  // Free UDP server allocated answer and Djoin message before leaving function
  DJoinNetworkServer(DjoinMsg, HostNode);
  CleanupResources(UDPAnswer, DjoinMsg);
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
void CheckSingularityId(Host *HostNode, char *Nodelist, char (*Id)[BUFSIZE]) {
  char DelId[BUFSIZE] = "", Ip[16] = "";
  int Port;

  sprintf(DelId, "\n%s ", (*Id));
  char *NodePt = strstr(Nodelist, DelId);

  // check if Id is on the list
  if (NodePt == NULL) {
    return;
  }
  // if id is on the list check if port and ip are the same
  if (sscanf(NodePt, "%*s %s %d", Ip, &Port) < 2) {
    DieWithSys("sscanf failed");
  }
  // if true, return
  if (strcmp(Ip, HostNode->InvocInfo->HostIP) == 0 && Port == HostNode->InvocInfo->HostTCP) {
    return;
  }
  while (strstr(Nodelist, DelId) != NULL) {
    sprintf((*Id), "%02d", rand() % 100);
    sprintf(DelId, "\n%s ", (*Id));
  }

  fprintf(stderr, RED "🚩 WARNING >" RESET " Id already registered in the network, using Id %s\n",
          (*Id));
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
  char Net[BUFSIZE] = "", Id[BUFSIZE] = "";
  char BootIp[BUFSIZE] = "", BootId[BUFSIZE] = "", BootTCP[BUFSIZE] = "";

  // Check if node is already in a network
  if (HostNode->Net != NULL) {
    CommandNotFound("Host is already registered in a network, leave before rejoining", buffer);
    return;
  }

  // Retrieve command args
  sscanf(buffer, "djoin %s %s %s %s %s\n", Net, Id, BootId, BootIp, BootTCP);

  // Check wether args come from JoinNetworkServer(), if not, check validity and register
  // in the network
  if (HostNode->type == DJOIN) {
    if (!(CheckNumberOfArgs(buffer, 5) && BootArgsCheck(BootId, BootIp, BootTCP) &&
          CheckNetAndId(Net, Id))) {
      CommandNotFound("Invalid argument invocation, type 'help' for usage", buffer);
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
  // Connect to Extern and ask for bck
  SendNewMsg(HostNode, Id, BootIp, BootTCP);

  if (HostNode->Ext != NULL) {
    InsertInForwardingTable(HostNode, atoi(HostNode->Ext->Id), atoi(HostNode->Ext->Id));
  }
}

/**
 * @brief Send a NEW message to a remote host, update host information, and insert into the
 * forwarding table if necessary.
 *
 * This function sends a NEW message to a remote host with the specified HostId and BootIP/BootTCP
 * information. It establishes a TCP connection with the remote host and receives a response. If the
 * connection is successful and the response is parsed correctly, the function updates the host
 * information and inserts an entry into the forwarding table.
 *
 * @param HostNode: Pointer to the Host structure representing the local host node.
 * @param HostId: Pointer to a character array containing the Host ID.
 * @param BootIp: Pointer to a character array containing the boot IP address.
 * @param BootTCP: Pointer to a character array containing the boot TCP port.
 */
void SendNewMsg(Host *HostNode, char *HostId, char *BootIp, char *BootTCP) {
  char msg[BUFSIZE << 2] = "";
  char *TCPAnswer = NULL;
  char Id[BUFSIZE] = "", Ip[BUFSIZE] = "", TCP[BUFSIZE] = "";

  // Compose the NEW message to be sent to the remote host
  sprintf(msg, "NEW %s %s %d\n", HostId, HostNode->InvocInfo->HostIP, HostNode->InvocInfo->HostTCP);
  // Connect to the remote host and send the NEW message
  TCPAnswer = TCPClientExternConnect(HostNode, msg, BootIp, BootTCP);

  // If the connection is not established, free the memory and return
  if (TCPAnswer == NULL) {
    FreeNode(HostNode->Ext);
    HostNode->Ext = NULL;
    free(TCPAnswer);
    return;
  }
  /*! TODO: Parser for EXTERN*/

  // Parse the received response and extract the external host information
  sscanf(TCPAnswer, "EXTERN %s %s %s", Id, Ip, TCP);
  free(TCPAnswer);

  // If the received node is not an anchor, update the host information and insert into the
  // forwarding table
  if (strcmp(Id, HostNode->HostId) != 0) {
    HostNode->Bck = InitNode(Ip, atoi(TCP), Id, -1);
    InsertInForwardingTable(HostNode, atoi(HostNode->Ext->Id), atoi(HostNode->Bck->Id));
  }
}
