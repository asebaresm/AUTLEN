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

Relaciones *iniMatrix(int t);

void insertaL(Relaciones *m, int row, int col);

void liberaMatrix(Relaciones *m);

void cierreReflex(Relaciones *m);

void cierreTransit(Relaciones *m);

Relaciones * reallocMatrix(Relaciones *m, int n);

int getTam(Relaciones *m);

int getMatrixData(Relaciones *m, int r, int c);

void cierreTransitAFND1O(Relaciones *c, Relaciones *p, Relaciones *r, int * mas);

void HookClose(int * a, int k, Relaciones * c, Relaciones *p, Relaciones * r, int * mas);

int isInArray(int * a, int k, int n);
#endif
