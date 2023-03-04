#ifndef UTILS_H
#define UTILS_H

#define KMAG "\x1B[35m"
#define RESET "\x1B[0m"

void DieWithUsr(const char *msg, const char *detail);
void DieWithSys(const char *msg);
int IsNumber(char *str);
void Usage(char *name);

#endif
