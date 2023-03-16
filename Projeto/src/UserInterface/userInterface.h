#ifndef USERINTERFACE_H
#define USERINTERFACE_H

#include "../HostStruct/nodeStructure.h"
#include "../Common/utils.h"
#include "../SocketProcessing/socketInterface.h"

void UserInterfaceParser(char buffer[], Host *HostNode);
void ShowParser(Host *HostNode, char *Buffer);

#endif // !USERINTERFACE_H
