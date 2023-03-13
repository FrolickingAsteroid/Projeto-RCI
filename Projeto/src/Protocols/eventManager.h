#ifndef EventManager_H
#define EventManager_H

#include "../HostStruct/nodeStructure.h"
#include "../Common/utils.h"
#include "../Protocols/TCP.h"
#include "../UserInterface/userInterface.h"
#include "../SocketProcessing/socketInterface.h"
#include "../SocketProcessing/withdrawMod.h"

void EventManager(Host *HostNode);
int UpdateMaxDes(Host *HostNode);

#endif
