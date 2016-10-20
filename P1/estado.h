#ifndef ESTADO_H
#define ESTADO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {INICIAL, NORMAL, FINAL, INICIAL_Y_FINAL} TIPO;
typedef enum { FALSE = 0, TRUE = 1} BOOL;

typedef struct Estados {
	char * n;
	TIPO t;
} Estado;

Estado * nuevoEstado (char * nombre, TIPO tipo);
void liberarEstado(Estado *estado);
char * getNombre(Estado *estado);
TIPO getTipo(Estado *estado);
char * tipoPrinteable(TIPO t, char *str);
BOOL equalEstado(Estado * e1, Estado *e2);
#endif



