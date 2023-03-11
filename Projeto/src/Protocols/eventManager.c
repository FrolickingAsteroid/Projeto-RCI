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
  socklen_t addrlen = sizeof(addr);

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

  MaxDescriptor = UpdateMaxDes(HostNode);
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
        perror(""); // redo later
      }
      ReadListeningSock(HostNode, buffer, NewFd);
      FD_CLR(HostNode->FdListen, &SockSet);
      continue;
    }
    // Check external node socket
    if (HostNode->Ext != NULL && FD_ISSET(HostNode->Ext->Fd, &SockSet)) {
      // do something
      FD_CLR(HostNode->Ext->Fd, &SockSet);
      continue;
    }
    // Check internal nodes sockets
    while (current != NULL) {
      if (FD_ISSET(current->Fd, &SockSet)) {
        // do something
        FD_CLR(current->Fd, &SockSet);
        break;
      }
      current = current->next;
    }
  }
}

int UpdateMaxDes(Host *HostNode) {

  // case where Host already has an extern and interns, choose between the most recent
  if (HostNode->Ext != NULL && HostNode->NodeList != NULL) {
    return max(HostNode->NodeList->Fd, HostNode->Ext->Fd);
  }
  // case where Host has no interns
  else if (HostNode->Ext != NULL && HostNode->NodeList == NULL) {
    return HostNode->Ext->Fd;
  }
  // case where Host is alone in the network
  return HostNode->FdListen;
}
/*! TODO: Colocar clausula do externo e criar função para tratamento de chamadas do
 * vizinho*/
