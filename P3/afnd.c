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
	afnd->nombre = (char*) malloc (strlen(nombre) + 1);
	strcpy(afnd->nombre, nombre);
	afnd->num_estados = num_estados;
	afnd->i_estados = 0;
	afnd->i_simbolos = 0;
	afnd->i_trans = 0;
	afnd->num_trans = 0;
	afnd->num_simbolos = num_simbolos;
	afnd->simbolos = (char*) malloc (sizeof(char) * (num_simbolos + 1));
	memset(afnd->simbolos, 0, (size_t)num_simbolos);
	afnd->cadena_entrada = NULL;
	afnd->i_cadena = 0;
	afnd->estados = (Estado **) malloc(num_estados * sizeof(Estado *));
	afnd->transiciones = NULL;
	afnd->estados_actuales = (Estado **) malloc(num_estados * sizeof(Estado *));
	afnd->num_eactuales = 0;
	afnd->lambdatrix = iniMatrix(num_estados);
	return afnd;
}

void AFNDElimina(AFND * p_afnd){
    liberarEstadosAFND(p_afnd);
    liberarTransicionesANFD(p_afnd);
	free(p_afnd->estados_actuales);
	free(p_afnd->nombre);
	if (p_afnd->cadena_entrada != NULL){
		free(p_afnd->cadena_entrada);
		
	}
	free(p_afnd->simbolos);
	liberaMatrix(p_afnd->lambdatrix);
	free(p_afnd);
}

void liberarEstadosAFND(AFND *p_afnd){
    int i=0;
    for (i=0; i<p_afnd->num_estados; i++){
        liberarEstado(p_afnd->estados[i]);
    }
    free(p_afnd->estados);
}

void liberarTransicionesANFD(AFND *p_afnd){
    int i=0;
    for (i=0; i<p_afnd->num_trans; i++){
        liberarTransicion(p_afnd->transiciones[i]);
    }
    free(p_afnd->transiciones);
}

void AFNDImprime(FILE * fd, AFND* p_afnd){
	fprintf(fd, "%s={\n", p_afnd->nombre);
	fprintf(fd, "\tnum_simbolos = %d\n\n\n", p_afnd->num_simbolos);
	AFNDImprimeSimbolo(fd, p_afnd);
	fprintf(fd, "\tnum_estados = %d\n\n", p_afnd->num_estados);
	AFNDImprimeConjuntoEstadosTotal(fd, p_afnd);
	AFNDImprimeMatrix(fd, p_afnd);
	AFNDImprimeTransiciones(fd, p_afnd);
	fprintf(fd, "}\n");
}

void AFNDImprimeSimbolo(FILE * fd, AFND* p_afnd){
	int i = 0;	
	fprintf(fd, "\tA={ ");
	for (i = 0; i < p_afnd->num_simbolos; i++) {
		fprintf(fd, "%c ", p_afnd->simbolos[i]);
	}
	fprintf(fd, "}\n\n\n");	
}

AFND * AFNDInsertaSimbolo(AFND * p_afnd, char * simbolo){		
	
	if (p_afnd->i_simbolos == p_afnd->num_simbolos || strchr (p_afnd->simbolos, simbolo[0]) != NULL){
		return NULL;	
	}
	
	p_afnd->simbolos[p_afnd->i_simbolos] = *simbolo;
	p_afnd->i_simbolos++;	
	return p_afnd;
}

AFND * AFNDInsertaEstado(AFND * p_afnd, char * nombre, int tipo){
	if (p_afnd->i_estados == p_afnd->num_estados || isEstado(p_afnd, nombre) == TRUE){
		return NULL;	
	}

	p_afnd->estados[p_afnd->i_estados] = nuevoEstado(nombre, tipo, p_afnd->i_estados);
	p_afnd->i_estados++;
	return p_afnd;
}

BOOL isSimbolo(AFND *p_afnd, char *id){
	char c = id[0];
	if (strchr(p_afnd->simbolos, c) != NULL){
		return TRUE;
	}
	return FALSE;
}

