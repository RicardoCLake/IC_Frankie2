#include "tomo.h"

#define FLUSH_STDIN {do fgets(Junk,16,stdin); while(Junk[strlen(Junk)-1]!='\n');}  /* macro for flshing stdin correctly. */
char Junk[16]; // buffer for discarding excessive user input,  used by "FLUSH_STDIN" macro*/


int kbhit() {
  struct pollfd key_in;

  key_in.fd = 0;
  key_in.events = POLLIN;
  key_in.revents = 0;

  if ( 0 < poll( &key_in, 1, 0 ) ) {
    if ( key_in.revents & POLLIN ) {
      FLUSH_STDIN;   /*meu macro para limpar o stdin*/
      return 1;
    }
  }
  return 0;
}

