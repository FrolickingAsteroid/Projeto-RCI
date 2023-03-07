#ifndef UTILS_H
#define UTILS_H

#define KMAG "\x1B[35m"
#define RESET "\x1B[0m"
#define RED "\x1B[31m"
#define BLU "\x1B[34m"
#define GRN "\x1B[32m"
#define clear() printf("\033[H\033[J")

extern int errno;

void DieWithUsr(const char *msg, const char *detail);
void DieWithSys(const char *msg);
int IsNumber(char *str);
void Usage(char *name);
void CommandNotFound(char *Command, char *msg);

#endif
