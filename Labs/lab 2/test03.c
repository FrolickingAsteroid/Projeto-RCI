#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

int main(void) {
  int fd, n;
  struct sockaddr_in addr;
  /* ... */
  fd = socket(AF_INET, SOCK_DGRAM, 0); // UDP socket
  if (fd == -1)                        /*error*/
    exit(1);

  memset((void *)&addr, (int)'\0', sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_addr = /* put the server IP address here; use gethostbyname */;
  addr.sin_port = htons(58000);

  n = sendto(fd, "Hello!\n", 7, 0, (struct sockaddr *)&addr, sizeof(addr));
  if (n == -1) /*error*/
    exit(1);
  /* ... */
  exit(0);
}
