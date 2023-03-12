#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "withdrawMod.h"
#define BUFSIZE 128

void WithdrawHandle(Host *HostNode, char *LeavingId) {
  char Id[BUFSIZE] = "";
  char BootIp[BUFSIZE] = "", BootId[BUFSIZE] = "", BootTCP[BUFSIZE] = "";

  char msg[256] = "", *TCPAnswer = NULL;
  Node *Current = HostNode->NodeList;
  Node *Del = NULL;

  // if extern withdraws evaluate
  if (strcmp(LeavingId, HostNode->Ext->Id) == 0) {
    // host has a bck
    if (HostNode->Bck != NULL) {
      FreeNode(HostNode->Ext);
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
      FreeNode(HostNode->Ext);
      HostNode->Ext = HostNode->NodeList;
      // verify if there were only two nodes on the network
      if (HostNode->NodeList != NULL) {
        HostNode->NodeList = HostNode->NodeList->next;
      }
    }
    // send EXTERN ext to all neighbours
    memset(msg, 0, sizeof(msg));
    sprintf(msg, "EXTERN %s %s %d", HostNode->Ext->Id, HostNode->Ext->IP,
            HostNode->Ext->TCPort);

    for (Node *temp = HostNode->NodeList; temp != NULL; temp = temp->next) {
      // send message
      if (write(temp->Fd, msg, (size_t)strlen(msg)) == -1) {
        // if connection is not available continue
        perror("Function WithdrawHandle >> " RED "☠  write() failed");
        continue;
      };
    }

    free(TCPAnswer);
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

void ExternHandle(char *Buffer, Host *HostNode) {
  char Id[BUFSIZE] = "", Ip[BUFSIZE] = "", TCP[BUFSIZE] = "";

  // scan bck info
  sscanf(Buffer, "EXTERN %s %s %s", Id, Ip, TCP);

  // check format
  if (!BootArgsCheck(Id, Ip, TCP)) {
    // do something
  }

  // Update bck node
  FreeNode(HostNode->Bck);
  HostNode->Bck = InitNode(Ip, atoi(TCP), Id, -1);
}
