#ifndef TRANSICION_H
#define TRANSICION_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "estado.h"

#define NOT_INIT -1

typedef struct Transiciones{
	Estado *e_ini;
	Estado *e_fin;
	char *s;
	char *id;
}Transicion;

Transicion * nuevaTransicion(Estado *e_inicial, char *simbolo, Estado *e_final);
void liberarTransicion(Transicion *transicion);
Estado * getIni(Transicion *t);
Estado * getFinal(Transicion *t);
char *getSimbolo(Transicion * t);
char * buildTransId(char *nombre_estado_i, char *nombre_simbolo_entrada, char *nombre_estado_f);


#endif
