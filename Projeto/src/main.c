#include <arpa/inet.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

#include "checkInvocationInfo.h"

int main(int argc, char *argv[]) {

  AddInvocInfo(argc, argv, usr_entries);
  InvocCheck(argc, argv);

  // Main loop
  // printf(">>> ");
  // fflush(stdout);
  // while (1){

  /*! TODO: Select
   *
   * Implement selet for parser
   */

  /*! TODO: Parser
   *
   * Implement select for parser
   */
  //}
  return EXIT_SUCCESS;
}
