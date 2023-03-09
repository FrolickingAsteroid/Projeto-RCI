#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "exitMod.h"
#include "joinMod.h"

#define BUFSIZE 128

/**
 * @brief Main function for join command. Parses user input and checks it's validity;
 * establishes an UDP connection with the node server; registers in the network and
 * proceeds to plug the chosen extern in the hosts structure (through DJoinNetworkServer
 * function).
 *
 * @param buffer User input to be parsed.
 * @param HostNode Host's information node, to complete connection..
 */
void JoinNetworkServer(char buffer[], Host *HostNode) {
  char Net[BUFSIZE] = " ", Id[BUFSIZE] = " ", msg[BUFSIZE << 2] = "";
  char *UDPAnswer = NULL, *DjoinMsg = NULL;

  // Check if node is already in a network
  if (HostNode->Net != NULL) {
    CommandNotFound("Host is already registered in a network, leave before rejoining",
                    buffer);
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

  // Return Djoin command, NULL if list is empty
  DjoinMsg = ExternFetch(UDPAnswer, buffer);
  free(UDPAnswer);

  // Resets msg buffer and registers in the network
  memset(msg, 0, strlen(msg));
  sprintf(msg, "REG %s %s %s %d", Net, Id, HostNode->InvocInfo->HostIP,
          HostNode->InvocInfo->HostTCP);

  UDPAnswer = UDPClient(HostNode, msg);
  if (strcmp(UDPAnswer, "OKREG") != 0) {
    free(DjoinMsg); // free allocated Djoin message, can't communicate with
                    // server
    return;
  }

  // Check if there are no nodes in the network, bypasses djoin function
  if (DjoinMsg == NULL) {
    free(UDPAnswer);
    PlugHostNetId(Net, Id, HostNode);
    return;
  }

  // Free UDP server allocated answer and Djoin message before leaving function
  free(UDPAnswer);
  DJoinNetworkServer(DjoinMsg, HostNode);
  free(DjoinMsg);
}

/**
 * @brief randomizes the process of choosing a potential extern from the list of
 * registered nodes given by the server.
 *
 * @param NODELIST list of registered nodes
 * @param buffer User input to build new Djoin command
 * @return Djoin command
 */
char *ExternFetch(char *NODELIST, char *buffer) {
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
  // remove \n from end of line
  buffer[strlen(buffer) - 1] = '\0';
  sprintf(DjoinMsg, "d%s %s", buffer, Extern);
  return DjoinMsg;
}

/**
 * @brief
 *
 * @param buffer
 * @param HostNode
 * @param FlagJoin
 */
void DJoinNetworkServer(char buffer[], Host *HostNode) {
  char msg[BUFSIZE << 2] = "", *TCPAnswer = NULL;
  char Net[BUFSIZE] = "", Id[BUFSIZE] = "";
  char BootIp[BUFSIZE] = "", BootId[BUFSIZE] = "", BootTCP[BUFSIZE] = "";

  // Check if node is already in a network
  if (HostNode->Net != NULL) {
    CommandNotFound("Host is already registered in a network, leave before rejoining",
                    buffer);
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
  sprintf(msg, "NEW %s %s %d\n", Id, HostNode->InvocInfo->HostIP,
          HostNode->InvocInfo->HostTCP);
  TCPAnswer = TCPClientExternConnect(HostNode, msg, BootIp, BootTCP);

  // if connection is not established return to userInterface and free initialized
  // extern node
  if (TCPAnswer == NULL) {
    LeaveNetwork(HostNode);
    return;
  }

  // lastly, if all is well,parse bck and add it to Host, aswell as Net and HostId
  sscanf(TCPAnswer, "EXTERN %s %s %s", BootId, BootIp, BootTCP);
  HostNode->Bck = InitNode(BootIp, atoi(BootTCP), BootId, -1);
}
