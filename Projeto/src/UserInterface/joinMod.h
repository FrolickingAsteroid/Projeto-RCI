#ifndef JOIN_H
#define JOIN_H

#include "../Common/checkInvocationInfo.h"
#include "../Common/formatChecking.h"
#include "../Common/nodeStructure.h"
#include "../Common/utils.h"
#include "../Protocols/TCP.h"
#include "../Protocols/UDP.h"

void JoinNetworkServer(char buffer[], Host *HostNode);
char *ExternFetch(char *NODELIST, char *buffer);
void DJoinNetworkServer(char buffer[], Host *HostNode);
int CheckNetAndId(char *Net, char *Id);
int CheckNumberOfArgs(char *Buffer, int ArgNumber);
int BootArgsCheck(char *BootId, char *BootIp, char *BootTCP);

#endif
