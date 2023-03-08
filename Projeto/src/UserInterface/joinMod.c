#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
  int FlagJoin = 1; // Tells Djoin function wheter to check its arguments or not

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
  printf("server answer:\n %s\n", UDPAnswer);

  // Wait for NODELIST, if recvfrom() returns nothing, return
  if (!CheckUDPAnswer(UDPAnswer)) {
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
  if (!CheckUDPAnswer(UDPAnswer)) {
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
  DJoinNetworkServer(DjoinMsg, HostNode, FlagJoin);
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
void DJoinNetworkServer(char buffer[], Host *HostNode, int FlagJoin) {
  char msg[BUFSIZE << 2] = "", *TCPAnswer = NULL, *UDPAnswer = NULL;
  char Net[BUFSIZE] = "", Id[BUFSIZE] = "";
  char BootIp[BUFSIZE] = "", BootId[BUFSIZE] = "", BootTCP[BUFSIZE] = "";

  printf("%s\n", buffer);
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
  if (!FlagJoin) {
    if (!(CheckNumberOfArgs(buffer, 5) && BootArgsCheck(BootId, BootIp, BootTCP) &&
          CheckNetAndId(Net, Id))) {
      CommandNotFound("Invalid argument format", buffer);
      return;
    }

    // register in the network
    UDPAnswer = UDPClient(HostNode, msg);
    if (!CheckUDPAnswer(UDPAnswer)) {
      return;
    }
    free(UDPAnswer);
  }

  // check if node is alone in the network
  if (strcmp(Id, BootId) == 0) {

    // bypass communication with extern and plug Net and Id directly
    PlugHostNetId(Net, Id, HostNode);
    return;
  }

  // create request to register in the network
  sprintf(msg, "REG %s %s %s %d", Net, Id, HostNode->InvocInfo->HostIP,
          HostNode->InvocInfo->HostTCP);

  // init extern node
  HostNode->Ext = InitNode(BootIp, atoi(BootTCP), BootId, -1);

  memset(msg, 0, strlen(msg));
  // else connect with chosen extern and ask for bck
  sprintf(msg, "NEW %s %s %d\n", Id, HostNode->InvocInfo->HostIP,
          HostNode->InvocInfo->HostTCP);
  TCPAnswer = TCPClientExternConnect(HostNode, msg, BootIp, BootTCP);

  // if connection is not established return to userInterface and free initialized
  // extern node
  if (TCPAnswer == NULL) {
    FreeNode(HostNode->Ext);
    return;
  }
  // lastly, if all is well,parse bck and add it to Host, aswell as Net and HostId
  sscanf(TCPAnswer, "EXTERN %s %s %s", BootId, BootIp, BootTCP);
  PlugHostNetId(Net, Id, HostNode);
  HostNode->Bck = InitNode(BootIp, atoi(BootTCP), BootId, -1);
}
