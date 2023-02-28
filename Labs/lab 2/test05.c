#include <stdlib.h>
#include <netdb.h>
#include <sys/socket.h>
/* ... */

int main(void) {
  int fd, n, addrlen;
  struct sockaddr_in addr;
  char buffer[128];

  /* ... */// see previous task code

  addrlen = sizeof(addr);
  n = recvfrom(fd, buffer, 128, 0, (struct sockaddr*) &addr, &addrlen);
  if (n == -1) /*error*/ exit(1);
  /* ... */

  h = gethostbyaddr(&addr.sin_addr, sizeof(addr.sin_addr), AF_INET);
  if (h == NULL) { 
    printf("sent by [%s:%hu]\n", inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));
  } else {
    printf("sent by [%s:%hu]\n", h->h_name, ntohs(addr.sin_port));
  }

  exit(0);
}