BOOL isEstado(AFND *p_afnd, char *id){
	int i=0;
	for (i=0; i<p_afnd->i_estados; i++){
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

	/*ret NULL si los estados no existen OR la transicion ya existe en el AFND*/
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

	/*TO-DO (opcional, si reusamos el automata): 
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
	if (isSimbolo(p_afnd, letra) == FALSE){
		return NULL;
	}
	if(p_afnd->i_cadena == 0){
		p_afnd->cadena_entrada = (char *) malloc(strlen(letra)+1);
		strcpy(p_afnd->cadena_entrada, letra);
	}else{
		p_afnd->cadena_entrada = (char *) realloc(p_afnd->cadena_entrada, strlen(p_afnd->cadena_entrada)+ strlen(letra) + 1);
		strcat(p_afnd->cadena_entrada, letra);
	}
	
	p_afnd->i_cadena++;
	return p_afnd;
}

void AFNDImprimeConjuntoEstadosActual(FILE * fd, AFND * p_afnd){
	int i = 0;
	fprintf(fd, "\nACTUALMENTE EN {");
	for (i = 0; i<p_afnd->num_eactuales; i++){
		if (getTipo(p_afnd->estados_actuales[i]) == INICIAL
		 || getTipo(p_afnd->estados_actuales[i]) == INICIAL_Y_FINAL){
			fprintf(fd, "->");
		}

		fprintf(fd, "%s", getNombre(p_afnd->estados_actuales[i]));

		if (getTipo(p_afnd->estados_actuales[i]) == FINAL
		 || getTipo(p_afnd->estados_actuales[i]) == INICIAL_Y_FINAL){
			fprintf(fd, "* ");
		} else {
			fprintf(fd, " ");
		}
	}
	fprintf(fd, "}\n");
}

void AFNDImprimeConjuntoEstadosTotal(FILE * fd, AFND * p_afnd){
	int i = 0;
	fprintf(fd, "\n\tQ={");
	for (i = 0; i<p_afnd->num_estados; i++){
		if (getTipo(p_afnd->estados[i]) == INICIAL
		 || getTipo(p_afnd->estados[i]) == INICIAL_Y_FINAL){
			fprintf(fd, "->");
		}

		fprintf(fd, "%s", getNombre(p_afnd->estados[i]));

		if (getTipo(p_afnd->estados[i]) == FINAL
		 || getTipo(p_afnd->estados[i]) == INICIAL_Y_FINAL){
			fprintf(fd, "* ");
		} else {
			fprintf(fd, " ");
		}
	}
	fprintf(fd, "}\n\n\n");
}

void AFNDImprimeTransiciones(FILE *fd, AFND *p_afnd){
	int i = 0;
	int j = 0;
	int k = 0;
	char *aux;
	fprintf(fd,"\n\tFuncion de Transicion = {\n");
	for (i = 0;i < p_afnd->num_estados; i++){
		for (j = 0; j < p_afnd->num_simbolos; j++){
			fprintf(fd, "\t\tf(%s,%c)={ ", getNombre(p_afnd->estados[i]), p_afnd->simbolos[j]);
			for (k = 0; k < p_afnd->num_trans; k++) {
				aux = getSimbolo(p_afnd->transiciones[k]);
				if (getIni(p_afnd->transiciones[k]) == p_afnd->estados[i]
					&& aux[0] == p_afnd->simbolos[j]){
					fprintf(fd, "%s ", getNombre(getFinal(p_afnd->transiciones[k])));
				}
			}
			fprintf(fd, "}\n");
		}
	}
	fprintf(fd, "\t}\n");
}

void AFNDImprimeCadenaActual(FILE *fd, AFND * p_afnd){
	int i = 0;
	fprintf(fd, "[(%d)", p_afnd->i_cadena);
	if (p_afnd->i_cadena != 0){
		for (i = strlen(p_afnd->cadena_entrada) - p_afnd->i_cadena; i < strlen(p_afnd->cadena_entrada); i++) {
			fprintf(fd, " %c", p_afnd->cadena_entrada[i]);
		}
	}
	fprintf(fd, "]\n");
}

AFND * AFNDInicializaCadenaActual(AFND * p_afnd){
	if (p_afnd->cadena_entrada != NULL) {	
		free(p_afnd->cadena_entrada);
		p_afnd->cadena_entrada = NULL;
		p_afnd->i_cadena = 0;
	}
	return p_afnd;
}

AFND * AFNDInicializaEstado (AFND * p_afnd){
	int i = 0;
	for (i=0; i<p_afnd->num_eactuales; i++){
		p_afnd->estados_actuales[i] = NULL;
	}
	p_afnd->num_eactuales = 0;
	for (i = 0; i<p_afnd->num_estados; i++){
		if (getTipo(p_afnd->estados[i]) == INICIAL
		 || getTipo(p_afnd->estados[i]) == INICIAL_Y_FINAL){
			p_afnd->estados_actuales[p_afnd->num_eactuales] = p_afnd->estados[i];
			p_afnd->num_eactuales++;
		}
	}
	return p_afnd;
}

void AFNDProcesaEntrada(FILE * fd, AFND * p_afnd){
	int i = 0;
	int j;
	int k = p_afnd->i_cadena;
	if (p_afnd->cadena_entrada == NULL){
		return;
	}
	j = strlen(p_afnd->cadena_entrada);
	AFNDTransitaLambdas(p_afnd);
	for (i = 0; i < j; i++) {
        AFNDImprimeConjuntoEstadosActual(fd, p_afnd);
        AFNDImprimeCadenaActual(fd, p_afnd);
        if (p_afnd->num_eactuales == 0){
        	break;
        }
		AFNDTransita(p_afnd);
		AFNDTransitaLambdas(p_afnd);
		p_afnd->i_cadena--;
        	if(p_afnd->i_cadena == 0) {
            		AFNDImprimeConjuntoEstadosActual(fd, p_afnd);
            		AFNDImprimeCadenaActual(fd, p_afnd);
        	}	
	}
	p_afnd->i_cadena = k;
}

void AFNDTransita(AFND * p_afnd){

	int i = 0;
	int j = 0;
	int k = 0;
	Estado *p_echeck = NULL;
	Transicion *p_tcheck = NULL;
	Estado **aux = (Estado **) malloc(p_afnd->num_estados * sizeof(Estado *));
	for (i = 0; i < p_afnd->num_eactuales; i++) {
		p_echeck = p_afnd->estados_actuales[i];
		for (j = 0; j < p_afnd->num_trans; j++) {
			p_tcheck = p_afnd->transiciones[j];
			if ((getIni(p_tcheck) == p_echeck)
			    && (p_afnd->cadena_entrada[strlen(p_afnd->cadena_entrada) - p_afnd->i_cadena] == getSimbolo(p_tcheck)[0])
			    && (findEstado(aux, k, getFinal(p_tcheck)) == FALSE)) {
				aux[k] = getFinal(p_tcheck);
				k++;
			}
		}
	}
	p_afnd->num_eactuales = k;
	/*Copiamos en bloque al puntero del AFND el nuevo "array" de estados actuales*/
	memcpy(p_afnd->estados_actuales, aux, p_afnd->num_estados * sizeof(Estado *));
	free(aux);
}

void AFNDTransitaLambdas(AFND *p_afnd){
	int i = 0;
	int j = 0;
	int k = 0;
	Estado *p_echeck = NULL;
	Estado **aux = (Estado **) malloc(p_afnd->num_estados * sizeof(Estado *));
	for (i = 0; i < p_afnd->num_eactuales; i++){
		p_echeck = p_afnd->estados_actuales[i];
		for (j = 0; j < getTam(p_afnd->lambdatrix); j++){
			if (getMatrixData(p_afnd->lambdatrix, getId(p_echeck), j) == 1
				&& (findEstado(aux, k, getEstadoPorId(p_afnd->estados, p_afnd->num_estados, j)) == FALSE)){
				aux[k] = getEstadoPorId(p_afnd->estados, p_afnd->num_estados, j);
				k++;
			}
		}		
	}
	p_afnd->num_eactuales = k;
	/*Copiamos en bloque al puntero del AFND el nuevo "array" de estados actuales*/
	memcpy(p_afnd->estados_actuales, aux, p_afnd->num_estados * sizeof(Estado *));
	free(aux);
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


AFND * AFNDInsertaLTransicion(AFND * p_afnd, 
       			      char * nombre_estado_i, 
       			      char * nombre_estado_f ){
	
	int id_i, id_f;
	if (isEstado(p_afnd, nombre_estado_i) == FALSE 
		|| isEstado(p_afnd, nombre_estado_f) == FALSE) {
		return NULL;
	}
	id_i = getId(getEstadoAFND(p_afnd, nombre_estado_i));
	id_f = getId(getEstadoAFND(p_afnd, nombre_estado_f));
	insertaL(p_afnd->lambdatrix, id_i, id_f);
	return p_afnd;
}

AFND * AFNDCierraLTransicion (AFND * p_afnd){
	cierreReflex(p_afnd->lambdatrix);
	cierreTransit(p_afnd->lambdatrix);
	return p_afnd;
}

AFND * AFNDImprimeMatrix(FILE * fd, AFND* p_afnd){
	int i = 0;
	int j = 0;
	int t;
	if (p_afnd == NULL){
		return NULL;
	}
	t = getTam(p_afnd->lambdatrix);
	fprintf(fd, "\tRL++*={\n\n\t");
	for (i = 0; i < t; i++){
		fprintf(fd, "\t[%d]", i);
	}
	fprintf(fd, "\n");

	for (i = 0; i < t; i++){
		fprintf(fd, "\t[%d]", i);
		for (j = 0; j < t; j++){			
			fprintf(fd, "\t%d", getMatrixData(p_afnd->lambdatrix, i, j));
		}
		fprintf(fd, "\n");
	}
	fprintf(fd, "}\n");
	return p_afnd;
}

AFND * AFND1ODeSimbolo( char * simbolo){}

AFND * AFND1ODeLambda(){}

AFND * AFND1ODeVacio(){}

AFND * AFNDAAFND1O(AFND * p_afnd){}

AFND * AFND1OUne(AFND * p_afnd1O_1, AFND * p_afnd1O_2){}

AFND * AFND1OConcatena(AFND * p_afnd_origen1, AFND * p_afnd_origen2){}

AFND * AFND1OEstrella(AFND * p_afnd_origen){}

void AFNDADot(AFND * p_afnd){}

