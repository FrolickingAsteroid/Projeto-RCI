#ifndef SOCKETINTERFACE_H
#define SOCKETINTERFACE_H

#include "../Common/nodeStructure.h"
#include "../Protocols/TCP.h"
#include "../Common/utils.h"
#include "withdrawMod.h"

void SocketInterfaceParser(char *Buffer, Host *HostNode);
#endif
