.
└── src                           - Source code directory
    ├── Common                    - Directory containing common utility functions
    │   ├── checkInvocationInfo.c - Implementation of invocation information checking
    │   ├── checkInvocationInfo.h - Header for invocation information checking
    │   ├── formatChecking.c      - Implementation of format checking functions
    │   ├── formatChecking.h      - Header for format checking functions
    │   ├── retry.c               - Implementation of retry logic
    │   ├── retry.h               - Header for retry logic
    │   ├── utils.c               - Implementation of utility functions
    │   └── utils.h               - Header for utility functions
    ├── HostStruct                - Directory containing data structures for the host
    │   ├── CirBuffer.c           - Implementation of circular buffer
    │   ├── CirBuffer.h           - Header for circular buffer
    │   ├── forwardingTable.c     - Implementation of forwarding table
    │   ├── forwardingTable.h     - Header for forwarding table
    │   ├── Name.c                - Implementation of name-related functions
    │   ├── Name.h                - Header for name-related functions
    │   ├── ncQueue.c             - Implementation of new connection queue
    │   ├── nodeStructure.c       - Implementation of node structure
    │   └── nodeStructure.h       - Header for node structure
    ├── main.c                    - Main source file
    ├── Protocols                 - Directory containing protocol implementations
    │   ├── eventManager.c        - Implementation of event manager
    │   ├── eventManager.h        - Header for event manager
    │   ├── TCP.c                 - Implementation of TCP protocol
    │   ├── TCP.h                 - Header for TCP protocol
    │   ├── UDP.c                 - Implementation of UDP protocol
    │   └── UDP.h                 - Header for UDP protocol
    ├── SocketProcessing          - Directory containing socket processing functions
    │   ├── newMod.c              - Implementation of new connection module
    │   ├── newMod.h              - Header for new connection module
    │   ├── queryMod.c            - Implementation of query module
    │   ├── queryMod.h            - Header for query module
    │   ├── socketInterface.c     - Implementation of socket interface
    │   ├── socketInterface.h     - Header for socket interface
    │   ├── withdrawMod.c         - Implementation of withdraw module
    │   └── withdrawMod.h         - Header for withdraw module
    └── UserInterface             - Directory containing user interface functions
        ├── exitMod.c             - Implementation of exit module
        ├── exitMod.h             - Header for exit module
        ├── getMod.c              - Implementation of get module
        ├── getMod.h              - Header for get module
        ├── joinMod.c             - Implementation of join module
        ├── joinMod.h             - Header for join module
        ├── showMod.c             - Implementation of show module
        ├── showMod.h             - Header for show module
        ├── userInterface.c       - Implementation of user interface
        └── userInterface.h       - Header for user interface

9 directories, 48 files
