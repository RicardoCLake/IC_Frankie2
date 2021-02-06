#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <argp.h>  /* for argument processing at the beginning*/
#include <math.h>
#include <pthread.h>

#include <sys/types.h>
#include <sys/poll.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/stat.h>


#include <sys/io.h>   /* for the paralel port control */
#include <sys/resource.h>   /* to set process priority*/

#include <termios.h>

#define M_TEXT_MAX 500

#define userID  1000   /* this will be used to set the file permissions. Check on your system with:   cat /etc/passwd   */
#define groupID  1000   /* this will be used to set the file permissions. Check on your system with:   cat /etc/passwd   */

/* struct for configuring plplot*/
/*struct conf_plplot {
  int n_graf_x;
  int n_graf_y;
  PLFLT *dados_x;
  PLFLT **dados_y;
  int NumberOfPoints;
  int NumberOfLines;
};*/


struct inputArg {
  char *output_file;
  char *conf_file;
  int MODE_TOMO;
  int MODE_COND;
  int MODE_OSC;
  int MODE_TOMO_EXP;
};


void CloseParallelPort();

int InitParallelPort();

int kbhit();

int SetChannels( int channel_plus, int channel_minus );

int SetCurrPatternSkip( int skip, int present_pattern ); 

int waitKeyboard( void );

int read_parameter_int( char *tag_parametro, char *arquivo );

double read_parameter_double( char *tag_parametro, char *arquivo );

void read_parameter_string( char *tag_parametro, char *arquivo, char *parametro );

int roda_script_e_pega_result_double( char *meu_script, double *resultado );
int roda_script_e_pega_result_int( char *meu_script, int *resultado );
int roda_script_e_pega_result_unsigned_int( char *meu_script, unsigned int *resultado );
int roda_script_e_pega_result_string( char *meu_script, char *saida );

