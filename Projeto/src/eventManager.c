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

void EventManager(int fd) {
  int MaxDescriptor = -1;
  fd_set SockSet;
  int NewFd = -1, Counter = 0;
  char buffer[MAXSIZE];
  struct sockaddr addr;
  socklen_t addrlen;

  FD_ZERO(&SockSet);
  // Add Stdin to set of fileDescriptors
  FD_SET(STDIN_FILENO, &SockSet);
  FD_SET(fd, &SockSet);
  MaxDescriptor = fd; // Update Maximum number of fp

  Counter = select(MaxDescriptor, (&SockSet), (fd_set *)NULL, (fd_set *)NULL,
                   (struct timeval *)NULL);

  if (FD_ISSET(STDIN_FILENO, &SockSet)) { // Check Keyboard
    //
    memset(&buffer, 0, MAXSIZE);
    if (read(STDIN_FILENO, buffer, MAXSIZE) == -1) {
      exit(EXIT_FAILURE);

      /*! TODO: Has mem leak, needs node as arg, make DieWithSys.*/
    }
    printf("Is this working? %s", buffer);
  }
  Counter = Counter;

  if (FD_ISSET(fd, &SockSet)) {
    if ((NewFd = accept(fd, &addr, &addrlen)) == -1) {
      exit(EXIT_FAILURE);

      /*! TODO: Has mem leak, needs node as arg, make DieWithSys.
       */
    }
    if (read(fd, buffer, MAXSIZE) == -1) {
      exit(EXIT_FAILURE);

      /*! TODO: Has mem leak, needs node as arg, make DieWithSys.
       * Needs parcer for TPC requests
       */
    }
  }
}