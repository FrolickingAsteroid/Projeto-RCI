#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
/* ... */
#define max(A,B) ((A)>=(B)?(A):(B))
#define FOREVER for(;;)

int main(void) {
  int fd, newfd, afd;
  fd_set rfds;
  enum {idle, busy} state;
  int maxfd, counter;
  /* ... */
  /* fd = socket(...); bind(fd, ...); listen(fd, ...) */
  state = idle;
  
  FOREVER {
    FD_ZERO(&rfds);
    FD_SET(fd, &rfds); maxfd = fd;
    if (state == busy) {
      FD_SET(afd, &rfds); maxfd = max(maxfd, afd);
    }
    
    counter = select(maxfd+1, &rfds, (fd_set*) NULL, (fd_set*) NULL, (struct timeval*) NULL);
    if (counter <= 0) /*error*/ exit(1);

    if (FD_ISSET(fd, &rfds)) {
      addrlen = sizeof(addr);
      if ((newfd = accept(fd, (struct sockaddr*) &addr, &addrlen)) == -1) /*error*/ exit(1);
      switch(state) {
        case idle: 
          afd = newfd; state = busy; break;
        case busy: /* ... */// write "busy\n" in newfd
          close(newfd); break;
      }
    }

    if (FD_ISSET(fd, &rfds)) {
      if ((n = read(afd,buffer, 128)) != 0) {
        if (n == -1) /*error*/ exit(1);
        /* ... */// write buffer in afd
      } else {
        close(afd); state = idle; //connection closed by peer
      }
    }
  }
  /* close(fd); exit(0); */
}
