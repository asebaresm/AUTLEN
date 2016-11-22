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
	int id;
} Estado;

Estado * nuevoEstado(char * nombre, TIPO tipo, int n);
void liberarEstado(Estado *estado);
char * getNombre(Estado *estado);
int getId(Estado *estado);
TIPO getTipo(Estado *estado);
char * tipoPrinteable(TIPO t, char *str);
BOOL equalEstado(Estado * e1, Estado *e2);
BOOL findEstado(Estado **lista_e, int tam, Estado *e);
Estado * getEstadoPorId(Estado **lista_e, int tam, int id);
void appendE(Estado **lista_e, int *tam, Estado *e);
#endif



