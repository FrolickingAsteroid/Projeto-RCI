#ifndef USERINTERFACE_H
#define USERINTERFACE_H

#include "UDP.h"
#include "checkInvocationInfo.h"
#include "nodeStructure.h"
#include "utils.h"

void UserInterfaceParser(char buffer[], Host *HostNode);
void JoinNetworkServer(char buffer[], Host *HostNode);
char *ExternFetch(char *NODELIST, Host *HostNode, char *buffer);
void DJoinNetworkServer(char buffer[], Host *HostNode, int FlagJoin);

#endif // !USERINTERFACE_H
