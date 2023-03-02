#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "TCP.h"

#define MAXPENDING 100

int TCPServer(UsrInvoc *usr) {
  int fd = socket(AF_INET, SOCK_STREAM, 0); // TCP socket
  if (fd == -1)
    exit(EXIT_FAILURE);

  // Construct Host Adress
  struct sockaddr_in HostAddr;
  memset(&HostAddr, 0, sizeof(HostAddr));

  HostAddr.sin_family = AF_INET;
  if (inet_pton(AF_INET, usr->HostIP, &(HostAddr.sin_addr)) != 1) {
    free(usr);
    exit(EXIT_FAILURE);

    /*! TODO: Make DieWithSys*/
  }

  HostAddr.sin_port = htons((in_port_t)usr->HostTCP);

  if (bind(fd, (struct sockaddr *)&HostAddr, sizeof(HostAddr)) < 0) {
    free(usr);
    exit(EXIT_FAILURE);
    /*! TODO: Make DieWithSys*/
  }

  if (listen(fd, MAXPENDING) < 0) {
    free(usr);
    exit(EXIT_FAILURE);
    /*! TODO: Make DieWithSys*/
  }

  return fd;
}
