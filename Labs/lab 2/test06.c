#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

int main(void) {
  int fd, n;
  struct sockaddr_in addr;
  /* ... */
  fd = socket(AF_INET, SOCK_STREAM, 0); // TCP socket
  if (fd == -1) /*error*/ exit(1);

  memset((void*) &addr, (int) '\0', sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_addr = /* as before, put the server IP address here */;
  addr.sin_port = htons(58000);

  n = connet(fd, (struct sockaddr*) &addr, sizeof(addr));
  if(n == -1) /*error*/ exit(1);
  /* ... */
  exit(0);
}
