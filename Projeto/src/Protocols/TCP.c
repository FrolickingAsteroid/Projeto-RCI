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

#define MAXPENDING 100 // Maximum number of allowed connections

/**
 * @brief Create a TCP server listening socket and bind it to the given address and port.
 *
 * @param usr A pointer to a UsrInvoc structure containing the IP and port for the server.
 *
 * @return The file descriptor of the created TCP listener server socket.
 */
int TCPServer(UsrInvoc *usr) {
  int fd = socket(AF_INET, SOCK_STREAM, 0); // TCP socket
  if (fd == -1)
    exit(EXIT_FAILURE);

  // Construct Host Adress:
  struct sockaddr_in HostAddr;
  memset(&HostAddr, 0, sizeof(HostAddr));

  HostAddr.sin_family = AF_INET;
  if (inet_pton(AF_INET, usr->HostIP, &(HostAddr.sin_addr)) != 1) {
    free(usr);
    DieWithSys("Function TCPServer >>" RED "inet_pton() failed", NULL);
  }
  HostAddr.sin_port = htons((in_port_t)usr->HostTCP);

  if (bind(fd, (struct sockaddr *)&HostAddr, sizeof(HostAddr)) < 0) {
    free(usr);
    DieWithSys("Function TCPServer >> " RED "bind() failed", NULL);
  }

  if (listen(fd, MAXPENDING) < 0) {
    free(usr);
    DieWithSys("Function TCPServer >>" RED "listen() failed", NULL);
  }

  return fd;
}

/**
 * @brief Create a TCP client socket and connect it to the chosen external node with the given IP
 * and port.
 *
 * @param HostNode: A pointer to the Host structure containing information about the client.
 * @param msg: A message to be sent to the external server upon connection.
 * @param BootIp: The IP address of the external node.
 * @param BootTCP: The port number of the external node.
 *
 * @return The number of bytes written to the external server, or -1 if an error occurs.
 */
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
    PerrorWrapper("Function TCPClientExternConnect >>" RED "inet_pton() failed" RESET);
    return -1;
  }
  ExternAddr.sin_port = htons((in_port_t)atoi(BootTCP));

  // connect to potential extern
  if (connect(Fd, (struct sockaddr *)&ExternAddr, sizeof(ExternAddr)) == -1) {
    PerrorWrapper("Function TCPClientExternConnect >> " RED "connect() failed" RESET);
    return -1;
  }

  return CustomWrite(Fd, msg, strlen(msg));
}
