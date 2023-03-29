//===- formatchecking.h --------------------------------------------------===//
//
// This header file declares functions that check the format of input strings
// used by the application. These functions verify that the input strings have
// the correct number of arguments, and that these arguments are valid numbers
// or IP addresses.
//
//===----------------------------------------------------------------------===//
#ifndef FORMATCHECKING_H
#define FORMATCHECKING_H

// Checks the format of the network and ID strings
int CheckNetAndId(char *Net, char *Id);
// Validates the boot ID, boot IP, and boot TCP arguments
int BootArgsCheck(char *BootId, char *BootIp, char *BootTCP);
// Parses the content of a name string and checks its format
int NameParser(char *Content);

#endif // FORMATCHECKING_H
