#include "tomo.h"

#define ENAB LP_PIN01
#define ENCD LP_PIN02
#define ENALL LP_PIN03
#define ENMS LP_PIN04

/* for MUX A and B   +I */

#define A0 LP_PIN05
#define A1 LP_PIN06
#define A2 LP_PIN07
#define A3 LP_PIN08

/* for MUX C and D   -I (bipolar) or GND (monopolar) */

#define A4 LP_PIN09
#define A5 LP_PIN14
#define A6 LP_PIN16
#define A7 LP_PIN17

static const int MUXA[] = {A0, A1, A2, A3};
static const int MUXB[] = {A0, A1, A2, A3};
static const int MUXC[] = {A4, A5, A6, A7};
static const int MUXD[] = {A4, A5, A6, A7};

/* set of controlable pins */

#define MY_PINS (ENAB | ENCD | ENMS | ENALL | A0 | A1 | A2 | A3 | A4 | A5 | A6 | A7)

/* check bit 'pos' whether it is 1 ou 0. ATENTION: pos starts in 0 */

#define CHECK_BIT(var,pos) ((var) & (1<<(pos))) /*this function takes the number designating the channel desired and helps convert to binary in the SetChannel function below*/

/* Initializes the parallel port. Returns -1 in case of error and 0 if everything goes well */
int InitParallelPort() {

  if ( pin_init_user( LPT1 ) < 0 ) {
    printf( "Couldn't get parallel port\n" );
    return -1;
  }
  /* set ports to output */
  pin_output_mode( MY_PINS );
  clear_pin( MY_PINS );
  return 0;
}

/* Closes the parallel port */
void CloseParallelPort() {
  clear_pin( MY_PINS );
}

/*
 activates the i-th current pattern on the muxes

 channel_plus: is the channel number for +I  (MUXA e MUXB)
 channel_minus: is the channel number for -I or GND  (MUXC e MUXD)

 ATTENTION! IT MUST BE A NUMBER BETWEEN 1 and 32

*/

int SetChannels( int channel_plus, int channel_minus ) {
  int i;
  /* turn off all multiplexers and the measurement selector pin*/
  clear_pin( ENAB | ENCD | ENALL ); /* JAN 24 MICHELLE Removed ENMS from this command*/
  set_pin( ENMS ); /* ADDED BY MICHELLE*/

  /*  mux for +I           (mux A and B)*/
  if ( channel_plus <= 16 ) {
    for ( i = 0; i < 4; i++ ) {
      if ( CHECK_BIT( channel_plus - 1 , i ) )
        set_pin( MUXA[i] );
      else
        clear_pin( MUXA[i] );
    }
    set_pin( ENAB );
  }
  else {
    for ( i = 0; i < 4; i++ ) {
      if ( CHECK_BIT( channel_plus -1 , i ) )
        set_pin( MUXB[i] );
      else
        clear_pin( MUXB[i] );
    }
    clear_pin( ENAB );
  }

  /* mux for -I ou GND      (mux C and D)*/
  if ( channel_minus <= 16 ) {
    for ( i = 0; i < 4; i++ ) {
      if ( CHECK_BIT( channel_minus -1 , i ) )
        set_pin( MUXC[i] );
      else
        clear_pin( MUXC[i] );
    }
    set_pin( ENCD );
  }
  else {
    for ( i = 0; i < 4; i++ ) {
      if ( CHECK_BIT( channel_minus -1 , i ) )
        set_pin( MUXD[i] );
      else
        clear_pin( MUXD[i] );
    }
    clear_pin( ENCD );
  }

   /* Turn on the muxes in the end to ensure that both current sources are turned on at the same time*/
  
  set_pin( ENALL); 

  /* Turn on the muxes in the end to ensure that both current sources are turned on at the same time*/
  
/* printf( "IN on %ldth electrode\n", channel_plus ); 
 printf( "OUT on %ldth electrode\n", channel_minus  ); 
 printf( "Press Enter to Change the current pattern\n"); 
 waitKeyboard(); */

  return 0;
}

/*
 Sets the current pattern Skip-N

 skip: is a number between 0 e 15
 present_pattern: is a number between 1 and 32
*/

int SetCurrPatternSkip( int skip, int present_pattern ) {
  int I_plus, I_minus;

  if ( present_pattern > 32 || present_pattern < 1 ) {
    printf( "ERROR: current pattern must be a number between (1 and 32).\n" ) ;
    return -1;
  }
  if ( skip < 0 || skip > 15 ) {
    printf( "ERRO: skip out of limits. Must be a number between (0 and 15).\n" ) ;
    return -1;
  }

  I_plus = present_pattern;

  I_minus = ( present_pattern + skip ) % 32 + 1; /* % means 'remainder of integer division' example:  5%4 is equivalent to   mod (5,4)=1*/

  if ( I_plus == I_minus ) {
    printf( "ERRO: injecting channel +I can not be the same as -I!\n" );
    return -1;
  }

  SetChannels( I_plus, I_minus );

  return 0;

 }
