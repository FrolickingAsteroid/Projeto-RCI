#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "UDP.h"
#include "checkInvocationInfo.h"
#include "nodeStructure.h"
#include "userInterface.h"
#include "utils.h"

/**
 * @brief Parses User Input Command and calls processing functions
 *
 * @param buffer -> user string written to STDIN_NO
 */
void UserInterfaceParser(char buffer[], Host *HostNode) {
  char Command[5];
  char Net[4], Id[3];

  // Command = strtok(buffer, " ");

  sscanf(buffer, "%s %s %s", Command, Net, Id);

  if (strcmp(Command, "join") == 0) {
    JoinNetworkServer(buffer, HostNode);

  } else if (strcmp(Command, "djoin") == 0) {
    DJoinNetworkServer(buffer);

  } else {
    printf("Command not found: Invalid Command\n");
    return;
  }
}

void JoinNetworkServer(char buffer[], Host *HostNode) {
  char Net[4], Id[3];
  char *msg = calloc(16, sizeof(char));
  char *UDPAnswer = NULL;
  char Dummy[5];

  sscanf(buffer, "%s %s %s", Dummy, Net, Id);

  if ((IsNumber(Net) == 0) || (IsNumber(Id) == 0)) {
    printf("Command not found: Invalid Command Aqui\n");
    /*! TODO: InterfaceUsage
     *
     * @todo Retorna a lista de comandos após a colocação de argumentos
     * inválidos.
     */
    return;
  }
  // Pedir a NodeList -> NODES NET
  sprintf(msg, "NODES %s\n", Net);
  UDPAnswer = UDPClient(HostNode, msg);
  /*! TODO: return if timeout
   *
   * Esperar pelo NODELIST, caso contrário retornar e dar free(msg)
   * Verificar se o ID já se encontra registado --> HostNode
   * */

  /*! TODO: Escolher Externo -> DJoinNetworkServer*/

  // Overwrite Buffer for new message
  memset(msg, 0, strlen(msg));
  // Pedir registo na rede -> REG (...)
  sprintf(msg, "REG %s %s %s %d\n", Net, Id, HostNode->InvocInfo->HostIP,
          HostNode->InvocInfo->HostTCP);
  UDPAnswer = UDPClient(HostNode, msg);

  UDPAnswer = UDPAnswer;
}

void DJoinNetworkServer(char buffer[]) {}
