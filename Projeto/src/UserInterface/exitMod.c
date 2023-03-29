#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "exitMod.h"

#include "../Common/utils.h"
#include "../Protocols/UDP.h"
#include "../HostStruct/Name.h"

/**
 * @brief This function is responsible for removing a host from a network it is currently
 * registered on. If the host is not registered in any network or if it is a DJOIN type host, it
 * simply frees the host and returns.
 *
 * @param HostNode: pointer to the Host structure representing the host to be removed from
 * the network
 */
void LeaveNetwork(Host *HostNode) {
  char msg[128] = "";
  char *UDPAnswer = NULL;

  // check need to contact server
  if (HostNode->type == DJOIN) {
    fprintf(stdout, GRN "🗹 SUCCESS > " RESET "Left network %s\n", HostNode->Net);
    LiberateHost(HostNode);
    return;
  }

  // check if Host is registered in a network
  if (HostNode->Net == NULL) {
    CommandNotFound("Unable to leave", "Host is not registered in any network");
    return;
  }

  // create and send UNREG message to server
  sprintf(msg, "UNREG %s %s", HostNode->Net, HostNode->HostId);
  UDPAnswer = UDPClient(HostNode, msg);
  if (UDPAnswer == NULL ||
      strcmp(UDPAnswer, "OKUNREG") != 0) { // if answer is NULL return with perror

    if (UDPAnswer != NULL) {
      free(UDPAnswer);
    }
    CommandNotFound("Unable to leave", "Could not get proper communication with server");
    return;
  }

  // unplug connections from node structures
  fprintf(stdout, GRN "🗹 SUCCESS > " RESET "Left network %s\n", HostNode->Net);
  LiberateHost(HostNode);
  free(UDPAnswer);
}

/**
 * @brief ExitProgram function that performs cleanup and exits the program
 *
 * @param HostNode: A pointer to the Host structure representing the host running the
 * program
 */
void ExitProgram(Host *HostNode) {
  // check wether Host needs to leave the network
  if (HostNode->Net != NULL) {
    LeaveNetwork(HostNode);
  } else {
    LiberateHost(HostNode);
  }

  // close listening socket and free Host skeleton
  close(HostNode->FdListen);
  FreeNameList(HostNode);
  free(HostNode->InvocInfo);
  free(HostNode);

  printf(BLU "# User Interface Deactivated " RED "🗹 \n" RESET);
  exit(EXIT_SUCCESS);
}
