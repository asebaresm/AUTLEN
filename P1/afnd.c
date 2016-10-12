#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "afnd.h"

AFND * AFNDNuevo(char * nombre, int num_estados, int num_simbolos){
	AFND *afnd = NULL;

	if (nombre == NULL || num_estados == 0 || num_simbolos == 0) {
		return NULL;
	}	
	
	afnd = (AFND*) malloc (sizeof(AFND));
	afnd->nombre = (char*) malloc (strlen(nombre));
	afnd->num_estados = num_estados;
	afnd->i_estados = 0;
	afnd->i_simbolos = 0;
	afnd->i_trans = NOT_INIT;
	afnd->num_simbolos = num_simbolos;
	afnd->simbolos = (char*) malloc (num_simbolos + 1);
	memset(afnd->simbolos, 0, num_simbolos);	
	afnd->cadena_entrada = NULL;
	afnd->estados = (Estado **) malloc(num_estados * sizeof(Estado *));
	afnd->transiciones = NULL;
	afnd->estados_actuales = NULL;
	printf("AFND nuevo creado\n");
	return afnd;
}

void AFNDElimina(AFND * p_afnd){
	int i=0;	
	for (i=0; i<p_afnd->num_estados; i++){
		liberarEstado(p_afnd->estados[i]);
	}
	free(p_afnd->estados);
	free(p_afnd->nombre);
	free(p_afnd->simbolos);
	


	/*Completar!!!!!*/

	free(p_afnd);
}

void AFNDImprime(FILE * fd, AFND* p_afnd){

}

AFND * AFNDInsertaSimbolo(AFND * p_afnd, char * simbolo){		
	
	if (p_afnd->i_simbolos == p_afnd->num_simbolos){
		return NULL;	
	}
	
	p_afnd->simbolos[p_afnd->i_simbolos] = *simbolo;
	p_afnd->i_simbolos++;
	printf("Simbolo nuevo creado\n");
	
	return p_afnd;
}

AFND * AFNDInsertaEstado(AFND * p_afnd, char * nombre, int tipo){
	if (p_afnd->i_estados == p_afnd->num_estados){
		return NULL;	
	}

	p_afnd->estados[p_afnd->i_estados] = nuevoEstado(nombre, tipo);
	p_afnd->i_estados++;
	printf("Estado nuevo creado\n");
	return p_afnd;
}

BOOL isSimbolo(AFND *p_afnd, char id){
	if (strchr(p_afnd->simbolos, id) != NULL){
		return TRUE;
	}
	return FALSE;
}

BOOL isEstado(AFND *p_afnd, char *id){
	int i=0;	
	for (i=0; i<p_afnd->num_estados; i++){
		if (strcmp(id, p_afnd->estados[i]->n) == 0){
			return TRUE;
		}
	}
	return FALSE;
}

BOOL isTransicion(AFND *p_afnd, char *id){
	int i=0;
	char * id = NULL:
	for(i=0; i<p_afnd->i_trans; i++){
		int i_t = p_afnd->i_trans;
		id = (char *) malloc (strlen(p_afnd->transiciones[i_t]) + strlen(nombre_simbolo_entrada) + strlen(nombre_estado_f) + 1);
		if 
	}
}

/*En la propia funcion se genera el id, se busca en el array de transiciones y se crea la transicion*/
AFND * AFNDInsertaTransicion(AFND * p_afnd, 
                             char * nombre_estado_i, 
                             char * nombre_simbolo_entrada, 
                             char * nombre_estado_f ){
		
	char * id = (char *) malloc (strlen(nombre_estado_i) + strlen(nombre_simbolo_entrada) + strlen(nombre_estado_f) + 1);
	strcpy(id, nombre_estado_i);
	strcat(id, nombre_simbolo_entrada);
	strcat(id, nombre_estado_f);
	if (isEstado(p_afnd, nombre_estado_i) == FALSE 
		|| isEstado(p_afnd, nombre_estado_f) == FALSE 
		|| isSimbolo(p_afnd, nombre_simbolo_entrada) == FALSE
		|| isTransicion(p_afnd, id) == TRUE ){
		return NULL;
	}
	if()

	free(id);
	return p_afnd;
}

AFND * AFNDInsertaLetra(AFND * p_afnd, char * letra){

	return p_afnd;
}

void AFNDImprimeConjuntoEstadosActual(FILE * fd, AFND * p_afnd){

}

void AFNDImprimeCadenaActual(FILE *fd, AFND * p_afnd){

}

AFND * AFNDInicializaEstado (AFND * p_afnd){

	return p_afnd;
}

void AFNDProcesaEntrada(FILE * fd, AFND * p_afnd){

}

void AFNDTransita(AFND * p_afnd){

}

/*Devuelve el estado inicial de uan transicion a partir de su indice*/
Estado * getIniTrans(AFND *p_afnd, int idx){
	int i = 0;
	if(p_afnd == NULL){
		return NULL;
	}
	return getIni(p_afnd->transiciones[i]);
}

/*Devuelve el estado inicial de uan transicion a partir de su indice*/
Estado * getFinalTrans(AFND *p_afnd, int idx){
	int i = 0;
	if(p_afnd == NULL){
		return NULL;
	}
	return getFinal(p_afnd->transiciones[i]);
}