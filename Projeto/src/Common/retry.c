
#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <errno.h>
#include "retry.h"
#include "utils.h"

#define MAX_ATTEMPTS 10

void print_progress_bar(int current, int total) {
  int progress = (int)(((double)current / (double)total) * 100);
  printf("\r[");
  for (int i = 0; i < 100; i += 2) {
    if (i <= progress) {
      printf("#");
    } else {
      printf(" ");
    }
  }
  printf("] %d%%", progress);
  fflush(stdout);
}

ssize_t retry(retry_func func, int sockfd, void *buf, size_t len, int flags,
              struct sockaddr *src_addr, socklen_t *addrlen) {
  int attempts = 0;
  ssize_t ret = 0;

  while (attempts < MAX_ATTEMPTS) {
    ret = func(sockfd, buf, len, flags, src_addr, addrlen);
    if (ret >= 0) {
      return ret;
    }
    fprintf(stderr, "retrying...\n");
    double delay = pow(2, attempts) * 200.0;
    usleep((unsigned int)delay);

    attempts++;
  }

  return -1;
}

ssize_t SendtoRetry(int sockfd, char *buffer, size_t buflen, int flags,
                    const struct sockaddr *dest_addr, socklen_t addrlen) {
  int attempts = 0;
  ssize_t ret;

  // Set Timeout for Server answer
  struct timeval tv = {.tv_sec = 10, .tv_usec = 0};

  // Set timeout for server answer
  if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) < 0) {
    perror(RED "☠  setsockopt() failed");
    close(sockfd);
  }

  // Retry loop
  while (attempts < MAX_ATTEMPTS) {
    ret = sendto(sockfd, buffer, buflen, flags, dest_addr, addrlen);
    if (ret != -1) {
      return ret;
    }

    // See if function has reached timeout
    if (errno == EAGAIN || errno == EWOULDBLOCK) {

      printf(YEL "retrying ...\n" RESET);
      double delay = pow(10, attempts) * 200.0;
      usleep((unsigned int)delay);

      attempts++;
    } else {
      // Error occurred that is not a timeout
      break;
    }
  }

  return -1;
}
