#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "newMod.h"
#include "../Common/formatChecking.h"

#define MAXSIZE 128
void ReadListeningSock(Host *HostNode, char *Buffer, int NewFd) {
  char msg[MAXSIZE << 1] = "";
  char NewId[MAXSIZE] = "", NewIp[MAXSIZE] = "", NewTCP[MAXSIZE] = "";

  // read info from new established socket
  if (read(NewFd, Buffer, MAXSIZE) == -1) {
    perror("Function TCPClientExternConnect >> " RED "☠  recv() failed");
    return;
  }
  // fetch input args and check validity
  sscanf(Buffer, "NEW %s %s %s", NewId, NewIp, NewTCP);
  if (!(CheckNumberOfArgs(Buffer, 3) && BootArgsCheck(NewId, NewIp, NewTCP))) {
    close(NewFd);
    return;
  }

  // if host is alone in the network send back the received information: create an ancor
  if (HostNode->Ext == NULL) {
    sprintf(msg, "EXTERN %s %s %s", NewId, NewIp, NewTCP);
    // send message back to intern
    if (write(NewFd, msg, (size_t)strlen(msg)) == -1) {
      perror("Function ReadListeningSock >> " RED "☠  write() failed");
      return;
    }
    // plug extern into structure and set host as bck -> NULL
    HostNode->Ext = InitNode(NewIp, atoi(NewTCP), NewId, NewFd);

  } else {
    sprintf(msg, "EXTERN %s %s %d", HostNode->Ext->Id, HostNode->Ext->IP,
            HostNode->Ext->TCPort);
    // send message to potential extern
    if (write(NewFd, msg, (size_t)strlen(msg)) == -1) {
      perror("Function ReadListeningSock >> " RED "☠  write() failed");
      return;
    }
    // set new intern
    PlugIntern(NewIp, atoi(NewTCP), NewId, NewFd, HostNode);
  }
}
