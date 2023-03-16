
Project Layout
===============

This project is organized into several directories and files, which are structured as follows:

main.c
The main source file of the project

Common
------
The Common directory contains utility functions and modules used throughout the project.

- checkInvocationInfo.c          ->Functions for validating user input and invocation info
- checkInvocationInfo.h
- formatChecking.c               ->Functions to check and validate formatting
- formatChecking.h
- utils.c                        ->General utility functions used across the project
- utils.h

HostStruct
----------
The HostStruct directory includes the files related to the host structure functions.

- forwardingTable.c              ->Functions for managing the forwardingTable
- forwardingTable.h
- nodeStructure.c                ->Functions and structures for managing nodes and the host
- nodeStructure.h

Protocols
---------
The Protocols directory contains modules related to event management and network protocols (TCP and UDP).

- eventManager.c                 ->Functions to manage and handle socket events in the project (select())
- eventManager.h
- TCP.c                          ->Functions related to the TCP protocol connections
- TCP.h
- UDP.c                          ->Functions related to the UDP protocol connections
- UDP.h

SocketProcessing
-----------------
The SocketProcessing directory contains the files related to socket processing and communication.

- newMod.c                       ->Functions related to creating and processing new connections
- newMod.h
- queryMod.c                     ->Functions related to query and content processing
- queryMod.h
- socketInterface.c              ->Functions for managing sockets and their interactions
- socketInterface.h
- withdrawMod.c                  ->Functions related to withdrawing and disconnecting from the network
- withdrawMod.h

UserInterface
-------------
The UserInterface directory contains the files related to user interaction and command handling.

- exitMod.c                      ->Functions for handling exit/leave-related commands
- exitMod.h
- getMod.c                       ->Functions for handling get-related commands
- getMod.h
- joinMod.c                      ->Functions for handling join/djoin-related commands
- joinMod.h
- showMod.c                      ->Functions for handling show-related commands
- showMod.h
- userInterface.c                ->Main user interface functions and command parsing
- userInterface.h
