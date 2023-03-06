#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#include "eventManager.h"
#include "userInterface.h"
#include "utils.h"

#define MAXSIZE 128

void EventManager(Host *HostNode) {
  int MaxDescriptor = -1;
  fd_set SockSet;
  int NewFd = -1, Counter = 0;
  char buffer[MAXSIZE];
  struct sockaddr addr;
  socklen_t addrlen;

  FD_ZERO(&SockSet);
  // Add Stdin to set of fileDescriptors
  FD_SET(STDIN_FILENO, &SockSet);
  FD_SET(HostNode->FdListen, &SockSet);
  MaxDescriptor = HostNode->FdListen; // Update Maximum number of fp

  Counter = select(MaxDescriptor, (&SockSet), (fd_set *)NULL, (fd_set *)NULL,
                   (struct timeval *)NULL);

  if (Counter == -1) {
    DieWithSys("Function EventManager >>" RED "☠  select() failed");
  }

  while (Counter--) {
    memset(&buffer, 0, MAXSIZE);

    if (FD_ISSET(STDIN_FILENO, &SockSet)) { // Check Keyboard
      if (read(STDIN_FILENO, buffer, MAXSIZE) == -1) {
        DieWithSys("Function EventManager >>" RED "☠  read() failed");
      }
      UserInterfaceParser(buffer, HostNode);
      FD_CLR(HostNode->FdListen, &SockSet);

    }

    else if (FD_ISSET(HostNode->FdListen, &SockSet)) {
      if ((NewFd = accept(HostNode->FdListen, &addr, &addrlen)) == -1) {
        exit(EXIT_FAILURE);

        /*! TODO: needs node as arg, make DieWithSys.
         */
      }
      if (read(HostNode->FdListen, buffer, MAXSIZE) == -1) {
        exit(EXIT_FAILURE);

        /*! TODO: Has mem leak, needs node as arg, make DieWithSys.
         * Needs parcer for TPC requests
         */
      }
    }
  }
}
