#ifndef RELACIONES_H
#define RELACIONES_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "estado.h"
#include "transicion.h"

#define DEBUG
#ifdef DEBUG
# define DEBUG_PRINT(x) printf x
#else
# define DEBUG_PRINT(x) do {} while (0)
#endif

typedef struct Rels {
	int **matrix;
	int tam;
} Relaciones;

Relaciones **iniMatrix(int t);

void insertaL(int row, int col);

void liberaMatrix(Relaciones m);

void cierreReflex(Relaciones m);

void cierreTransit(Relaciones m);

#endif
