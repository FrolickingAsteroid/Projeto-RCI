#ifndef USERINTERFACE_H
#define USERINTERFACE_H

#include "../HostStruct/nodeStructure.h"
#include "../Common/utils.h"
#include "../SocketProcessing/socketInterface.h"
#include "exitMod.h"
#include "joinMod.h"
#include "showMod.h"

void UserInterfaceParser(char buffer[], Host *HostNode);

#endif // !USERINTERFACE_H
