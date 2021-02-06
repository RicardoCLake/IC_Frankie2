#include "tomo.h"

int read_parameter_int( char *tag_parametro, char *arquivo ) {
  char comando[M_TEXT_MAX];
  int resultado;
  sprintf( comando, "less %s | cut -f1 -d#  | sed 's| ||g' | grep '^%s=' | cut -f2 -d= | cut -f1 -d\\ ", arquivo , tag_parametro);
  roda_script_e_pega_result_int( comando, &resultado );
  return resultado;
}

double read_parameter_double( char *tag_parametro, char *arquivo ) {
  char comando[M_TEXT_MAX];
  double resultado;
  sprintf( comando, "less %s | cut -f1 -d#  | sed 's| ||g' | grep '^%s=' | cut -f2 -d= | cut -f1 -d\\ ", arquivo , tag_parametro);
  roda_script_e_pega_result_double( comando, &resultado );
  return resultado;
}

void read_parameter_string( char *tag_parametro, char *arquivo, char *parametro ) {
  char comando[M_TEXT_MAX];
  sprintf( comando, "less %s | cut -f1 -d#  | sed 's| ||g' | grep '^%s=' | cut -f2 -d= | cut -f1 -d\\ ", arquivo , tag_parametro);
  roda_script_e_pega_result_string( comando, parametro );
}
