#ifndef FORMATCHECKING_H
#define FORMATCHECKING_H

#include "utils.h"

int CheckNetAndId(char *Net, char *Id);
int CheckNumberOfArgs(char *Buffer, int ArgNumber);
int BootArgsCheck(char *BootId, char *BootIp, char *BootTCP);

#endif //
