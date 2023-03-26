//===-- nodeStructure.h ---------------------------------------------------===//
//
// This file contains the data structures for the Name, Node, and Host as well
// as function prototypes for initializing and managing these structures.
//
//===----------------------------------------------------------------------===//

#ifndef NODESTRUCTURE_H
#define NODESTRUCTURE_H

#include "../Common/checkInvocationInfo.h"
#include <stddef.h>
#include <time.h>

#define BUFFER_SIZE 1024
//===----------------------------------------------------------------------===//
// Structure representing a circular buffer.
//===----------------------------------------------------------------------===//
typedef struct {
  char data[BUFFER_SIZE]; // The buffer data.
  int ReadPos;            // The read position in the buffer.
  int WritePos;           // The write position in the buffer.
  int Count;              // The number of elements in the buffer.
} CircularBuffer;

//===----------------------------------------------------------------------===//
// Structure representing a new connection Queue
//===----------------------------------------------------------------------===//
typedef struct NewConQueue {
  int NewFd; // New file descriptor to be processed
  CircularBuffer *Cb;
  time_t LastActivity; // Time of the last activity

  struct NewConQueue *next; // pointer to nest item in  the list
} NewConQueue;

//===----------------------------------------------------------------------===//
// Enumeration of command types.
//===----------------------------------------------------------------------===//
typedef enum Command { JOIN, DJOIN, IDLE } Command;

//===----------------------------------------------------------------------===//
// Structure representing content.
//===----------------------------------------------------------------------===//
typedef struct Name {
  char Content[100]; // The content in question
  struct Name *next; // Pointer to the next Name in the list
} Name;

//===----------------------------------------------------------------------===//
// Structure representing a Node.
//===----------------------------------------------------------------------===//
typedef struct Node {
  char *Id;   // The ID of the Node
  int Fd;     // The file descriptor of the Node
  char *IP;   // The IP address of the Node
  int TCPort; // The TCP port of the Node

  CircularBuffer *Cb;

  struct Node *next; // Pointer to the next Node in the list
} Node;

//===----------------------------------------------------------------------===//
// Structure representing a Host.
//===----------------------------------------------------------------------===//
typedef struct Host {
  char *HostId; // The ID of the Host
  int FdListen; // The file descriptor of the Host (Listening socket)

  char *Net; // The network of the Host

  Name *NameList;    // Pointer to the list of Names
  int ForTable[100]; // The forwarding table of the Host

  UsrInvoc *InvocInfo; // The user invocation information
  Command type;        // The type of command invoked by the usr

  Node *Ext;      // Pointer to the external Node
  Node *Bck;      // Pointer to the backup Node
  Node *NodeList; // Pointer to the list of internal Nodes

  NewConQueue *NClist; //

} Host;

// Initialize the Host structure with basic info
Host *InitHostStructure(int Fd, UsrInvoc *UsrInfo);

// Assign Net and Id to the Host structure
void PlugHostNetId(char *Net, char *Id, Host *HostNode);

// Add an internal node to the Host structure
void PlugIntern(char *Ip, int TCP, char *Id, int Fd, Host *HostNode);

void RemoveIntern(Host *HostNode, char *LeavingId);

// Initialize a Node structure with provided info
Node *InitNode(char *Ip, int TCP, char *Id, int Fd);

// Free the memory allocated for a Node structure
void FreeNode(Node *Node);

// Free the memory allocated for a Host structure and its associated data
void LiberateHost(Host *HostNode);

void FreeNodeList(Host *HostNode);

#endif // NODESTRUCTURE_H
