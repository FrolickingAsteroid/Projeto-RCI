#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

int main(void) {
  int fd, n, addrlen;
  struct sockaddr_in addr;
  char buffer[128];

  /* ... *///see previous task code

  addrlen = sizeof(addr);
  n = recvfrom(fd, buffer, 128, 0, (struct sockaddr*) &addr, &addrlen);
  if (n == -1) /*error*/ exit(1);

  write(1, "echo: ",6); //stdout
  write(1,buffer,n);

  close(fd);
  exit(0);
}
