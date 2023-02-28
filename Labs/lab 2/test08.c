#include <signal.h>

/* ... */

int main(void) {
  void (*old_handler) (int); // interrupt handler

  /* ... */

  if ((old_handler = signal(SIGPIPE, SIG_IGN)) == SIG_ERR) /*error*/ exit(1);

  /* ... */
}
