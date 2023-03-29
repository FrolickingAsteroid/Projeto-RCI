#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>

#include "UDP.h"

#include "../Common/utils.h"
#include "../Common/retry.h"

// Maximum buffer size for receiving server UDP responses
#define MAXSIZE 4096

/**
 * @brief Sends a message to a server via UDP and receives the server's response.
 *
 * @param HostNode: A pointer to the Host structure containing server connection information.
 * @param msg: A pointer to the message string to be sent to the server.
 *
 * @return A pointer to the received server response string or NULL if an error occurred.
 */
char *UDPClient(Host *HostNode, char *msg) {

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
  if (SendtoRetry(Fd, msg, strlen(msg), 0, (struct sockaddr *)&ServerAddr, sizeof(ServerAddr)) ==
      -1) {
    perror("Function UDPServer >> " RED "☠  sendto() failed");
    close(Fd);
    return NULL;
  }

  char *Buffer = calloc(MAXSIZE, sizeof(char));
  if (Buffer == NULL) {
    DieWithSys("calloc() failed");
  }

  socklen_t addrlen = sizeof(ServerAddr);
  if (retry(recvfrom, Fd, Buffer, MAXSIZE, 0, (struct sockaddr *)&ServerAddr, &addrlen) == -1) {
    perror("Function UDPServer >> " RED "☠  recvfrom() failed");
    free(Buffer);
    close(Fd);
    return NULL;
  }
  close(Fd);
  ServerAnswer(Buffer, "UDP server answer");
  return Buffer;
}
