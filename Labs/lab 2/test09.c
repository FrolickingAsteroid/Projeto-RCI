#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

#define FOREVER for(;;)

int main(void) {
  int fd, addrlen, ret, nread;
  struct sockaddr_in addr;
  char buffer[128];

  if ((fd = socket(AF_INET,SOCK_DGRAM,0)) == -1) /*error*/ exit(1);

  memset((void*) &addr, (int) '\0', sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_addr.s = htonl(INADDR_ANY);
  addr.sin_port = htons(9000);

  ret = bind(fd, (struct sockaddr*) &addr, sizeof(addr));
  if (ret == -1) /*error*/ exit(1);

  FOREVER {
    addrlen = sizeof(addr);
    nread = recvfrom(fd, buffer, 128, 0, (struct sockaddr*) &addr, &addrlen);
    if (nread == -1) /*error*/ exit(1);

    ret = sendto(fd, buffer, nread, 0, (struct sockaddr*) &addr, addrlen);
    if (ret == -1) /*error*/ exit(1);
  }

  //close(fd);
  //exit(0);
}
