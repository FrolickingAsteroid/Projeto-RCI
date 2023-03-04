#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>

#include "UDP.h"
#include "utils.h"
#define MAXSIZE 128

char *UDPClient(Host *HostNode, char *msg) {
  char *Buffer = calloc(MAXSIZE, sizeof(char));

  // Set Timeout for Server answer
  struct timeval tv;
  tv.tv_sec = 30;
  tv.tv_usec = 0;

  int Fd = socket(AF_INET, SOCK_DGRAM, 0); // UDP socket
  if (Fd == -1)
    exit(EXIT_FAILURE);

  // Construct Server Adress
  struct sockaddr_in ServerAddr;
  memset(&ServerAddr, 0, sizeof(ServerAddr));

  ServerAddr.sin_family = AF_INET;
  if (inet_pton(AF_INET, HostNode->InvocInfo->RegIP, &(ServerAddr.sin_addr)) !=
      1) {
    DieWithSys("Function TCPServer >> inet_pton() failed");
  }
  ServerAddr.sin_port = htons((in_port_t)HostNode->InvocInfo->RegUDP);

  // Send message to server
  if (sendto(Fd, msg, strlen(msg), 0, (struct sockaddr *)&ServerAddr,
             sizeof(ServerAddr)) == -1) {
    DieWithSys("Function UDPServer >> sendto() failed");
  }
  // Set timeout for server answer
  if (setsockopt(Fd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) < 0) {
    DieWithSys("Function UDPServer >> setsockopt() failed");
  }
  // Receive server answer
  socklen_t addrlen = sizeof(ServerAddr);
  if (recvfrom(Fd, Buffer, MAXSIZE, 0, (struct sockaddr *)&ServerAddr,
               &addrlen) == -1) {
    perror("Function UDPServer >> recvfrom() failed");
    close(Fd);
    return (char *)NULL;
  }
  printf("buffer: %s %d\n", Buffer, (int)sizeof(Buffer));
  close(Fd);
  return Buffer;
}
