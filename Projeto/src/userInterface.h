#ifndef USERINTERFACE_H
#define USERINTERFACE_H

#include "nodeStructure.h"

void UserInterfaceParser(char buffer[], Host *HostNode);
void JoinNetworkServer(char buffer[], Host *HostNode);
void DJoinNetworkServer(char buffer[]);

#endif // !USERINTERFACE_H
