#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "relaciones.h"

Relaciones *iniMatrix(int t){
	Relaciones *m;
	int i = 0;
	m = (Relaciones *) malloc(sizeof(Relaciones));
	m->matrix = (int **) malloc(t * sizeof(int *));
	for (i = 0; i<t; i++){
		m->matrix[i] = (int *) malloc(t * sizeof(int));
		memset(m->matrix[i], 0, (t * sizeof(int)));
	}
	m->tam = t;
	if (m!= NULL){

		return m;
	} else {
		return NULL;
	}
}

void insertaL(Relaciones *m, int row, int col){
	m->matrix[row][col] = 1;
}

void insertaCero(Relaciones *m, int row, int col){
	m->matrix[row][col] = 0;
}

void liberaMatrix(Relaciones *m){
	int i = 0;
	for (i = 0; i<m->tam; i++){
		free(m->matrix[i]);
	}
	free(m->matrix);
	free(m);
}

void cierreReflex(Relaciones *m){
	int i = 0;
	for (i = 0; i<m->tam; i++){
		insertaL(m, i, i);
	}
}

void cierreTransit(Relaciones *m){
	int i = 0;
	int j = 0;
	int k = 0;
	for (i = 0; i < m->tam; i++) {
		for (j = 0; j < m->tam; j++){
			if (m->matrix[i][j] == 1) {
				for (k = 0; k < m->tam; k++){
					if (m->matrix[k][i] == 1) {
						insertaL(m, k, j);
					}
				} 	
			}
		}	
	}
}
Relaciones * reallocMatrix(Relaciones *m, int n){
	int i = 0;
	if (m == NULL ||n < 1){
		return NULL;
	}
	m->matrix = (int **) realloc(m->matrix, ((m->tam + n) * sizeof(int*)));
	/*Realloc de nuevos datos en cada fila*/
	for (i = 0; i< m->tam; i++){
		m->matrix[i] = (int *) realloc(m->matrix[i] , (m->tam + n) * sizeof(int));
		memset(m->matrix[i] + m->tam, 0, (n * sizeof(int)));
	}
	/*Realloc de nuevas filas*/	
	for (i = m->tam; i< m->tam + n; i++){
		m->matrix[i] = (int *) realloc(m->matrix[i] , (m->tam + n) * sizeof(int));
		memset(m->matrix[i], 0, ((m->tam + n) * sizeof(int)));
	}

	m->tam += n;
	return m;
}

int getTam(Relaciones *m){
	return m->tam;
}

int getMatrixData(Relaciones *m, int r, int c){
	return m->matrix[r][c];
}

void cierreTransitAFND1O(Relaciones *c, Relaciones *p, Relaciones *r, int * mas){
	int i , j;
	int k = 0;
	int * array;
	if (c == NULL || p == NULL || r == NULL){
		return;
	}
	array = (int*) malloc(c->tam * sizeof(int));
	memset(array, 0, (c->tam * sizeof(int)));
	for (i = 0; i < c->tam; i++) {
		for (j = 0; j < c->tam; j++){
			if (r->matrix[i][j] == 1) {
				insertaL(c, i, j);
				array[0] = i;
				array[1] = j;
				HookClose(array, 1, c, p, r, &k);
				if (k > *mas){
					*mas = k;
				}
                                k = 0;
			}
		}	
	}
	free(array);
}

/*Algoritmo recursivo para calcular todos los cierres de transiciones lambda,
almacenando las ids de los estados previos conectados en un array*/
void HookClose(int * a, int k, Relaciones * c, Relaciones * p, Relaciones * r, int * mas){
	int i, j;
	if (c == NULL || p==NULL || a == NULL || r == NULL || k < 1){
		return;
	}
	for (i = 0; i < c->tam; i++){
		if (r->matrix[a[k]][i] == 1) {
			/*Potencia i=1... por ejemplo*/
			insertaL(p, a[k-1], i);
			for (j = 0; j <= k; j++){
				insertaL(c, a[j], i);
			}
			/*Control de posibles errores*/
			if (isInArray(a, k, i) == 0) {
				/*Cierre estrella*/
				if (a[k-1] == i){
					continue;
				/*Bucle de lambdas*/
				} else {
					return;
				}
			}			
			a[k + 1] = i;
			HookClose(a, k + 1, c, p, r, mas);
		}
	}
	(*mas)++;
}

int isInArray(int * a, int k, int n){
	int i;
	if (a == NULL || n < 0 || k < 1) {
		return -1;
	}
	for (i = 0; i <= k; i++) {
		if (a[i] == n){
			return 0;
		}
	}
	return -1;
}
