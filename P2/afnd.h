#ifndef AFND_H
#define AFND_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "estado.h"
#include "transicion.h"
#include "relaciones.h"

#define DEBUG
#ifdef DEBUG
# define DEBUG_PRINT(x) printf x
#else
# define DEBUG_PRINT(x) do {} while (0)
#endif

typedef struct Afnds {
	char * nombre;
	int num_estados;
	int i_estados;
	int num_simbolos;
	int i_simbolos;
	int num_trans;
	int i_trans;
	char *simbolos;
	char *cadena_entrada;
	int i_cadena;
	Estado **estados;
	Transicion **transiciones;
	Estado **estados_actuales;
	int num_eactuales;
	Relaciones *lambdatrix;
} AFND;

AFND * AFNDNuevo(char * nombre, int num_estados, int num_simbolos);

void AFNDElimina(AFND * p_afnd);

void liberarEstadosAFND(AFND *p_afnd);

void liberarTransicionesANFD(AFND *p_afnd);

void AFNDImprime(FILE * fd, AFND* p_afnd);

void AFNDImprimeSimbolo(FILE * fd, AFND* p_afnd);

AFND * AFNDInsertaSimbolo(AFND * p_afnd, char * simbolo);

AFND * AFNDInsertaEstado(AFND * p_afnd, char * nombre, int tipo);

BOOL isSimbolo(AFND *p_afnd, char *id);

BOOL isEstado(AFND *p_afnd, char *id);

BOOL isTransicion(AFND *p_afnd, char*id);

/*devuelve la id de una transicion a partir de un indice del array de transiciones en el afnd*/
char * getTransIdAFND(AFND * p_afnd, int idx);



AFND * AFNDInsertaTransicion(AFND * p_afnd, 
                             char * nombre_estado_i, 
                             char * nombre_simbolo_entrada, 
                             char * nombre_estado_f );

AFND * AFNDInsertaLetra(AFND * p_afnd, char * letra);

void AFNDImprimeConjuntoEstadosActual(FILE * fd, AFND * p_afnd);

void AFNDImprimeConjuntoEstadosTotal(FILE * fd, AFND * p_afnd);

void AFNDImprimeTransiciones(FILE *fd, AFND *p_afnd);

void AFNDImprimeCadenaActual(FILE *fd, AFND * p_afnd);

AFND * AFNDInicializaCadenaActual(AFND * p_afnd);

AFND * AFNDInicializaEstado (AFND * p_afnd);

void AFNDProcesaEntrada(FILE * fd, AFND * p_afnd);

void AFNDTransita(AFND * p_afnd);

Estado * getIniTrans(AFND *p_afnd, int idx);

Estado * getFinalTrans(AFND *p_afnd, int idx);

Estado *getEstadoAFND(AFND *p_afnd, char *id);

BOOL findEstado(Estado **lista_e, int tam, Estado *e);

AFND * AFNDInsertaLTransicion(AFND * p_afnd, 
       			      char * nombre_estado_i, 
       			      char * nombre_estado_f );

AFND * AFNDCierraLTransicion (AFND * p_afnd);

AFND * AFNDImprimeMatrix(FILE * fd, AFND *p_afnd);

#endif
