
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

#define FOREVER for(;;)

int main(void) {
  int fd, addrlen, newfd;
  struct sockaddr_in addr;
  int n, nw;
  char *ptr, buffer[128];

  if ((fd = socket(AF_INET,SOCK_DGRAM,0)) == -1) /*error*/ exit(1);

  memset((void*) &addr, (int) '\0', sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_addr.s = htonl(INADDR_ANY);
  addr.sin_port = htons(9000);
  if (bind(fd, (struct sockaddr*) &addr, sizeof(addr)) == -1) /*error*/ exit(1);

  if (listen(fd, 5) == -1) /*error*/ exit(1);

  ret = bind(fd, (struct sockaddr*) &addr, sizeof(addr));
  if (ret == -1) /*error*/ exit(1);

  FOREVER {
    if ((newfd = accept(fd, (struct sockaddr*) &addr, &addrlen)) == -1) /*error*/ exit(1);

    while ((n = read(newfd, buffer, 128)) != 0) {
      ptr = &buffer[0];
      while (n > 0) {
        if ((nw = write(newfd, ptr, n)) <= 0) /*error*/ exit(1);
        n -= nw;
        ptr += nw;
      }
      close(newfd);
    }
  /* close(fd); exit(0); */
  }
}
