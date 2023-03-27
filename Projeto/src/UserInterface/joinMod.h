//===------------------- joinMod.h ------------------------------------------===//
//
// This file defines the interface for joining and managing network servers,
// fetching external information, checking ID uniqueness, and sending connection messages
// between hosts in the network.
//
//===----------------------------------------------------------------------===//
#ifndef JOIN_H
#define JOIN_H

#include "../HostStruct/nodeStructure.h"

// Joins a network server.
void JoinNetworkServer(char buffer[], Host *HostNode);
// Fetches external information.
char *ExternFetch(char *NODELIST, char *Net, char *Id);
// Joins a network server in double connection mode.
void DJoinNetworkServer(char buffer[], Host *HostNode);
// Checks ID uniqueness.
void CheckSingularityId(Host *HostNode, char *Nodelist, char (*Id)[128]);
// Sends a connection message to a new host.
int SendNewMsg(Host *HostNode, char *HostId, char *BootIp, char *BootTCP);

#endif
