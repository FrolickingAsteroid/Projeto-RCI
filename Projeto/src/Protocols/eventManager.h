//===------------------- EventManager.h ------------------------------------===//
//
// This file declares the interface for the event manager, which is responsible
// for handling various events that occur within the host node, such as
// incoming connections, new messages, and other updates.
//
//===----------------------------------------------------------------------===//

#ifndef EventManager_H
#define EventManager_H

#include "../HostStruct/nodeStructure.h"

// Function to manage file descriptor events
void EventManager(Host *HostNode);

#endif // !EventManager_H
