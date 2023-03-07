#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "TCP.h"
#include "UDP.h"
#include "checkInvocationInfo.h"
#include "nodeStructure.h"
#include "userInterface.h"
#include "utils.h"

/**
 * @brief Parses User Input Command and calls processing functions
 *
 * @param buffer -> user string written to STDIN_NO
 */
void UserInterfaceParser(char buffer[], Host *HostNode) {
  char Command[6];
  int FlagJoin = 0;

  // Parse Command from buffer
  sscanf(buffer, "%s", Command);

  // Chain of if/if-else for each command sent
  if (strcmp(Command, "join") == 0) {
    JoinNetworkServer(buffer, HostNode);

  } else if (strcmp(Command, "djoin") == 0) {
    DJoinNetworkServer(buffer, HostNode, FlagJoin);

  } else if (strcmp(Command, "clear") == 0) {
    clear();
    printf(BLU "# User Interface Activated " GRN "🗹 \n" RESET);
  }

  else {
    CommandNotFound("Command not found", buffer);
    return;
  }
}

/**
 * @brief Joins network server through an UDP connection, parses paramaters
 * given by the user and checks their validity
 *
 * @param buffer
 * @param HostNode
 */
void JoinNetworkServer(char buffer[], Host *HostNode) {
  char Net[128] = " ", Id[128] = " ", msg[128] = "";
  char *UDPAnswer = NULL, *DjoinMsg = NULL;
  int FlagJoin = 1; // Tells Djoin function wheter to test its arguments or not

  // Retrieve command args and check their validity
  sscanf(buffer, "join %s %s", Net, Id);
  if ((IsNumber(Net) == 0) || (IsNumber(Id) == 0)) {
    CommandNotFound("Invalid argument format", buffer);
    return;
  }

  // Check if node is in a network
  if (HostNode->Net != NULL) {
    CommandNotFound("Host is already registered in a network leave before joining again.",
                    "\0");
    return;
  }

  // Ask for NODELIST -> NODES Net
  sprintf(msg, "NODES %s", Net);
  UDPAnswer = UDPClient(HostNode, msg);
  printf("server answer: %s\n", UDPAnswer);

  // Wait for NODELIST, if recvfrom() returns nothing, return
  if (!CheckUDPAnswer(UDPAnswer)) {
    return;
  }

  // Return Djoin command, NULL if list is empty
  DjoinMsg = ExternFetch(UDPAnswer, HostNode, buffer);

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

  HostNode->Net = calloc(8, sizeof(char));
  strcpy(HostNode->Net, Net);

  // Check if there are no nodes in the network, bypasses djoin function
  if (DjoinMsg == NULL) {
    free(DjoinMsg);
    /*! TODO: Add Node variables to HostNode if lonely*/
    return;
  }

  // Free UDP server allocated answer and Djoin message before leaving function
  free(UDPAnswer);
  // DJoinNetworkServer(DjoinMsg, HostNode, FlagJoin);
  FlagJoin = FlagJoin;
  free(DjoinMsg);
}

/**
 * @brief
 *
 * @param NODELIST
 * @param HostNode
 * @param buffer
 * @return
 */
char *ExternFetch(char *NODELIST, Host *HostNode, char *buffer) {
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
    return NULL;
  }
  // choose a node from the array as extern and make Djoin command
  char *Extern = array[(rand() % i) + 1];
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
  char msg[128] = "", *TCPAnswer = NULL;
  char Net[128] = "", Id[128] = "";
  char BootIp[128] = "", BootId[128] = "", BootTCP[128] = "";

  // Retrieve command args
  if (sscanf(buffer, "djoin %s %s %s %s %s", Net, Id, BootId, BootIp, BootTCP) < 5) {
    /*! TODO: CommandNotFound*/
  }

  // Check wether args come from JoinNetworkServer(), if not, check validity
  if (!FlagJoin) {
    /*! TODO: ValCheck()*/
  }

  // check if node is alone in the network
  if (strcmp(Id, BootIp) == 0) {
    // bypass communication with externa nd plug Net and Id directly
    PlugHostNetId(Net, Id, HostNode);
    return;
  }

  // init extern node
  HostNode->Ext = InitNode(BootIp, atoi(BootTCP), BootId, -1);

  // else connect with chosen extern and ask for bck
  sprintf(msg, "NEW %s %s %d\n", Id, HostNode->InvocInfo->HostIP,
          HostNode->InvocInfo->HostTCP);
  TCPAnswer = TCPClientExternConnect(HostNode, msg, BootIp, BootId, BootTCP);

  // if connection is not established return to userInterface and free initialized extern
  // node
  if (TCPAnswer == NULL) {
    /*! TODO: freeNode() */
    return;
  }
  // lastly, if all is well,parse bck and add it to Host, aswell as Net and HostId
  sscanf(TCPAnswer, "EXTERN %s %s %s", BootId, BootIp, BootTCP);
  PlugHostNetId(Net, Id, HostNode);
  HostNode->Bck = InitNode(BootIp, atoi(BootTCP), BootId, -1);
}
