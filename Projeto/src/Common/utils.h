//===-- utils.h - Helper functions -----------------------------------------===//
//
// This header file defines a set of helper functions, including functions for error handling,
// string parsing, and printing output to the console. These functions are used throughout  code in
// this project.
//
//===----------------------------------------------------------------------===//

#ifndef UTILS_H
#define UTILS_H

// ANSI color codes for printing colored output to the console
#define KMAG "\x1B[35m"
#define RESET "\x1B[0m"
#define RED "\x1B[31m"
#define BLU "\x1B[34m"
#define GRN "\x1B[32m"

// Clear the console screen
#define clear() printf("\033[H\033[J")

// Standard library error number
extern int errno;

void DieWithUsr(const char *msg, const char *detail);
void DieWithSys(const char *msg);
int IsNumber(char *str);
int IsAlphanumeric(char *str);
void Usage(char *name);
void CommandNotFound(char *Command, char *msg);
void ServerAnswer(char *UDPAnswer);
void InterfaceUsage();

#endif
