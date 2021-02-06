#include "tomo.h"

/*
 *  waitKeyboard()  --  a blocking single character input from stdin
 *
 *  Returns a character, or -1 if an input error occurs.
 *
 *  Conditionals allow compiling with or without echoing of
 *  the input characters, and with or without flushing pre-existing
 *  existing  buffered input before blocking.
 *
 */
int waitKeyboard( void ) {
  char                  ch;
  int                   error;
  static struct termios Otty, Ntty;

  fflush( stdout );
  tcgetattr( 0, &Otty );
  Ntty = Otty;

  Ntty.c_iflag  =  0;           /* input mode           */
  Ntty.c_oflag  =  0;           /* output mode          */
  Ntty.c_lflag &= ~ICANON;  /* line settings        */

#if 1
  /* disable echoing the char as it is typed */
  Ntty.c_lflag &= ~ECHO;    /* disable echo         */
#else
  /* enable echoing the char as it is typed */
  Ntty.c_lflag |=  ECHO;        /* enable echo          */
#endif

  Ntty.c_cc[VMIN]  = CMIN;      /* minimum time to wait */
  Ntty.c_cc[VTIME] = CTIME;     /* minimum chars to wait for */

#if 1
  /*
   * use this to flush the input buffer before blocking for new input
   */
#define FLAG TCSAFLUSH
#else
  /*
   * use this to return a char from the current input buffer, or block if
   * no input is waiting.
   */
#define FLAG TCSANOW

#endif

  if ( 0 == ( error = tcsetattr( 0, FLAG, &Ntty ) ) ) {
    error  = read( 0, &ch, 1 );               /* get char from stdin */
    error += tcsetattr( 0, FLAG, &Otty ); /* restore old settings */
  }

  return ( error == 1 ? ( int ) ch : -1 );

}
