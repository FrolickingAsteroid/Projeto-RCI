#include <asm-generic/errno.h>
#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <errno.h>

#include "retry.h"
#include "utils.h"

#define MAX_ATTEMPTS 5 // Number of attempts before giving up

/**
 * @brief Retry function for socket operations with exponential backoff (used mainly with
 * recvfrom()).
 *
 * @param func: The function to retry.
 * @param sockfd: The socket file descriptor.
 * @param buf: The buffer to send or receive data.
 * @param len: The length of the buffer.
 * @param flags: Flags for the socket operation.
 * @param src_addr: The source address for the socket operation.
 * @param addrlen: The length of the source address.
 *
 * @return ssize_t: The number of bytes sent or received, or -1 on failure.
 */
ssize_t retry(retry_func func, int sockfd, void *buf, size_t len, int flags,
              struct sockaddr *src_addr, socklen_t *addrlen) {
  int attempts = 0;
  ssize_t ret = 0;

  // Set Timeout
  struct timeval tv = {.tv_sec = 1, .tv_usec = 0};

  // Set timeout for server answer
  if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) < 0) {
    perror(RED "☠  setsockopt() failed");
    close(sockfd);
  }

  while (attempts < MAX_ATTEMPTS) {
    ret = func(sockfd, buf, len, flags, src_addr, addrlen);
    if (ret >= 0) {
      return ret;
    }

    // See if function has reached timeout
    if (errno == EAGAIN || errno == EWOULDBLOCK || errno == ENETDOWN || errno == ENETUNREACH) {

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

/**
 * @brief Retry function for sendto() socket operation with exponential backoff.
 *
 * @param sockfd: The socket file descriptor.
 * @param buffer: The buffer containing the data to send.
 * @param buflen: The length of the buffer.
 * @param flags: Flags for the sendto() operation.
 * @param dest_addr: The destination address for the sendto() operation.
 * @param addrlen: The length of the destination address.
 *
 * @return ssize_t The number of bytes sent, or -1 on failure.
 */
ssize_t SendtoRetry(int sockfd, char *buffer, size_t buflen, int flags,
                    const struct sockaddr *dest_addr, socklen_t addrlen) {
  int attempts = 0;
  ssize_t ret;

  // Set Timeout
  struct timeval tv = {.tv_sec = 1, .tv_usec = 0};

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
    if (errno == EAGAIN || errno == EWOULDBLOCK || errno == ENETDOWN || errno == ENETUNREACH) {

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
