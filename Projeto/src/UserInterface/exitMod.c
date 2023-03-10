#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "exitMod.h"

void LeaveNetwork(Host *HostNode) {
  char msg[16] = "";
  char *UDPAnswer = NULL;

  if (HostNode->type == DJOIN) {
    LiberateHost(HostNode);
    return;
  }

  // check if Host is registered in a network
  if (HostNode->Net == NULL) {
    printf("\x1B[31m🚩 WARNING >\x1B[0m Host is not registered in any a network\n");
    return;
  }

  // create and send UNREG message to server
  sprintf(msg, "UNREG %s %s", HostNode->Net, HostNode->HostId);
  UDPAnswer = UDPClient(HostNode, msg);
  if (strcmp(UDPAnswer, "OKUNREG") != 0) { // if answer is NULL return with perror
    free(UDPAnswer);
    printf("\x1B[31m🚩 WARNING >\x1B[0m Unable to unregister from network %s\n",
           HostNode->Net);
    return;
  }

  // send withdraw message to all internal and external nodes

  // unplug connections from node structures
  fprintf(stdout, GRN "🗹 SUCCESS > " RESET "Unregistered from network %s with success\n",
          HostNode->Net);
  LiberateHost(HostNode);
  free(UDPAnswer);
}

void ExitProgram(Host *HostNode) {
  // check wether Host needs to leave the network
  if (HostNode->Net != NULL) {
    LeaveNetwork(HostNode);
  } else {
    LiberateHost(HostNode);
  }

  // check if LeaveNetwork was unsuccessful
  if (HostNode->Ext != NULL) {
    LiberateHost(HostNode);
  }

  // close listening socket and free Host skeleton
  close(HostNode->FdListen);
  free(HostNode->InvocInfo);
  free(HostNode);

  printf(BLU "# User Interface Deactivated " RED "🗹 \n" RESET);
  exit(EXIT_SUCCESS);
}
