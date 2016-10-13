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
	afnd->i_trans = 0;
	afnd->num_trans = 0;
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
	for (i=0; i<p_afnd->num_trans; i++){
		liberarTransicion(p_afnd->transiciones[i]);
	}
	free(p_afnd->estados);
	free(p_afnd->transiciones);
	free(p_afnd->nombre);
	free(p_afnd->simbolos);

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

BOOL isSimbolo(AFND *p_afnd, char *id){
	char c = id[0];
	/*printf("\nchar es %c\n", c);*/
	if (strchr(p_afnd->simbolos, c) != NULL){
		return TRUE;
	}
	return FALSE;
}

BOOL isEstado(AFND *p_afnd, char *id){
	int i=0;	
	for (i=0; i<p_afnd->num_estados; i++){
		/*printf("\ncomparando [%s] con [%s]\n", p_afnd->estados[i]->n, id);*/
		if (strcmp(p_afnd->estados[i]->n, id) == 0){
			return TRUE;
		}
	}
	return FALSE;
}

char * getTransIdAFND(AFND * p_afnd, int idx){
	char * id = NULL;
	id = (char *) malloc (strlen(getNombre(getIni(p_afnd->transiciones[idx]))) 
		                  + strlen(getSimbolo(p_afnd->transiciones[idx]))
		                  + strlen(getNombre(getFinal(p_afnd->transiciones[idx]))) 
		                  + 1);
	strcpy(id, getNombre(getIni(p_afnd->transiciones[idx])));
	strcat(id, getSimbolo(p_afnd->transiciones[idx]));
	strcat(id, getNombre(getFinal(p_afnd->transiciones[idx])));
	return id;
}

BOOL isTransicion(AFND *p_afnd, char *id){
	int i=0;
	char *check_id = NULL;

	for(i=0; i<p_afnd->i_trans; i++){
		check_id = getTransIdAFND(p_afnd,i);
		if(strcmp(id,check_id) == 0){
			free(check_id);
			return TRUE;
		}
		free(check_id);
	}
	return FALSE;
}

/*En la propia funcion se genera el id, se busca en el array de transiciones y se crea la transicion*/
AFND * AFNDInsertaTransicion(AFND * p_afnd, 
                             char * nombre_estado_i, 
                             char * nombre_simbolo_entrada, 
                             char * nombre_estado_f ){
		
	char *id = NULL;
	id = buildTransId(nombre_estado_i, nombre_simbolo_entrada, nombre_estado_f);

	/*ret NULL si los estados existen OR la transicion ya existe en el AFND*/
	if (isEstado(p_afnd, nombre_estado_i) == FALSE 
		|| isEstado(p_afnd, nombre_estado_f) == FALSE 
		|| isSimbolo(p_afnd, nombre_simbolo_entrada) == FALSE
		|| isTransicion(p_afnd, id) == TRUE ){
		free(id);
		return NULL;
	}

	if(p_afnd->num_trans == 0){
		p_afnd->transiciones = (Transicion **) malloc(sizeof(Transicion*));
	}else{
		p_afnd->transiciones = (Transicion **) realloc(p_afnd->transiciones, ((p_afnd->num_trans+1) * sizeof(Transicion*)));
	}
	p_afnd->num_trans++;

	/*TO-DO: 
		-empaquetar en funcion nuevaTransicion()
		-revisar secuencia de mallocs (simplificar)
		-comprobaciones de ret NULLs*/
	p_afnd->transiciones[p_afnd->i_trans] = (Transicion*) malloc(sizeof(Transicion));
	p_afnd->transiciones[p_afnd->i_trans]->e_ini = getEstadoAFND(p_afnd, nombre_estado_i);
	p_afnd->transiciones[p_afnd->i_trans]->s = (char*) malloc(strlen(nombre_simbolo_entrada) + 1);
	p_afnd->transiciones[p_afnd->i_trans]->id = (char*) malloc(strlen(id) + 1);
	strcpy(p_afnd->transiciones[p_afnd->i_trans]->s, nombre_simbolo_entrada);
	strcpy(p_afnd->transiciones[p_afnd->i_trans]->id, id);
	free(id);
	p_afnd->transiciones[p_afnd->i_trans]->e_fin = getEstadoAFND(p_afnd, nombre_estado_f);
	p_afnd->i_trans++;

	return p_afnd;
}

Estado *getEstadoAFND(AFND *p_afnd, char *id){
	int i=0;	
	for (i=0; i<p_afnd->num_estados; i++){
		if (strcmp(id, p_afnd->estados[i]->n) == 0){
			return p_afnd->estados[i];
		}
	}
	return NULL;
}

AFND * AFNDInsertaLetra(AFND * p_afnd, char * letra){
	
	return p_afnd;
}

void AFNDImprimeConjuntoEstadosActual(FILE * fd, AFND * p_afnd){

}

void AFNDImprimeConjuntoEstadosTotal(FILE * fd, AFND * p_afnd){
	int i = 0;
	char str[17];
	printf("\nEstados insertados en el AFND:");
	for(i=0; i<p_afnd->num_estados; i++){
		printf("\n\t(%s|%s)", p_afnd->estados[i]->n, tipoPrinteable(p_afnd->estados[i]->t,str));
	}
}

void AFNDImprimeTransiciones(FILE *fd, AFND *p_afnd){
	int i= 0;
	printf("\nTransiciones insertadas en el AFND:");
	for(i=0; i<p_afnd->num_trans; i++){
		printf("\n\t(%s|%s|%s) - id:%s", getNombre(p_afnd->transiciones[i]->e_ini), 
										 p_afnd->transiciones[i]->s, 
										 getNombre(p_afnd->transiciones[i]->e_fin),
										 p_afnd->transiciones[i]->id);
	}
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