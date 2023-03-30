#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "ncQueue.h"
#include "CirBuffer.h"

#include "../Common/utils.h"

#define TIMEOUT_NEW_CONNEC 300 // Timout for inactive file descriptors

/**
 * @brief Initialize a new connection queue.
 *
 * @param NewFd: The file descriptor for the new connection.
 *
 * @return A pointer to the newly created NewConQueue structure.
 */
NewConQueue *InitNCQueue(int NewFd) {
  // Init Queue struct
  NewConQueue *NewCon = (struct NewConQueue *)malloc(sizeof(struct NewConQueue));
  if (NewCon == NULL) {
    return NULL;
  }

  NewCon->Cb = (CircularBuffer *)malloc(sizeof(CircularBuffer));
  if (NewCon->Cb == NULL) {
    return NULL;
  }

  CbInit(NewCon->Cb);
  NewCon->NewFd = NewFd;
  NewCon->next = NULL;
  NewCon->ActivityTimer = TIMEOUT_NEW_CONNEC;

  return NewCon;
}

/**
 * @brief Add a new connection to the host's connection queue.
 *
 * @param Fd: The file descriptor for the new connection.
 * @param HostNode: The host node to which the new connection is added.
 * @param Buffer: The buffer containing the data to be written.
 */
void PlugNC(int Fd, Host *HostNode, char *Buffer) {
  NewConQueue *NewCon = InitNCQueue(Fd);
  if (NewCon == NULL) {
    DieWithSys("Function InitNCQueue >> " RED " malloc() failed", HostNode);
  }

  if (CbWrite(NewCon->Cb, Buffer, strlen(Buffer)) != strlen(Buffer)) {
    close(NewCon->NewFd), free(NewCon->Cb), free(NewCon);
    return;
  }
  NewCon->next = HostNode->NClist;
  HostNode->NClist = NewCon;
}

/**
 * @brief Remove a connection from the host's connection queue.
 *
 * @param HostNode The host node from which the connection is removed.
 * @param Newfd The file descriptor for the connection to be removed.
 */
void RemoveNC(Host *HostNode, int Newfd) {

  NewConQueue *Current = HostNode->NClist;
  NewConQueue *Del = NULL;

  if (Newfd == HostNode->NClist->NewFd) {
    HostNode->NClist = Current->next;
    free(Current->Cb);
    free(Current);

  } else {
    while (Current != NULL) {
      if (Newfd == Current->next->NewFd) {
        Del = Current->next, Current->next = Del->next;
        free(Del->Cb);
        free(Del);
        break;
      }
      Current = Current->next;
    }
  }
}

/**
 * @brief Free the memory allocated for the new connection queue list.
 *
 * @param HostNode The host node whose connection queue list is to be freed.
 */
void FreeNCList(Host *HostNode) {
  NewConQueue *AuxNC = NULL;
  while (HostNode->NClist != NULL) {
    AuxNC = HostNode->NClist, HostNode->NClist = HostNode->NClist->next;
    close(AuxNC->NewFd);
    free(AuxNC->Cb);
    free(AuxNC);
  }
  HostNode->NClist = NULL;
}

static void DecrementActivityTimer(Host *HostNode) {

  for (NewConQueue *Current = HostNode->NClist; Current != NULL; Current = Current->next) {
    Current->ActivityTimer--;
  }
}

/**
 * @brief Remove inactive connections from the host's connection queue.
 *
 * @param HostNode The host node to be checked for inactive connections.
 */
void CleanInactiveConnections(Host *HostNode) {
  NewConQueue *Prev = NULL;
  NewConQueue *Curr = HostNode->NClist;

  DecrementActivityTimer(HostNode);

  while (Curr != NULL) {
    if (Curr->ActivityTimer <= 0) {
      // Connection timeout exceeded
      if (Prev == NULL) {
        // If curr is the head of the list
        HostNode->NClist = Curr->next;
      } else {
        Prev->next = Curr->next;
      }

      // Close the file descriptor and free the resources
      close(Curr->NewFd);
      free(Curr->Cb);
      NewConQueue *Del = Curr;
      Curr = Curr->next;
      free(Del);
    } else {
      // Move to the next connection
      Prev = Curr;
      Curr = Curr->next;
    }
  }
}
