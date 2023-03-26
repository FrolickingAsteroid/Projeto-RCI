#include <asm-generic/socket.h>
#include <stdio.h>
#include <string.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>

#include "eventManager.h"

#include "../SocketProcessing/newMod.h"
#include "../SocketProcessing/withdrawMod.h"
#include "../SocketProcessing/socketInterface.h"

#include "../HostStruct/ncQueue.h"

#include "../Common/utils.h"

#include "../UserInterface/userInterface.h"

#define MAXSIZE 256
#define max(A, B) ((A) >= (B) ? (A) : (B))
/**
 * @brief Set up the file descriptor set for use with select().
 *
 * This function sets up the file descriptor set for use with the `select()` function. It adds the
 * following file descriptors to the set:
 *
 * - STDIN_FILENO: the file descriptor for standard input (keyboard input).
 * - FdListen: the file descriptor for the listening socket.
 * - HostNode->Ext->Fd: the file descriptor for the external node (if it exists).
 * - The file descriptors of all internal nodes in the NodeList linked list.
 *
 * @param SockSet: Pointer to the `fd_set` structure to be set up.
 * @param FdListen: The file descriptor of the listening socket.
 * @param NodeList: Pointer to the linked list of internal nodes.
 * @param HostNode: Pointer to the `Host` structure containing the external node information.
 *
 * @note This function modifies the `SockSet` `fd_set` structure passed as a parameter.
 */
static void SetFileDescriptors(fd_set *SockSet, int FdListen, Node *NodeList, Host *HostNode) {
  FD_ZERO(SockSet);
  FD_SET(STDIN_FILENO, SockSet);
  FD_SET(FdListen, SockSet);

  if (HostNode->Ext != NULL) {
    FD_SET(HostNode->Ext->Fd, SockSet);
  }

  Node *current = NodeList;
  while (current != NULL) {
    FD_SET(current->Fd, SockSet);
    current = current->next;
  }
}

/**
 * @brief Wait on file descriptors using select().
 *
 * This function waits on the file descriptors in the given `fd_set` structure using the `select()`
 * function.
 *
 * @param SockSet: Pointer to the `fd_set` structure containing the file descriptors to be checked.
 * @param MaxDescriptor: The maximum file descriptor in the `SockSet` set.
 *
 * @return The number of file descriptors that are ready for reading, or -1 if an error occurs.
 *
 * @note This function assumes that `SockSet` has already been set up using `SetFileDescriptors()`.
 */
static int WaitOnFileDescriptors(fd_set *SockSet, int MaxDescriptor) {
  int Counter = select(MaxDescriptor + 1, SockSet, NULL, NULL, NULL);
  if (Counter == -1) {
    DieWithSys("Function WaitOnFileDescriptors >>" RED "☠  select() failed");
  }
  return Counter;
}

/**
 * @brief Handle keyboard input.
 *
 * This function reads keyboard input using the `CustomRead()` function and passes the input buffer
 * to the `UserInterfaceParser()` function for parsing.
 *
 * @param HostNode: Pointer to the `Host` structure containing the node information.
 * @param buffer: Pointer to the buffer where the keyboard input will be stored.
 *
 * @note This function assumes that `buffer` is large enough to hold the input data.
 */
static void HandleKeyboardInput(Host *HostNode, char *buffer) {
  if (read(STDIN_FILENO, buffer, MAXSIZE) == -1) {
    perror("Function HandleKeyboardInput >> " RED "☠  read() failed");
    return;
  }
  UserInterfaceParser(buffer, HostNode);
}

/**
 * @brief Handle incoming connections on the listening socket.
 *
 * This function accepts an incoming connection on the listening socket, reads data from the socket
 * using the `ReadListeningSock()` function, and stores the data in the provided buffer.
 *
 * @param HostNode: Pointer to the `Host` structure containing the node information.
 * @param buffer: Pointer to the buffer where the incoming data will be stored.
 * @param NewFd: Pointer to the variable where the new socket file descriptor will be stored.
 * @param addr: Pointer to a `struct sockaddr` structure that will be filled with the address
 * of the remote socket.
 * @param addrlen: Pointer to a `socklen_t` variable that will be set to the size of the
 * `addr` structure.
 *
 * @note This function assumes that the listening socket has already been set up and is ready to
 * accept incoming connections.
 */
