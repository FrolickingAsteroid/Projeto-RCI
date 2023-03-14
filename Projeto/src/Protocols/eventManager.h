#ifndef EventManager_H
#define EventManager_H

#include "../HostStruct/nodeStructure.h"
#include "../Common/utils.h"
#include "../UserInterface/userInterface.h"
#include "../SocketProcessing/socketInterface.h"

void EventManager(Host *HostNode);
int UpdateMaxDes(Host *HostNode);

#endif
