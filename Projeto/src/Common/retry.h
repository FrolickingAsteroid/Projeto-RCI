#ifndef RETRY_H
#define RETRY_H

#include <netdb.h>
typedef ssize_t (*retry_func)(int sockfd, void *buf, size_t len, int flags,
                              struct sockaddr *src_addr, socklen_t *addrlen);

ssize_t retry(retry_func func, int sockfd, void *buf, size_t len, int flags,
              struct sockaddr *src_addr, socklen_t *addrlen);

ssize_t SendtoRetry(int sockfd, char *buffer, size_t buflen, int flags,
                    const struct sockaddr *dest_addr, socklen_t addrlen, const char *FuncName);

#endif // !RETRY_H
