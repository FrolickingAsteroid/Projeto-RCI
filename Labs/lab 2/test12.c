#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <errno.h>
extern int errno;

#define FOREVER for(;;)

int main(void) {
  int fd, newfd, addrlen, n, nw, ret;
  struct sockaddr_in addr;
  char *ptr, buffer[128];
  pid_t pid;
  void (*old_handler) (int); // interrupt handler

  if ((old_handler = signal(SIGCHLD,SIG_IGN)) == SIG_ERR) /*error*/ exit(1);

  if ((fd = socket(AF_INET,SOCK_STREAM,0)) == -1) /*error*/ exit(1);
  memset((void*) &addr, (int) '\0', sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_addr.s = htonl(INADDR_ANY);
  addr.sin_port = htons(9000);
  if (bind(fd,(struct sockaddr*) &addr, &addrlen) == -1) /*error*/ exit(1);
  if (listen(fd,2) == -1) /*error*/ exit(1);

  FOREVER {
    addrlen = sizeof(addr);
    do newfd = accept(fd, (struct sockaddr*) &addr, &addrlen); //wait for a connection
    while(newfd == -1 && errno == EINTR);
    if (newfd == -1) /*error*/ exit(1);
    
    if((pid = fork()) == -1) {
      /*error*/ exit(1);
    } else if (pid == 0) { // child process
      close(fd);
      while ((n = read(newfd, buffer, 128)) != 0) {
        if (n == -1) /*error*/ exit(1);
        ptr = &buffer[0];
        while (n > 0) {
          if ((nw = write(newfd, ptr, n)) <= 0) /*error*/ exit(1);
          n -= nw; ptr += nw;
        }
      }
    close(newfd); exit(0);
    }
    //parent process
    do ret = close(newfd); while (ret == -1 && errno == EINTR);
    if (ret == -1) /*error*/ exit(1);
  }

  /* close(fd); exit(0); */
}
