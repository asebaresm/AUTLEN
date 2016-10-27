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
	for (i = 0; i < m->tam; i++){
		for (j = 0; j < m->tam; j++){
			if (m->matrix[i][j] == 1) {
				for (k = 0; k < m->tam; k++){
					if (m->matrix[k][i] == 1{
						insertaL(m, k, j);
					}
				} 	
			}
		}	
	}
}