static void HandleListeningSocket(Host *HostNode, char *buffer, int *NewFd, struct sockaddr *addr,
                                  socklen_t *addrlen) {
  *NewFd = accept(HostNode->FdListen, addr, addrlen);

  if (*NewFd == -1) {
    perror("Function HandleListeningSocket >> " RED "☠  accept() failed");
    return;
  }

  if (read(*NewFd, buffer, MAXSIZE) == -1) {
    perror("Function HandleListeningSocket >> " RED "☠  read() failed");
    close(*NewFd);
    return;
  }
  // inserts new connection in connection queue and handle if message is complete
  PlugNC(*NewFd, HostNode, buffer);
  HandleNewCon(HostNode, HostNode->NClist);
}

/**
 * @brief Handle incoming data on the external node socket.
 *
 * This function reads data from the external node socket using the `CustomRead()` function, and
 * passes the data buffer to the `SocketInterfaceParser()` function for parsing. If the read
 * operation returns 0, this function calls `WithdrawHandle()` to remove the external node from the
 * `NodeList`.
 *
 * @param HostNode: Pointer to the `Host` structure containing the node information.
 * @param buffer: Pointer to the buffer where the incoming data will be stored.
 * @param n: Pointer to a `ssize_t` variable that will be set to the number of bytes read
 * from the socket.
 *
 * @note This function assumes that the external node socket has already been set up and is ready to
 * receive data.
 */
static void HandleExternalNodeSocket(Host *HostNode, char *buffer, ssize_t n) {
  char *Token = NULL;
  n = CustomRead(HostNode->Ext->Fd, buffer, MAXSIZE);
  if (n == 0) {
    WithdrawHandle(HostNode, HostNode->Ext->Id, HostNode->Ext->Fd);
  } else if (n == -1) {
    perror("Function HandleExternalNodeSocket >> " RED "☠  read() failed");
  }
  for (Token = strtok(buffer, "\n"); Token != NULL; Token = strtok(NULL, "\n")) {
    SocketInterfaceParser(buffer, HostNode, HostNode->Ext);
  }
}

/**
 * @brief Handle incoming data on an internal node socket.
 *
 * This function reads data from the provided internal node socket using the `CustomRead()`
 * function, and passes the data buffer to the `SocketInterfaceParser()` function for parsing. If
 * the read operation fails, this function calls `WithdrawHandle()` to remove the node from the
 * `NodeList`.
 *
 * @param HostNode: Pointer to the `Host` structure containing the node information.
 * @param buffer: Pointer to the buffer where the incoming data will be stored.
 * @param current: Pointer to the `Node` structure representing the internal node.
 * @param n: Pointer to a `ssize_t` variable that will be set to the number of bytes read
 * from the socket.
 *
 * @note This function assumes that the internal node socket has already been set up and is ready to
 * receive data.
 */
static void HandleInternalNodeSocket(Host *HostNode, char *buffer, Node *current, ssize_t n) {
  char *Token = NULL;
  n = CustomRead(current->Fd, buffer, MAXSIZE);
  if (n == 0) {
    WithdrawHandle(HostNode, current->Id, current->Fd);
  } else if (n == -1) {
    perror("Function HandleInternalNodeSocket >> " RED "☠  read() failed");
  }
  // Handle multiple requests at the same time
  for (Token = strtok(buffer, "\n"); Token != NULL; Token = strtok(NULL, "\n")) {
    SocketInterfaceParser(buffer, HostNode, current);
  }
}

/**
 * @brief Main event loop for the node program.
 *
 * This function implements the main event loop for the node program. It sets up the necessary file
 * descriptors for the listening socket, internal node sockets, and external node socket, and uses
 * the `select()` function to wait for incoming connections and messages. When data is received on a
 * socket, the appropriate handler function is called to process the data.
 *
 * @param[in] HostNode Pointer to the `Host` structure containing the node information.
 *
 * @note This function assumes that the `Host` structure and all associated data structures have
 * already been initialized and set up properly.
 */
