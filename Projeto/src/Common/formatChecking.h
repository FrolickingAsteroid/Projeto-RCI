//===- formatchecking.h - Input Format Checking Library ------------------===//
//
// This header file declares functions that check the format of input strings
// used by the application. These functions verify that the input strings have
// the correct number of arguments, and that these arguments are valid numbers
// or IP addresses.
//
//===----------------------------------------------------------------------===//
#ifndef FORMATCHECKING_H
#define FORMATCHECKING_H

int CheckNetAndId(char *Net, char *Id);
int BootArgsCheck(char *BootId, char *BootIp, char *BootTCP);
int NameParser(char *Content);
#endif //
