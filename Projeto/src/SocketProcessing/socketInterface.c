#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include "socketInterface.h"

#define MAXSIZE 128

void SocketInterfaceParser(char *Buffer, Host *HostNode) {
  char LeavingId[MAXSIZE] = "";
  char Token[64] = "";

  // Parse type of message from buffer
  sscanf(Buffer, "%s", Token);

  // Chain of if/if-else for each node message sent
  if (strcmp(Token, "WITHDRAW") == 0) {
    sscanf(Buffer, "WITHDRAW %s", LeavingId);

    // check protocol format
    if (strlen(LeavingId) != 2 || !IsNumber(LeavingId)) {
      // do something
    }
    WithdrawHandle(HostNode, LeavingId);

  } else if (strcmp(Token, "EXTERN") == 0) {
    ExternHandle(Buffer, HostNode);
  } else {
    return;
  }
}
