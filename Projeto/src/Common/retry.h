//===------------------- retry.h -------------------------------------------===//
//
// This file declares the interface for the retry functionality with exponential
// backoff when performing socket operations, as well as the SendtoRetry function
// which implements retries for the sendto function.
//
//===----------------------------------------------------------------------===//

#ifndef RETRY_H
#define RETRY_H

#include <netdb.h>

// Typedef for a function pointer that matches the signature of common socket
// functions (e.g., recvfrom, sendto)
typedef ssize_t (*retry_func)(int sockfd, void *buf, size_t len, int flags,
                              struct sockaddr *src_addr, socklen_t *addrlen);

// Function for performing socket operations with exponential backoff retries
ssize_t retry(retry_func func, int sockfd, void *buf, size_t len, int flags,
              struct sockaddr *src_addr, socklen_t *addrlen);

// Function for sending data using sendto with exponential backoff retries
ssize_t SendtoRetry(int sockfd, char *buffer, size_t buflen, int flags,
                    const struct sockaddr *dest_addr, socklen_t addrlen);

#endif // !RETRY_H
