#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>

#include "UDP.h"

#define MAXSIZE 256

char *UDPClient(Host *HostNode, char *msg) {

  // Set Timeout for Server answer
  struct timeval tv;
  tv.tv_sec = 5;
  tv.tv_usec = 0;

  int Fd = socket(AF_INET, SOCK_DGRAM, 0); // UDP socket
  if (Fd == -1)
    exit(EXIT_FAILURE);

  // Construct Server Adress
  struct sockaddr_in ServerAddr;
  memset(&ServerAddr, 0, sizeof(ServerAddr));

  ServerAddr.sin_family = AF_INET;
  if (inet_pton(AF_INET, HostNode->InvocInfo->RegIP, &(ServerAddr.sin_addr)) != 1) {
    DieWithSys("Function TCPServer >> inet_pton() failed");
  }
  ServerAddr.sin_port = htons((in_port_t)HostNode->InvocInfo->RegUDP);

  // Send message to server
  if (sendto(Fd, msg, strlen(msg), 0, (struct sockaddr *)&ServerAddr,
             sizeof(ServerAddr)) == -1) {
    perror("Function UDPServer >> " RED "☠  sendto() failed");
    close(Fd);
    return NULL;
  }

  // Set timeout for server answer
  if (setsockopt(Fd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) < 0) {
    perror("Function UDPServer >> " RED "☠  setsockopt() failed");
    close(Fd);
    return NULL;
  }

  char *Buffer = calloc(MAXSIZE, sizeof(char));
  // Receive server answer:
  socklen_t addrlen = sizeof(ServerAddr);
  if (recvfrom(Fd, Buffer, MAXSIZE, 0, (struct sockaddr *)&ServerAddr, &addrlen) == -1) {
    perror("Function UDPServer >> " RED "☠  recvfrom() failed");
    free(Buffer);
    close(Fd);
    return NULL;
  }
  close(Fd);
  return Buffer;
}

int CheckUDPAnswer(char *UDPAnswer) {
  if (UDPAnswer == NULL) {
    return 0;
  }
  return 1;
}
