//===-- utils.h ----------------------------------------------------------===//
//
// This header file defines a set of helper functions, including functions for error handling,
// string parsing, and printing output to the console. These functions are used throughout the code
// in this project.
//
//===----------------------------------------------------------------------===//
#ifndef UTILS_H
#define UTILS_H

// ANSI color codes for printing colored output to the console
#define RESET "\x1B[0m"
#define RED "\x1B[31m"
#define GRN "\033[1;92m"
#define YEL "\x1B[33m"
#define BLU "\033[1;94m"
#define KMAG "\x1B[35m"
#define CYAN "\033[0;36m"

// Clear the console screen
#define clear() printf("\033[H\033[J")

#include "../HostStruct/nodeStructure.h"

extern int Verbose; // global variable for verbose mode use

// Display a user error message and terminate the program
void DieWithUsr(const char *msg, const char *detail);
// Display a system error message and terminate the program
void DieWithSys(const char *msg, Host *HostNode);
// Check if a given string is a valid number
int IsNumber(char *str);
// Check if a given string contains only alphanumeric characters
int IsAlphanumeric(char *str);
// Print usage instructions for the application
void Usage(char *name);
// Print a command-not-found error message
void CommandNotFound(char *Command, char *msg);
// Print the server's response to a command
void ServerAnswer(char *Answer, char *place);
// Print usage instructions for the network interface
void InterfaceUsage();

void PerrorWrapper(const char *msg);

void prompt();

void clear_stdout_line();

#endif // UTILS_H
