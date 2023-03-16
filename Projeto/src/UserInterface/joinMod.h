#ifndef JOIN_H
#define JOIN_H

#include "../HostStruct/nodeStructure.h"

void JoinNetworkServer(char buffer[], Host *HostNode);
char *ExternFetch(char *NODELIST, char *Net, char *Id);
void DJoinNetworkServer(char buffer[], Host *HostNode);
int CheckNetAndId(char *Net, char *Id);
int CheckNumberOfArgs(char *Buffer, int ArgNumber);
int BootArgsCheck(char *BootId, char *BootIp, char *BootTCP);
void CheckSingularityId(char *Nodelist, char (*Id)[128]);
void SendNewMsg(Host *HostNode, char *HostId, char *BootIp, char *BootTCP);

#endif
