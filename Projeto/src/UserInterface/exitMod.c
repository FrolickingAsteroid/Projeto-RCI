#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "exitMod.h"

void LeaveNetwork(Host *HostNode) {
  char msg[16] = "";
  char *UDPAnswer = NULL;

  if (HostNode->type == DJOIN) {
    /*! TODO: send withdraw*/
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

  // send Withdraw to all neighbours
  memset(msg, 0, sizeof(msg));
  sprintf(msg, "WITHDRAW %s", HostNode->Ext->Id);

  for (Node *temp = HostNode->NodeList; temp != NULL; temp = temp->next) {
    // send message
    if (write(temp->Fd, msg, (size_t)strlen(msg)) == -1) {
      // if connection is not available continue
      perror("Function WithdrawHandle >> " RED "☠  write() failed");
      continue;
    };
  }

  if (HostNode->Ext != NULL) {
    // send Withdraw to extern
    if (write(HostNode->Ext->Fd, msg, (size_t)strlen(msg)) == -1) {
      // if connection is not available continue
      perror("Function WithdrawHandle >> " RED "☠  write() failed");
    };
    printf("Extern Fd = %d\n", HostNode->Ext->Fd);
  }

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
