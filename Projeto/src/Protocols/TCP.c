#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>

#include "TCP.h"

#include "../Common/utils.h"

#include "../SocketProcessing/socketInterface.h"

#define MAXPENDING 100
#define MAXSIZE 256

int TCPServer(UsrInvoc *usr) {
  int fd = socket(AF_INET, SOCK_STREAM, 0); // TCP socket
  if (fd == -1)
    exit(EXIT_FAILURE);

  // Construct Host Adress:
  struct sockaddr_in HostAddr;
  memset(&HostAddr, 0, sizeof(HostAddr));

  HostAddr.sin_family = AF_INET;
  if (inet_pton(AF_INET, usr->HostIP, &(HostAddr.sin_addr)) != 1) {
    DieWithSys("Function TCPServer >>" RED "☠  inet_pton() failed");
  }
  HostAddr.sin_port = htons((in_port_t)usr->HostTCP);

  if (bind(fd, (struct sockaddr *)&HostAddr, sizeof(HostAddr)) < 0) {
    DieWithSys("Function TCPServer >> " RED "☠  bind() failed");
  }

  if (listen(fd, MAXPENDING) < 0) {
    DieWithSys("Function TCPServer >>" RED "☠  listen failed");
  }

  return fd;
}

ssize_t TCPClientExternConnect(Host *HostNode, char *msg, char *BootIp, char *BootTCP) {

  int Fd = socket(AF_INET, SOCK_STREAM, 0); // TCP socket
  if (Fd == -1)
    exit(EXIT_FAILURE);

  // Plug file descriptor into new extern
  HostNode->Ext->Fd = Fd;

  // Construct Extern Adress
  struct sockaddr_in ExternAddr;
  memset(&ExternAddr, 0, sizeof(ExternAddr));

  ExternAddr.sin_family = AF_INET;
  if (inet_pton(AF_INET, BootIp, &(ExternAddr.sin_addr)) != 1) {
    perror("Function TCPClientExternConnect >> inet_pton() failed");
    return -1;
  }
  ExternAddr.sin_port = htons((in_port_t)atoi(BootTCP));

  // connect to potential extern
  if (connect(Fd, (struct sockaddr *)&ExternAddr, sizeof(ExternAddr)) == -1) {
    perror(RESET "☠  Function TCPClientExternConnect >> " RED "connect() failed");
    return -1;
  }

  return CustomWrite(Fd, msg, strlen(msg));
}
