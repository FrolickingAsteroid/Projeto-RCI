#ifndef UDP_H
#define UDP_H

#include "../Common/nodeStructure.h"
#include "../Common/utils.h"

char *UDPClient(Host *HostNode, char *msg);
int CheckUDPAnswer(char *UDPAnswer);

#endif
