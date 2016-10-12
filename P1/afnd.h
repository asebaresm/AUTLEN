#ifndef AFND_H
#define AFND_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "estado.h"
#include "transicion.h"

typedef enum { FALSE = 0, TRUE = 1} BOOL;

typedef struct Afnds {
	char * nombre;
	int num_estados;
	int i_estados;
	int num_simbolos;
	int i_simbolos;
	int i_trans;
	char *simbolos;
	char *cadena_entrada;
	Estado **estados;
	Transicion **transiciones;
	Estado *estados_actuales;
} AFND;

AFND * AFNDNuevo(char * nombre, int num_estados, int num_simbolos);

void AFNDElimina(AFND * p_afnd);

void AFNDImprime(FILE * fd, AFND* p_afnd);

AFND * AFNDInsertaSimbolo(AFND * p_afnd, char * simbolo);


AFND * AFNDInsertaEstado(AFND * p_afnd, char * nombre, int tipo);

BOOL isSimbolo(AFND *p_afnd, char id);

BOOL isEstado(AFND *p_afnd, char *id);

BOOL isTransicion(AFND *p_afnd, char*id);

AFND * AFNDInsertaTransicion(AFND * p_afnd, 
                             char * nombre_estado_i, 
                             char * nombre_simbolo_entrada, 
                             char * nombre_estado_f );

AFND * AFNDInsertaLetra(AFND * p_afnd, char * letra);

void AFNDImprimeConjuntoEstadosActual(FILE * fd, AFND * p_afnd);

void AFNDImprimeCadenaActual(FILE *fd, AFND * p_afnd);

AFND * AFNDInicializaEstado (AFND * p_afnd);

void AFNDProcesaEntrada(FILE * fd, AFND * p_afnd);

void AFNDTransita(AFND * p_afnd);

Estado * getIniTrans(AFND *p_afnd, int idx);

Estado * getFinalTrans(AFND *p_afnd, int idx);

#endif
