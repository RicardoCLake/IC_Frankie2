#include "tomo.h"


FILE *popen( const char *command, const char *mode );
int pclose( FILE *stream );

/* esta funcao executa meu_script em bash e retorna um int resultante

 Esta funcao retorna -1 se nao for encontrado nenhum valor
 Esta funcao retorna 0 se tudo correr bem

 ATENCAO: este script retorna apenas um int!!!!!*/

int roda_script_e_pega_result_int( char *meu_script, int *resultado ) {
  int temp = 0, result;
  FILE *fp;

  fp = popen( meu_script, "r" );
  temp = fscanf( fp, "%d", &result );
  fclose( fp );

  if ( temp == -1 ) /*se nao encontrou nenhum resultado*/
    return -1;
  else {
    ( *resultado ) = result;
    return 0;
  }
}

/* esta funcao executa meu_script em bash e retorna um unsigned int resultante

 Esta funcao retorna -1 se nao for encontrado nenhum valor
 Esta funcao retorna 0 se tudo correr bem

 ATENCAO: este script retorna apenas um int!!!!!*/

int roda_script_e_pega_result_unsigned_int( char *meu_script, unsigned int *resultado ) {
  int temp = 0;
  unsigned int result;
  FILE *fp;

  fp = popen( meu_script, "r" );
  temp = fscanf( fp, "%u", &result );
  fclose( fp );

  if ( temp == -1 ) /*se nao encontrou nenhum resultado*/
    return -1;
  else {
    ( *resultado ) = result;
    return 0;
  }
}

/* esta funcao executa meu_script em bash e retorna o double resultante

 Esta funcao retorna -1 se nao for encontrado nenhum valor
 Esta funcao retorna 0 se tudo correr bem

 ATENCAO: este script retorna apenas um double!!!!!*/

int roda_script_e_pega_result_double( char *meu_script, double *resultado ) {
  int temp = 0;
  double result;
  FILE *fp;

  fp = popen( meu_script, "r" );
  temp = fscanf( fp, "%lf", &result );
  fclose( fp );

  if ( temp == -1 ) /*se nao encontrou nenhum resultado*/
    return -1;
  else {
    ( *resultado ) = result;
    return 0;
  }
}

/* esta funcao executa meu_script em bash e retorna o string resultante

 Esta funcao retorna -1 se nao for encontrado nenhum valor
 Esta funcao retorna 0 se tudo correr bem */

int roda_script_e_pega_result_string( char *meu_script, char *saida ) {
  int temp = 0;
  char result[M_TEXT_MAX];
  FILE *fp;

  fp = popen( meu_script, "r" );
  temp = fscanf( fp, "%s", result );
  fclose( fp );

  if ( temp == -1 ) /*se nao encontrou nenhum resultado*/
    return -1;
  else {
    sprintf( saida, "%s", result );
    return 0;
  }
}
