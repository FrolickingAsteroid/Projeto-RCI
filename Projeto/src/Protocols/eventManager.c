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
#include "../HostStruct/CirBuffer.h"

#include "../Common/utils.h"

#include "../UserInterface/userInterface.h"

#define MAXSIZE 256 // Intermidiate Buffer maxsize

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
 * - The file descriptor of all new connections that have not been processed
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

  // Set external Fd
  if (HostNode->Ext != NULL) {
    FD_SET(HostNode->Ext->Fd, SockSet);
  }

  // Set internal Fds
  Node *current_nodes = NodeList;
  while (current_nodes != NULL) {
    FD_SET(current_nodes->Fd, SockSet);
    current_nodes = current_nodes->next;
  }

  // Set new con Fd
  NewConQueue *current_con = HostNode->NClist;
  while (current_con != NULL) {
    FD_SET(current_con->NewFd, SockSet);
    current_con = current_con->next;
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
static int WaitOnFileDescriptors(fd_set *SockSet, int MaxDescriptor, Host *HostNode) {
  int Counter = select(MaxDescriptor + 1, SockSet, NULL, NULL, NULL);
  if (Counter == -1) {
    DieWithSys("Function WaitOnFileDescriptors >>" RED "select() failed" RESET, HostNode);
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
  if (read(STDIN_FILENO, buffer, MAXSIZE - 1) == -1) {
    PerrorWrapper("Function HandleKeyboardInput >> " RED "read() failed" RESET);
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
    PerrorWrapper("Function HandleListeningSocket >> " RED "accept() failed" RESET);
    return;
  }

  if (read(*NewFd, buffer, MAXSIZE - 1) == -1) {
    PerrorWrapper("Function HandleListeningSocket >> " RED "read() failed" RESET);
    close(*NewFd);
    return;
  }

  // inserts new connection in connection queue and handle if message is complete
  PlugNC(*NewFd, HostNode, buffer);

  if (HostNode->NClist != NULL) {
    HandleNewCon(HostNode, HostNode->NClist);
  }
}

/**
 * @brief Handles the new connection queue of a host node.
 *
 * @param HostNode: The host node containing the new connection queue.
 * @param SockSet: The file descriptor set to check for activity.
 * @param Buffer: The buffer to store the read data.
 *
 * @return Returns 1 if a new connection is handled, 0 otherwise.
 */
static int HandleNewConQueue(Host *HostNode, fd_set *SockSet, char *Buffer) {
  NewConQueue *Current = HostNode->NClist;

  // Iterate through the new connection queue
  while (Current != NULL) {
    if (FD_ISSET(Current->NewFd, SockSet)) {
      FD_CLR(Current->NewFd, SockSet);

      // Read data from the file descriptor
      if (read(Current->NewFd, Buffer, MAXSIZE - 1) <= 0) {
        PerrorWrapper("Function HandleNewConQueue >> " RED "read() failed" RESET);
        close(Current->NewFd), RemoveNC(HostNode, Current->NewFd);
        return 1;
      }
      // Write the data to the circular buffer
      if (CbWrite(Current->Cb, Buffer, strlen(Buffer)) != strlen(Buffer)) {
        close(Current->NewFd), RemoveNC(HostNode, Current->NewFd);
        return 1;
      }

      // Handle the new connection
      HandleNewCon(HostNode, Current);
      return 1;
    }
    Current = Current->next;
  }
  return 0;
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
  char SecBuffer[MAXSIZE] = "";

  n = read(HostNode->Ext->Fd, buffer, MAXSIZE - 1);
  if (n == 0) {
    WithdrawHandle(HostNode, HostNode->Ext->Id, HostNode->Ext->Fd);
    return;
  } else if (n == -1) {
    PerrorWrapper("Function HandleExternalNodeSocket >> " RED "read() failed" RESET);
    return;
  }

  // Check if Cb is not full, if so, flush contents
  if (CbAvail(HostNode->Ext->Cb) == 0) {
    LiberateCircularBuffer(HostNode->Ext->Cb);
  }

  // write into circular buffer
  if (CbWrite(HostNode->Ext->Cb, buffer, strlen(buffer)) != strlen(buffer)) {
    return;
  }
  // while Cb contains complete messages process them
  while (CbRead(HostNode->Ext->Cb, SecBuffer, sizeof(SecBuffer) - 1)) {
    SocketInterfaceParser(SecBuffer, HostNode, HostNode->Ext);
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
  char SecBuffer[MAXSIZE] = "";

  n = read(current->Fd, buffer, MAXSIZE - 1);
  if (n == 0) {
    WithdrawHandle(HostNode, current->Id, current->Fd);
    return;
  } else if (n == -1) {
    PerrorWrapper("Function HandleInternalNodeSocket >> " RED "read() failed" RESET);
    return;
  }

  // Check if Cb is not full, if so, flush contents
  if (CbAvail(current->Cb) == 0) {
    LiberateCircularBuffer(current->Cb);
  }

  // write into circular buffer
  if (CbWrite(current->Cb, buffer, strlen(buffer)) != strlen(buffer)) {
    return;
  }
  // while Cb contains complete messages process them
  while (CbRead(current->Cb, SecBuffer, sizeof(SecBuffer) - 1)) {
    SocketInterfaceParser(SecBuffer, HostNode, current);
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
static int UpdateMaxDes(Host *HostNode) {

  // initialize maxFd to the listen file descriptor
  int maxFd = HostNode->FdListen;

  if (HostNode->NClist != NULL) {

    // update maxFd if New con FD is greater
    maxFd = max(maxFd, HostNode->NClist->NewFd);
  }

  if (HostNode->Ext != NULL) {
    // update maxFd if external FD is greater
    maxFd = max(maxFd, HostNode->Ext->Fd);
  }

  if (HostNode->NodeList != NULL) {
    // update maxFd if internal FD is greater
    maxFd = max(maxFd, HostNode->NodeList->Fd);
  }

  return maxFd;
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

  int Counter = WaitOnFileDescriptors(&SockSet, MaxDescriptor, HostNode);
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

    // Queued connections
    if (HandleNewConQueue(HostNode, &SockSet, buffer)) {
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
  CleanInactiveConnections(HostNode);
}
