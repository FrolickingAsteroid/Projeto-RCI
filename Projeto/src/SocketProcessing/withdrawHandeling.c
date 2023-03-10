#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "withdrawHandeling.h"
#define BUFSIZE 128

void WithdrawHandel(Host *HostNode, char *LeavingId) {
  char Id[BUFSIZE] = "";
  char BootIp[BUFSIZE] = "", BootId[BUFSIZE] = "", BootTCP[BUFSIZE] = "";

  char msg[256] = "", *TCPAnswer = NULL;
  Node *Current = HostNode->NodeList;
  Node *Del = NULL;

  // if bck withdraws remove it from host
  if (strcmp(LeavingId, HostNode->Bck->Id) == 0) {
    FreeNode(HostNode->Bck);
    HostNode->Bck = NULL;
    // SendTCPMessage()
  }
  // if extern withdraws evaluate
  else if (strcmp(LeavingId, HostNode->Ext->Id) == 0) {
    // host has a bck
    if (HostNode->Bck != NULL) {
      HostNode->Ext = HostNode->Bck;
      HostNode->Bck = NULL;

      // connect with new extern and ask for bck
      sprintf(msg, "NEW %s %s %d\n", Id, HostNode->InvocInfo->HostIP,
              HostNode->InvocInfo->HostTCP);
      TCPAnswer = TCPClientExternConnect(HostNode, msg, BootIp, BootTCP);

      // if connection is not established
      if (TCPAnswer == NULL) {
        // do something -> ask
      }
      sscanf(TCPAnswer, "EXTERN %s %s %s", BootId, BootIp, BootTCP);

      // if node is not an ancor, plug new bck into host
      if (strcmp(BootId, HostNode->HostId) != 0) {
        HostNode->Bck = InitNode(BootIp, atoi(BootTCP), BootId, -1);
      }
    } else {
      HostNode->Ext = HostNode->NodeList;
      HostNode->NodeList = HostNode->NodeList->next;
    }

  }
  // if intern withdraws remove it from the list
  // first in the list
  else if (strcmp(LeavingId, HostNode->NodeList->Id) == 0) {
    HostNode->NodeList = Current->next;
    FreeNode(Current);

    // any other place
  } else {

    while (Current != NULL) {
      if (strcmp(LeavingId, Current->next->Id) == 0) {
        Del = Current->next;
        Current->next = Del->next;
        FreeNode(Del);
        break;
      }
      Current = Current->next;
    }
  }
}
