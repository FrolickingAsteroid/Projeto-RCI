#include <arpa/inet.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#include "TCP.h"

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

char *TCPClientExternConnect(Host *HostNode, char *msg, char *BootIp, char *BootTCP) {

  // Set Timeout for Server answer
  struct timeval tv;
  tv.tv_sec = 15;
  tv.tv_usec = 0;

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
    errno = EFAULT; // set errno for inet_pton failure
    perror("Function TCPClientExternConnect >> inet_pton() failed");
    return NULL;
  }
  ExternAddr.sin_port = htons((in_port_t)atoi(BootTCP));

  // set socket connect Timeout
  if (setsockopt(Fd, SOL_SOCKET, SO_SNDTIMEO, &tv, sizeof(tv)) < 0) {
    perror("Function TCPClientExternConnect >> " RED "☠  setsockopt() failed");
    return NULL;
  }

  // connect to potential extern
  if (connect(Fd, (struct sockaddr *)&ExternAddr, sizeof(ExternAddr)) == -1) {
    perror(RESET "☠  Function TCPClientExternConnect >> " RED "connect() failed");
    return NULL;
  }

  return SendTCPMessage(Fd, msg);
}

char *SendTCPMessage(int Fd, char *msg) {
  // Set Timeout for Server answer
  struct timeval tv;
  tv.tv_sec = 15;
  tv.tv_usec = 0;

  // send message
  if (write(Fd, msg, (size_t)strlen(msg)) == -1) {
    perror("Function TCPClientExternConnect >> " RED "☠  write() failed");
    return NULL;
  };

  // Set timeout for server answer
  if (setsockopt(Fd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) < 0) {
    perror("Function TCPClientExternConnect >> " RED "☠  setsockopt() failed");
    return NULL;
  }

  char *Buffer = calloc(MAXSIZE, sizeof(char));
  // receive message
  if (recv(Fd, Buffer, MAXSIZE, 0) == -1) {
    perror("Function TCPClientExternConnect >> " RED "☠  recv() failed");
    free(Buffer);
    return NULL;
  }
  ServerAnswer(Buffer);
  return Buffer;
}
