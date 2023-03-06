#ifndef UDP_H
#define UDP_H

#include "nodeStructure.h"
#include "utils.h"

char *UDPClient(Host *HostNode, char *msg);
int CheckUDPAnswer(char *UDPAnswer);

#endif
