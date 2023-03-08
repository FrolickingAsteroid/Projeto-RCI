#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "showMod.h"

void ShowTopology(Host *HostNode) {

  fprintf(stdin, GRN "External Node\n" RESET);
  if (HostNode->Ext != NULL) {
    fprintf(stdin, "%s %s %d", HostNode->Ext->Id, HostNode->Ext->IP,
            HostNode->Ext->TCPort);

  } else {
    fprintf(stdin, "None\n");
  }
}
