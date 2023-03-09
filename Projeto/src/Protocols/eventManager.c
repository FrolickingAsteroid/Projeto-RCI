#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#include "eventManager.h"

#define MAXSIZE 128
#define max(A, B) ((A) >= (B) ? (A) : (B))

void EventManager(Host *HostNode) {
  int MaxDescriptor = -1, Counter = 0, NewFd = -1;
  char buffer[MAXSIZE];
  fd_set SockSet;

  struct sockaddr addr;
  socklen_t addrlen;

  Node *current = HostNode->NodeList;

  // reset file descriptors before next run
  FD_ZERO(&SockSet);

  // --- Set fileDescriptors --- //
  FD_SET(STDIN_FILENO, &SockSet);
  FD_SET(HostNode->FdListen, &SockSet);

  Node *temp = HostNode->NodeList;
  while (temp != NULL) {
    FD_SET(temp->Fd, &SockSet);
    temp = temp->next;
  }

  if (HostNode->NodeList != NULL) {
    MaxDescriptor = max(HostNode->NodeList->Fd, HostNode->FdListen);
  } else {
    MaxDescriptor = HostNode->FdListen;
  }
  /// --------------------------- //

  // start select
  Counter = select(MaxDescriptor + 1, (&SockSet), (fd_set *)NULL, (fd_set *)NULL,
                   (struct timeval *)NULL);

  if (Counter <= 0) {
    DieWithSys("Function EventManager >>" RED "☠  select() failed");
  }

  while (Counter-- > 0) {
    memset(&buffer, 0, MAXSIZE);

    // Check Keyboard
    if (FD_ISSET(STDIN_FILENO, &SockSet)) {
      if (read(STDIN_FILENO, buffer, MAXSIZE) == -1) {
        DieWithSys("Function EventManager >>" RED "☠  read() failed");
      }
      UserInterfaceParser(buffer, HostNode);
      FD_CLR(STDIN_FILENO, &SockSet);
      continue;
    }
    // Check Listening Socket
    if (FD_ISSET(HostNode->FdListen, &SockSet)) {
      if ((NewFd = accept(HostNode->FdListen, &addr, &addrlen)) == -1) {
        exit(EXIT_FAILURE); // redo later
      }
      ReadListeningSock(HostNode, buffer, NewFd);
      FD_CLR(HostNode->FdListen, &SockSet);
      continue;
    }
  }
  // Check internal nodes sockets
  while (current != NULL) {
    if (FD_ISSET(current->Fd, &SockSet)) {
      // do something
      break;
    }
    current = current->next;
  }
}

/*! TODO: Colocar clausula do externo e criar função para tratamento de chamadas do
 * vizinho*/
