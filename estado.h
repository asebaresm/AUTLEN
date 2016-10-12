#ifndef ESTADO_H
#define ESTADO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {INICIAL, NORMAL, FINAL, INICIAL_Y_FINAL} TIPO;

typedef struct Estados {
	char * n;
	TIPO t;
} Estado;

Estado * nuevoEstado (char * nombre, TIPO tipo);
void liberarEstado(Estado *estado);

#endif