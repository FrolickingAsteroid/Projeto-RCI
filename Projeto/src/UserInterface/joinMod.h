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

void JoinNetworkServer(char buffer[], Host *HostNode);
char *ExternFetch(char *NODELIST, char *Net, char *Id);
void DJoinNetworkServer(char buffer[], Host *HostNode);
void CheckSingularityId(Host *HostNode, char *Nodelist, char (*Id)[128]);
void SendNewMsg(Host *HostNode, char *HostId, char *BootIp, char *BootTCP);

#endif
