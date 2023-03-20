//===-- UserInterface.h ---------------------------------------------------===//
//
// This header file is a part of the 'UserInterface' module. It contains the
// function declarations for user interface operations, such has input parsing.
// These functions are used to handle user interactions with the system throughout the code in this
// project.
//
//===----------------------------------------------------------------------===//

#ifndef USERINTERFACE_H
#define USERINTERFACE_H

#include "../HostStruct/nodeStructure.h"

// Parses user input and processes the corresponding commands for the HostNode.
void UserInterfaceParser(char buffer[], Host *HostNode);

// Parses the 'show' commands and displays the output based on the command given.
void ShowParser(Host *HostNode, char *Buffer);

#endif // !USERINTERFACE_H
