#include <stdio.h>
#include <stdlib.h>

#include "nodeStructure.h"

Host *InitHostStructure(int Fd, UsrInvoc *UsrInfo) {
  Host *node = calloc(1, sizeof(Host));

  node->HostID = NULL;
  node->Bck = NULL;
  node->Ext = NULL;
  node->FdListen = Fd;
  node->NodeList = NULL;
  node->InvocInfo = UsrInfo;
  node->NET = NULL;
  return node;
}
