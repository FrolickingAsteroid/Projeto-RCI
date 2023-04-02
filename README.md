# Projeto-RCI

[Transferência de Conteúdos em Redes Arbóreas](https://github.com/FrolickingAsteroid/Projeto-RCI/blob/main/Books/ConteudosArvore-1.pdf) is a project that focuses on facilitating the pseudo transfer of content items in tree-based networks. It provides a set of tools and utilities for managing content within nodes in a network, allowing users to create, delete, and search for content items.


# Features

* Support for creating and deleting content items
* Dynamic joining and leaving of nodes within the network
* Displaying network topology, content items, and forwarding tables
* Robust color coded error handling and reporting
* Interactive user interface with a comprehensive set of commands


# Getting Started
To compile the project, navigate to the `Projeto` directory and run the make command:
```sh
cd Projeto
make
```
# Running the Application

Once compiled, run the `cot` executable to launch the application:
```sh
./cot IPv4 TCP regIP regUDP
```
Replace IPv4, TCP, regIP, and regUDP with appropriate values for your specific use case.


# Example
Here is an example of running the cot application with sample arguments:
```sh
./cot 192.168.1.100 56000 192.168.1.101 59000
```
In this example, the application will be launched with the following parameters:

* **IPv4 address**: 192.168.1.100
* **TCP port**: 56000
* **Registration server IP**: 192.168.1.101
* **Registration server UDP port**: 59000

regIP and regUDP may be ommited and will default to values 193.136.138.142 and 59000, respectively

# User Interface
The user interface provides an interactive command-line interface for managing nodes and content items within the network. To access the interface, run the `cot` executable.

The interface supports various commands for network and content item management, including:

| Command                      | Description                                                   |
|------------------------------|---------------------------------------------------------------|
| `join <net> <id>`            | Registers a node with the given identifier to the network.    |
| `djoin <net> <id> <bootid> <bootIP> <bootTCP>` | Adds a node with the given identifier to the network, using the provided boot node for initial connection. |
| `create <name>`              | Creates a content item with the given name (alphanumeric and '.'). |
| `delete <name>`              | Deletes the content item with the given name (must exist in host). |
| `get <dest> <name>`          | Searches for the content item with the given name on the specified destination node. |
| `show topology (st)`         | Displays the identifiers and contacts of the internal neighbors, external neighbor, and recovery neighbor. |
| `show names (sn)`            | Displays the names of content items present on the node.      |
| `show routing (sr)`          | Displays the node's forwarding table.                         |
| `clear routing (cr)`         | Clears the node's forwarding table.                           |
| `clear names (cn)`           | Clears the names of content items present on the node.        |
| `leave`                      | Exits the node from the network.                              |
| `exit`                       | Closes the application.                                       |
| `verbose`                    | Enables verbose mode, displaying additional information during program execution. |
| `quiet`                      | Enables quiet mode, suppressing non-critical output.          |
| `clr`                        | Clears the terminal screen.                                   |
