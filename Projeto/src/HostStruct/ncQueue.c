#include <stdlib.h>
#include <string.h>

#include "ncQueue.h"
#include "CirBuffer.h"

#include "../Common/utils.h"
#include "nodeStructure.h"

NewConQueue *InitNCQueue(int NewFd) {
  // Init Queue struct
  NewConQueue *NewCon = (struct NewConQueue *)malloc(sizeof(struct NewConQueue));
  if (NewCon == NULL) {
    DieWithSys("Function InitNCQueue: malloc() failed");
  }

  NewCon->Cb = (CircularBuffer *)malloc(sizeof(CircularBuffer));
  if (NewCon->Cb == NULL) {
    DieWithSys("Function InitNCQueue: malloc() failed");
  }

  CbInit(NewCon->Cb);
  NewCon->NewFd = NewFd;
  NewCon->next = NULL;

  return NewCon;
}

void PlugNC(int Fd, Host *HostNode, char *Buffer) {
  NewConQueue *NewCon = InitNCQueue(Fd);

  if (CbWrite(NewCon->Cb, Buffer, strlen(Buffer))) {
    free(NewCon->Cb), free(NewCon);
    exit(EXIT_FAILURE);
  }
  NewCon->next = HostNode->NClist;
  HostNode->NClist = NewCon;
}

void RemoveNC(Host *HostNode, int Newfd) {

  NewConQueue *Current = HostNode->NClist;
  NewConQueue *Del = NULL;

  if (Newfd == HostNode->NClist->NewFd) {
    HostNode->NClist = Current->next;
    free(Current);

  } else {
    while (Current != NULL) {
      if (Newfd == Current->next->NewFd) {
        Del = Current->next, Current->next = Del->next;
        free(Del);
        break;
      }
      Current = Current->next;
    }
  }
}

void FreeNCList(Host *HostNode) {
  NewConQueue *AuxNC = NULL;
  while (HostNode->NClist != NULL) {
    AuxNC = HostNode->NClist, HostNode->NClist = HostNode->NClist->next;
    free(AuxNC->Cb);
    free(AuxNC);
  }
  HostNode->NClist = NULL;
}
