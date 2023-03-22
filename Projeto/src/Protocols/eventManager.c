#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>

#include "eventManager.h"
#include "../SocketProcessing/newMod.h"
#include "../SocketProcessing/withdrawMod.h"
#include "../Common/utils.h"
#include "../UserInterface/userInterface.h"
#include "../SocketProcessing/socketInterface.h"

#define MAXSIZE 4096
#define max(A, B) ((A) >= (B) ? (A) : (B))

/**
 * @brief Event manager function for handling incoming connections and messages.
 *
 * This function sets up the necessary file descriptors for the host node and then enters
 * a loop to wait for incoming connections and messages. It checks the file descriptors
 * using the `select()` system call and then handles any incoming data accordingly.
 * Specifically, it checks for messages from the user interface, incoming connections on
 * the listening socket, messages from the external node, and messages from internal
 * nodes.
 *
 * @param HostNode: A pointer to the host node that will handle incoming connections and
 * messages.
 */
void EventManager(Host *HostNode) {
  int MaxDescriptor = -1, Counter = 0, NewFd = -1;
  char buffer[MAXSIZE];
  fd_set SockSet;
  ssize_t n;
  struct sockaddr addr;
  socklen_t addrlen = sizeof(addr);

  // passed by reference in order to keep changes
  Node *current = HostNode->NodeList;

  // reset file descriptors before next run
  FD_ZERO(&SockSet);

  // Set file descriptors
  FD_SET(STDIN_FILENO, &SockSet);
  FD_SET(HostNode->FdListen, &SockSet);

  if (HostNode->Ext != NULL) {
    FD_SET(HostNode->Ext->Fd, &SockSet);
  }
  Node *temp = HostNode->NodeList;
  while (temp != NULL) {
    FD_SET(temp->Fd, &SockSet);
    temp = temp->next;
  }

  MaxDescriptor = UpdateMaxDes(HostNode);

  // -------------- Wait for incoming connections and messages ------------------//
  Counter =
      select(MaxDescriptor + 1, (&SockSet), (fd_set *)NULL, (fd_set *)NULL, (struct timeval *)NULL);

  if (Counter <= 0) {
    DieWithSys("Function EventManager >>" RED "☠  select() failed");
  }
  while (Counter--) {
    memset(&buffer, 0, MAXSIZE);

    // Check Keyboard
    if (FD_ISSET(STDIN_FILENO, &SockSet)) {
      FD_CLR(STDIN_FILENO, &SockSet);
      if (CustomRead(STDIN_FILENO, buffer, MAXSIZE) == -1) {
        perror("Function EventManager >> " RED "☠  read() failed");
      }
      UserInterfaceParser(buffer, HostNode);
      continue;
    }

    // Check Listening Socket
    if (FD_ISSET(HostNode->FdListen, &SockSet)) {
      if ((NewFd = accept(HostNode->FdListen, &addr, &addrlen)) == -1) {
        perror(""); // redo later
      }
      ReadListeningSock(HostNode, buffer, NewFd);
      FD_CLR(HostNode->FdListen, &SockSet);
      continue;
    }

    // Check external node socket
    if (FD_ISSET(HostNode->Ext->Fd, &SockSet)) {
      FD_CLR(HostNode->Ext->Fd, &SockSet);
      // read info from established socket
      n = CustomRead(HostNode->Ext->Fd, buffer, MAXSIZE);
      if (n == 0) {
        WithdrawHandle(HostNode, HostNode->Ext->Id, HostNode->Ext->Fd);
        continue;

      } else if (n == -1) {
        perror("Function EventManager >> " RED "☠  read() failed");
      }
      SocketInterfaceParser(buffer, HostNode, HostNode->Ext);
      continue;
    }

    // Check internal nodes sockets
    while (current != NULL) {
      if (FD_ISSET(current->Fd, &SockSet)) {
        FD_CLR(current->Fd, &SockSet);
        // read info from established socket
        n = CustomRead(current->Fd, buffer, MAXSIZE);
        if (n == 0) {
          WithdrawHandle(HostNode, current->Id, current->Fd);
          break;

        } else if (n == -1) {
          perror("Function EventManager >>" RED "☠  read() failed");
        }

        SocketInterfaceParser(buffer, HostNode, current);
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
  size_t BytesReceived = 0;
  ssize_t BytesRead = 0;

  while (BytesReceived < BufferSize) {
    BytesRead = read(Fd, (char *)Buffer + BytesReceived, 1); // Read one byte at a time
    if (BytesRead < 0) {
      return -1;
    } else if (BytesRead == 0) {
      // The sender has closed the connection or no more data is available
      break;
    }
    // Add Bytes to stack
    BytesReceived += (size_t)BytesRead;

    if (Buffer[BytesReceived - 1] == '\n') {
      // Newline character encountered, stop reading
      break;
    }
  }

  return (ssize_t)BytesReceived;
}