void EventManager(Host *HostNode) {
  int MaxDescriptor = -1, NewFd = -1;
  char buffer[MAXSIZE];
  fd_set SockSet;
  ssize_t n = 0;
  struct sockaddr addr;
  socklen_t addrlen = sizeof(addr);

  Node *current = HostNode->NodeList;

  SetFileDescriptors(&SockSet, HostNode->FdListen, HostNode->NodeList, HostNode);
  MaxDescriptor = UpdateMaxDes(HostNode);

  int Counter = WaitOnFileDescriptors(&SockSet, MaxDescriptor);
  while (Counter--) {
    memset(buffer, 0, MAXSIZE);
    // Keyboard
    if (FD_ISSET(STDIN_FILENO, &SockSet)) {
      FD_CLR(STDIN_FILENO, &SockSet);
      HandleKeyboardInput(HostNode, buffer);
      continue;
    }
    // Listening Socket
    if (FD_ISSET(HostNode->FdListen, &SockSet)) {
      FD_CLR(HostNode->FdListen, &SockSet);
      HandleListeningSocket(HostNode, buffer, &NewFd, &addr, &addrlen);
      continue;
    }
    // External Node
    if (HostNode->Ext != NULL && FD_ISSET(HostNode->Ext->Fd, &SockSet)) {
      FD_CLR(HostNode->Ext->Fd, &SockSet);
      HandleExternalNodeSocket(HostNode, buffer, n);
      continue;
    }
    // Internal Nodes
    while (current != NULL) {
      if (FD_ISSET(current->Fd, &SockSet)) {
        FD_CLR(current->Fd, &SockSet);
        HandleInternalNodeSocket(HostNode, buffer, current, n);
        break;
      }
      current = current->next;
    }
  }
}

/**
 * @brief Updates the maximum number of file descriptors.
 *
 * This function determines the maximum number of file descriptors, the function
 * compares the listening FD to the maximum file descriptor found so far. The final
 * maximum file descriptor is returned.
 *
 * @param HostNode A pointer to the host for which to update the maximum file descriptor.
 *
 * @return The maximum file descriptor for the given host.
 */
int UpdateMaxDes(Host *HostNode) {
  // initialize maxFd to the listen file descriptor
  int maxFd = HostNode->FdListen;

  // check if host has an external nodes
  if (HostNode->Ext != NULL) {
    // update maxFd if external FD is greater
    maxFd = max(maxFd, HostNode->Ext->Fd);

    if (HostNode->NodeList != NULL) {
      // update maxFd if internal FD is greater
      maxFd = max(maxFd, HostNode->NodeList->Fd);
    }
  }

  return maxFd;
}

/**
 * @brief Reads data from a file descriptor into a buffer until a newline character is encountered,
 * up to a specified size.
 *
 * This function reads data one byte at a time from a file descriptor and stores it in a provided
 * buffer. The reading process stops when a newline character is encountered, the specified buffer
 * size is reached, or the sender closes the connection.
 *
 * @param Fd: The file descriptor to read data from.
 * @param Buffer: A pointer to the buffer where the read data will be stored.
 * @param BufferSize: The maximum size of the buffer (number of bytes to read).
 *
 * @return The number of bytes read, or -1 if an error occurs during the read operation.
 */
ssize_t CustomRead(int Fd, char *Buffer, size_t BufferSize) {
  fd_set SockSet;
  struct timeval timeout;
  int retval;

  size_t BytesReceived = 0;
  ssize_t BytesRead = 0;

  while (BytesReceived < BufferSize) {
    // Wait for data to be available using select()
    FD_ZERO(&SockSet);
    FD_SET(Fd, &SockSet);
    timeout.tv_sec = 5;
    timeout.tv_usec = 0;

    retval = select(Fd + 1, &SockSet, NULL, NULL, &timeout);
    if (retval == -1) {
      return -1; // Error occurred
    } else if (retval == 0) {
      return (ssize_t)BytesReceived; // Timeout occurred
    }

    // Call read() to read the available data
    BytesRead = read(Fd, Buffer + BytesReceived, 1); // Read one byte at a time
    if (BytesRead < 0) {
      return -1; // Error occurred
    } else if (BytesRead == 0) {
      return (ssize_t)BytesReceived; // End of file reached
    }
    // printf("%lu\n", BytesReceived);
    //  Add bytes to the stack
    BytesReceived += (size_t)BytesRead;
  }
  return (ssize_t)BytesReceived;
}
