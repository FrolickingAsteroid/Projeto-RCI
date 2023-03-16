#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "getMod.h"
#include "../Common/utils.h"

#define TOKENSIZE 128

void GetName(Host *HostNode, char *Buffer) {
  char dest[TOKENSIZE] = "";
  char name[TOKENSIZE] = "";

  if (sscanf(Buffer, "get %s %s", dest, name) < 2) {
    CommandNotFound("Command not found", Buffer);
  }
}
