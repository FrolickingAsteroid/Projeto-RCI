#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    CommandNotFound(buffer);
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
  char Net[8] = " ", Id[8] = " ", msg[64] = "";
  char *UDPAnswer = NULL, *DjoinMsg = NULL;
  int FlagJoin = 1; // Tells Djoin function wheter to test its arguments or not

  // Retrieve command args and check their validity
  sscanf(buffer, "join %s %s", Net, Id);
  if ((IsNumber(Net) == 0) || (IsNumber(Id) == 0)) {
    CommandNotFound(buffer);
    /*! TODO: InterfaceUsage*/
    return;
  }

  // Check if node is in a network
  if (HostNode->NET != NULL) {
    fprintf(stderr,
            RED "WARNING >> " RESET
                "Host is already registered in the network %s with ID %s\n",
            HostNode->NET, HostNode->HostID);
    return;
  }

  // Ask for NODELIST -> NODES NET
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

  HostNode->NET = calloc(8, sizeof(char));
  strcpy(HostNode->NET, Net);

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

  // Discard Nodelist NET <LF>
  strtok(NODELIST, "\n");

  // Separate NODELIST into array
  for (i = 0; array[i] != NULL; i++) {
    array[i] = strtok(NULL, "\n");

    // Return NULL if List is emply
    if (i == 0 && array[i] == NULL) {
      return (char *)NULL;
    }
  }

  // choose a node from the array as extern and make Djoin command
  char *Extern = array[rand() % i];
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
  char Net[4] = "", Id[3] = "", msg[64] = "";
  char BootIP[16] = "", BootId[3] = "", BootTCP[4] = "";

  // Check wether args come from JoinNetworkServer()
  if (!FlagJoin) {

    // Retrieve command args and check their validity
    if (sscanf(buffer, "djoin %s %s %s %s %s", Net, Id, BootId, BootIP, BootTCP) < 5) {
      perror("Function DJoinNetworkServer >> " RED "☠  sscanf() failed");
    }
    if ((IsNumber(Net) == 0) || (IsNumber(Id) == 0) || (IsNumber(BootId) == 0)) {
      CommandNotFound(buffer);
      /*! TODO: InterfaceUsage and IP PORT parser*/
      return;
    }
    // Check if node is in a network
    if (HostNode->NET != NULL) {
      fprintf(stderr,
              RED "WARNING >>" RESET
                  "Host is already registered in the network %s with ID %s",
              HostNode->NET, HostNode->HostID);
      return;
    }
  }

  sprintf(msg, "NEW %s %s %d\n", Id, HostNode->InvocInfo->HostIP,
          HostNode->InvocInfo->HostTCP);
}
