#ifndef CHECKSYSTEM_H
#define CHECKSYSTEM_H

typedef struct usr {
  char *IP;
  char *ID;
  char *regIP;
  char regUDP;

} usr_entry;

usr_entry *AddInvocInfo(int argc, char *argv[], usr_entry *usr);
void InvocCheck(int argc, char *argv[]);
void usage(char *name);
int CheckValidAdress(char *IP);
int CheckValidPort(char *PORT);

#endif
