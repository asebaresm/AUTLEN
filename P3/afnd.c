#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "afnd.h"

AFND * AFNDNuevo(char * nombre, int num_estados, int num_simbolos){
	AFND *afnd = NULL;

	if (nombre == NULL || num_estados == 0 || num_simbolos < 0) {
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
	afnd->potencia_i = NULL;
	afnd->relacion_inicial_i = NULL;
	return afnd;
}

void AFNDElimina(AFND * p_afnd){
    liberarEstadosAFND(p_afnd);
    liberarTransicionesAFND(p_afnd);
	free(p_afnd->estados_actuales);
	free(p_afnd->nombre);
	if (p_afnd->cadena_entrada != NULL){
		free(p_afnd->cadena_entrada);		
	}
	free(p_afnd->simbolos);
	liberaMatrix(p_afnd->lambdatrix);
	if (p_afnd->potencia_i != NULL){
		liberaMatrix(p_afnd->potencia_i);
		liberaMatrix(p_afnd->relacion_inicial_i);
	}
	free(p_afnd);
}

void liberarEstadosAFND(AFND *p_afnd){
    int i=0;
    for (i=0; i<p_afnd->num_estados; i++){
        liberarEstado(p_afnd->estados[i]);
    }
    free(p_afnd->estados);
}

void liberarTransicionesAFND(AFND *p_afnd){
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
			fprintf(fd, "*  ");
		} else {
			fprintf(fd, "  ");
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
	if (p_afnd->potencia_i != NULL) {
		insertaL(p_afnd->relacion_inicial_i, id_i, id_f);	
	} else {
		insertaL(p_afnd->lambdatrix, id_i, id_f);
	}
	/*Insertar en potencia i o alguna de esas*/
	return p_afnd;
}

AFND * AFNDCierraLTransicion (AFND * p_afnd){
	if (p_afnd == NULL){
		return NULL;
	}
	if (p_afnd->potencia_i == NULL) {
		cierreTransit(p_afnd->lambdatrix);			
	} else {
		cierreTransitAFND1O(p_afnd->lambdatrix, p_afnd->potencia_i, p_afnd->relacion_inicial_i);
	}
	cierreReflex(p_afnd->lambdatrix);
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
	fprintf(fd, "\t\tCIERRE\n");
	
	for (i = 0; i < t; i++){
		fprintf(fd, "\t[%d]", i);
		for (j = 0; j < t; j++){			
			fprintf(fd, "\t%d", getMatrixData(p_afnd->lambdatrix, i, j));
		}
		fprintf(fd, "\n");
	}
	if (p_afnd->potencia_i != NULL){
		fprintf(fd, "\t\tPOTENCIA i\n");
		for (i = 0; i < t; i++){
			fprintf(fd, "\t[%d]", i);
			for (j = 0; j < t; j++){			
				fprintf(fd, "\t%d", getMatrixData(p_afnd->potencia_i, i, j));
			}
			fprintf(fd, "\n");
		}

		fprintf(fd, "\t\tRELACION INICIAL i\n");
		for (i = 0; i < t; i++){
			fprintf(fd, "\t[%d]", i);
			for (j = 0; j < t; j++){			
				fprintf(fd, "\t%d", getMatrixData(p_afnd->relacion_inicial_i, i, j));
			}
			fprintf(fd, "\n");
		}
	}
	fprintf(fd, "}\n");
	return p_afnd;
}

AFND * AFND1ODeSimbolo( char * simbolo){
	AFND * afnd = NULL;
	char * n_name;
	if (simbolo == NULL) {
		return NULL;
	}
	n_name = (char *) malloc ((strlen("afnd1o_") + strlen(simbolo) + 1) * sizeof(char));
	strcpy(n_name, "afnd1o_");
	strcat(n_name, simbolo);
	if ((afnd = AFNDNuevo(n_name, 2, 1)) == NULL) {
		return NULL;
	}
	free(n_name);
	afnd->potencia_i = iniMatrix(afnd->num_estados);
	afnd->relacion_inicial_i = iniMatrix(afnd->num_estados);
	AFNDInsertaSimbolo(afnd, simbolo);
	AFNDInsertaEstado(afnd, "q0", INICIAL);
	AFNDInsertaEstado(afnd, "qf", FINAL);
	AFNDInsertaTransicion(afnd, "q0", simbolo, "qf");
	return afnd;
}

AFND * AFND1ODeLambda(){
	AFND * afnd = NULL;
	if ((afnd = AFNDNuevo("afnd1o_lambda", 2, 0)) == NULL) {
		return NULL;
	}
	afnd->potencia_i = iniMatrix(afnd->num_estados);
	afnd->relacion_inicial_i = iniMatrix(afnd->num_estados);
	AFNDInsertaEstado(afnd, "q0", INICIAL);
	AFNDInsertaEstado(afnd, "qf", FINAL);
	AFNDInsertaLTransicion(afnd, "q0", "qf");
	return afnd;
}

AFND * AFND1ODeVacio(){
	AFND * afnd = NULL;
	if ((afnd = AFNDNuevo("afnd1o_DeVacio", 2, 0)) == NULL) {
		return NULL;
	}
	afnd->potencia_i = iniMatrix(afnd->num_estados);
	afnd->relacion_inicial_i = iniMatrix(afnd->num_estados);
	return afnd;
}

AFND * AFNDAAFND1O(AFND * p_afnd){
	AFND * afnd;
	char * n_name;
	int i;
	if (p_afnd == NULL) {
		return NULL;
	}

	n_name = AFND1ONuevoNombre(p_afnd, NULL, NULL, "_AFND1O");
	afnd = AFNDNuevo(n_name, p_afnd->num_estados + 2, p_afnd->num_simbolos);
	free(n_name);
	
   	p_afnd->potencia_i = iniMatrix(p_afnd->num_estados);
	p_afnd->relacion_inicial_i = iniMatrix(p_afnd->num_estados);

	AFNDInsertaCadenaSimbolos(afnd, p_afnd->simbolos);

	for (i = 0; i < p_afnd->num_estados; i++){
		AFNDInsertaEstado(afnd, getNombre(p_afnd->estados[i]), getTipo(p_afnd->estados[i]));
		renameEstado(afnd->estados[i], "_AFND1O");
	}

	AFND1OExportarTransiciones(afnd, p_afnd, "_AFND1O");
	AFNDExportarMatrices(afnd, p_afnd, 0);
	
	/*Transicionar hacia iniciales y finales*/
	AFNDInsertaEstado(afnd,"_AFND1O_i",INICIAL);
    AFNDInsertaEstado(afnd,"_AFND1O_f",FINAL);
	nuevasLTransicionesAFND1O(afnd, "_AFND1O_i", "_AFND1O_f");
	AFNDCierraLTransicion(afnd);
	return afnd;
}

AFND * AFND1OUne(AFND * p_afnd1O_1, AFND * p_afnd1O_2){
	AFND * afnd;
	char * n_name;
	int simbolos_comunes, i;
	if (p_afnd1O_1 == NULL || p_afnd1O_2 == NULL){
		return NULL;
	}
	n_name = AFND1ONuevoNombre(p_afnd1O_1, p_afnd1O_2, "_1_U_", "_2");

	/*Nuevo AFND */
	/*Calcular numero de simbolos totales entre los dos simbolos*/
	simbolos_comunes = AFNDNSimbolos(p_afnd1O_1, p_afnd1O_2);
	afnd = AFNDNuevo(n_name, p_afnd1O_1->num_estados + p_afnd1O_2->num_estados + 2, p_afnd1O_1->num_simbolos + p_afnd1O_2->num_simbolos - simbolos_comunes);
	free(n_name);
	afnd->potencia_i = iniMatrix(afnd->num_estados);
	afnd->relacion_inicial_i = iniMatrix(afnd->num_estados);
	/*Insertar simbolos*/
	AFNDInsertaCadenaSimbolos(afnd, p_afnd1O_1->simbolos);
	AFNDInsertaCadenaSimbolos(afnd, p_afnd1O_2->simbolos);


	/*Insertar y renombrar y rehacer tipo de estados*/
	for (i = 0; i < p_afnd1O_1->num_estados; i++){
		AFNDInsertaEstado(afnd, getNombre(p_afnd1O_1->estados[i]), getTipo(p_afnd1O_1->estados[i]));
		renameEstado(afnd->estados[i], "_U1_");
	}
	/*TRANSICIONES*/
	AFND1OExportarTransiciones(afnd, p_afnd1O_1, "_U1_");

	/*MATRICES*/
	AFNDExportarMatrices(afnd, p_afnd1O_1, 0);
	AFNDExportarMatrices(afnd, p_afnd1O_2, p_afnd1O_1->num_estados);

	for (i = 0; i < p_afnd1O_2->num_estados; i++){
		AFNDInsertaEstado(afnd, getNombre(p_afnd1O_2->estados[i]), getTipo(p_afnd1O_2->estados[i]));
		renameEstado(afnd->estados[i + p_afnd1O_1->num_estados], "_U2_");
	}

	AFND1OExportarTransiciones(afnd, p_afnd1O_2, "_U2_");

	AFNDInsertaEstado(afnd,"_i_1O",INICIAL);
    AFNDInsertaEstado(afnd,"_f_1O",FINAL);

	nuevasLTransicionesAFND1O(afnd, "_i_1O", "_f_1O");
	AFNDCierraLTransicion(afnd);

	return afnd;
}

AFND * AFND1OConcatena(AFND * p_afnd_origen1, AFND * p_afnd_origen2){
	AFND * afnd;
	char * n_name;
	int simbolos_comunes, i, j;
	if (p_afnd_origen1 == NULL || p_afnd_origen2 == NULL){
		return NULL;
	}
	n_name = AFND1ONuevoNombre(p_afnd_origen1, p_afnd_origen2, "_1_K_", "_2");

	/*Nuevo AFND */
	/*Calcular numero de simbolos totales entre los dos simbolos*/
	simbolos_comunes = AFNDNSimbolos(p_afnd_origen1, p_afnd_origen2);
	afnd = AFNDNuevo(n_name, p_afnd_origen1->num_estados + p_afnd_origen2->num_estados + 2, p_afnd_origen1->num_simbolos + p_afnd_origen2->num_simbolos - simbolos_comunes);
	free(n_name);
	afnd->potencia_i = iniMatrix(afnd->num_estados);
	afnd->relacion_inicial_i = iniMatrix(afnd->num_estados);
	/*Insertar simbolos*/
	AFNDInsertaCadenaSimbolos(afnd, p_afnd_origen1->simbolos);
	AFNDInsertaCadenaSimbolos(afnd, p_afnd_origen2->simbolos);


	/*Insertar y renombrar y rehacer tipo de estados*/
	for (i = 0; i < p_afnd_origen1->num_estados; i++){
		AFNDInsertaEstado(afnd, getNombre(p_afnd_origen1->estados[i]), getTipo(p_afnd_origen1->estados[i]));
		renameEstado(afnd->estados[i], "_K1_");
	}
	/*TRANSICIONES*/
	AFND1OExportarTransiciones(afnd, p_afnd_origen1, "_K1_");

	/*MATRICES*/
	AFNDExportarMatrices(afnd, p_afnd_origen1, 0);
	AFNDExportarMatrices(afnd, p_afnd_origen2, p_afnd_origen1->num_estados);

	for (i = 0; i < p_afnd_origen2->num_estados; i++){
		AFNDInsertaEstado(afnd, getNombre(p_afnd_origen2->estados[i]), getTipo(p_afnd_origen2->estados[i]));
		renameEstado(afnd->estados[i + p_afnd_origen1->num_estados], "_K2_");

		for (j = 0; j < p_afnd_origen1->num_estados; j++){
			if ((getTipo(afnd->estados[j]) == FINAL || getTipo(afnd->estados[j]) == INICIAL_Y_FINAL) 
				&& (getTipo(afnd->estados[i + p_afnd_origen1->num_estados]) == INICIAL || getTipo(afnd->estados[i + p_afnd_origen1->num_estados]) == INICIAL_Y_FINAL)){
				AFNDInsertaLTransicion(afnd, getNombre(afnd->estados[j]), getNombre(afnd->estados[i + p_afnd_origen1->num_estados]));
				setTipo(afnd->estados[j], NORMAL);
				if (getTipo(afnd->estados[i + p_afnd_origen1->num_estados]) == INICIAL){
					setTipo(afnd->estados[i + p_afnd_origen1->num_estados], NORMAL);
				} else {
					setTipo(afnd->estados[i + p_afnd_origen1->num_estados], FINAL);
				}
			}
		}		
	}

	AFND1OExportarTransiciones(afnd, p_afnd_origen2, "_K2_");

	AFNDInsertaEstado(afnd,"_K_i",INICIAL);
    AFNDInsertaEstado(afnd,"_K_f",FINAL);

	nuevasLTransicionesAFND1O(afnd, "_K_i", "_K_f");
	AFNDCierraLTransicion (afnd);

	return afnd;
}

AFND * AFND1OEstrella(AFND * p_afnd_origen){
	AFND * afnd;
	char * n_name;
	int i;
	if (p_afnd_origen == NULL){
		return NULL;
	}
	n_name = AFND1ONuevoNombre(p_afnd_origen, NULL, NULL, "X");

	/*Nuevo AFND */
	/*Calcular numero de simbolos totales entre los dos simbolos*/
	afnd = AFNDNuevo(n_name, p_afnd_origen->num_estados + 2, p_afnd_origen->num_simbolos);
	free(n_name);
	afnd->potencia_i = iniMatrix(afnd->num_estados);
	afnd->relacion_inicial_i = iniMatrix(afnd->num_estados);
	/*Insertar simbolos*/
	AFNDInsertaCadenaSimbolos(afnd, p_afnd_origen->simbolos);


	/*Insertar y renombrar y rehacer tipo de estados*/
	for (i = 0; i < p_afnd_origen->num_estados; i++){
		AFNDInsertaEstado(afnd, getNombre(p_afnd_origen->estados[i]), getTipo(p_afnd_origen->estados[i]));
		renameEstado(afnd->estados[i], "X");
	}
	/*TRANSICIONES*/
	AFND1OExportarTransiciones(afnd, p_afnd_origen, "X");

	/*MATRICES*/
	AFNDExportarMatrices(afnd, p_afnd_origen, 0);

	AFNDInsertaEstado(afnd,"_X_i",INICIAL);
    AFNDInsertaEstado(afnd,"_X_f",FINAL);

	nuevasLTransicionesAFND1O(afnd, "_X_i", "_X_f");
	AFNDInsertaLTransicion(afnd, "_X_i", "_X_f");
	AFNDInsertaLTransicion(afnd, "_X_f", "_X_i");
	AFNDCierraLTransicion(afnd);

	return afnd;
}

void AFNDADot(AFND * p_afnd){
	char * n_name;
	int i, j;
	FILE *f = NULL;
	if (p_afnd == NULL){
		return;
	}
	n_name = AFND1ONuevoNombre(p_afnd, NULL, NULL, ".dot");
	if ((f = fopen(n_name, "w")) == NULL){
		return;
	}
	free(n_name);
	fprintf(f, "digraph %s  { rankdir=LR;\n", p_afnd->nombre);
	fprintf(f, "\t\t_invisible [style=\"invis\"];\n");
	/*Printear estados*/
    for (i = 0; i < p_afnd->num_estados; i++){
    	fprintf(f, "\t\t%s", getNombre(p_afnd->estados[i]));
    	if (getTipo(p_afnd->estados[i]) == FINAL || getTipo(p_afnd->estados[i]) == INICIAL_Y_FINAL){
    		fprintf(f, " [penwidth=\"2\"]");
    	}
    	fprintf(f, ";\n");
    }
    /*Estados iniciales*/
for (i = 0; i < p_afnd->num_estados; i++){
    	if (getTipo(p_afnd->estados[i]) == INICIAL || getTipo(p_afnd->estados[i]) == INICIAL_Y_FINAL){
    		fprintf(f, "\t\t_invisible -> %s ;\n", getNombre(p_afnd->estados[i]));
    	}    	
    }
    /*Printear transiciones normales*/
    for (i = 0; i < p_afnd->num_trans; i++){
		fprintf(f, "\t\t%s -> %s [label=\"%s\"];\n", getNombre(getIni(p_afnd->transiciones[i])), getNombre(getFinal(p_afnd->transiciones[i])), getSimbolo(p_afnd->transiciones[i]));  	
    }
    /*Printear transiciones lambda*/
    for (i = 0; i < p_afnd->num_estados; i++){
    	for (j = 0; j < p_afnd->num_estados; j++){
    		if (p_afnd->relacion_inicial_i != NULL){
	    		if (getMatrixData(p_afnd->relacion_inicial_i, i, j) == 1 && i != j) {
    				fprintf(f, "\t\t%s -> %s [label=\"&lambda;\"];\n", getNombre(getEstadoPorId(p_afnd->estados, p_afnd->i_estados, i)), getNombre(getEstadoPorId(p_afnd->estados, p_afnd->i_estados, j)));  	
    			}
    		} else {
    			if (getMatrixData(p_afnd->lambdatrix, i, j) == 1 && i != j) {
    				fprintf(f, "\t\t%s -> %s [label=\"&lambda;\"];\n", getNombre(getEstadoPorId(p_afnd->estados, p_afnd->i_estados, i)), getNombre(getEstadoPorId(p_afnd->estados, p_afnd->i_estados, j)));  	
    			}
    		}
    	}
		
    }

    fprintf(f, "}");
    fclose(f);
}

char * renameEstado(Estado * estado, char * new){
	int n_size;
	char *o_name;

	if (estado == NULL || new == NULL){
		return NULL;
	}

	n_size = strlen(getNombre(estado)) + strlen(new) + 1;
	o_name = (char *) malloc ((strlen(getNombre(estado)) + 1) * sizeof(char));
	strcpy(o_name, getNombre(estado));
	estado->n = (char *) realloc(getNombre(estado), n_size * sizeof(char));
	strcpy(estado->n, new);
	strcat(estado->n, o_name);
	free(o_name);
	return estado->n;
}

AFND * nuevasLTransicionesAFND1O(AFND * p_afnd, char * ini, char * fin){
	int i = 0;
	if (p_afnd == NULL || ini == NULL || fin == NULL) {
		return NULL;
	}
	for (i=0; i<p_afnd->num_estados - 2; i++){
		if (getTipo(p_afnd->estados[i]) == INICIAL){
			AFNDInsertaLTransicion(p_afnd, ini, getNombre(p_afnd->estados[i]));
			setTipo(p_afnd->estados[i], NORMAL);
		} else 	if (getTipo(p_afnd->estados[i]) == FINAL){
			AFNDInsertaLTransicion(p_afnd, getNombre(p_afnd->estados[i]), fin);
			setTipo(p_afnd->estados[i], NORMAL);
		} else if (getTipo(p_afnd->estados[i]) == INICIAL_Y_FINAL){
			AFNDInsertaLTransicion(p_afnd, ini, getNombre(p_afnd->estados[i]));
			AFNDInsertaLTransicion(p_afnd, getNombre(p_afnd->estados[i]), fin);
			setTipo(p_afnd->estados[i], NORMAL);
		}
	}
	return p_afnd;
}

int AFNDNSimbolos(AFND * afnd_1, AFND * afnd_2){
	int i;
	int tot = 0;
	char *c;
	if (afnd_1 == NULL || afnd_2 == NULL){
		return -1;
	} 
	for (i = 0; i < afnd_1->num_simbolos; i++){
		if ((c = strchr(afnd_2->simbolos, afnd_1->simbolos[i])) != NULL){
			tot++;
		}
	}
	return tot;
}

AFND * AFNDInsertaCadenaSimbolos(AFND * p_afnd, char * cadena){
	int i, sl;
	char * c = cadena;
	if (p_afnd == NULL || cadena == NULL){
		return NULL;
	}
	sl = strlen(cadena);
	for (i = 0; i < sl; i++){		
		AFNDInsertaSimbolo(p_afnd, c);
		c++;
	}
	return p_afnd;
}

AFND * AFND1OExportarTransiciones(AFND * p_afnd_n, AFND * p_afnd_o, char * nombre){
	int i, j, k;
	char *n_trans1, *n_trans2;
	if (p_afnd_n == NULL || p_afnd_o == NULL || nombre == NULL) {
		return NULL;
	}
	for (i = 0; i < p_afnd_o->num_trans; i++){
		for (j = 0; j < p_afnd_o->num_estados; j++){
			for (k = 0; k < p_afnd_o->num_estados; k++){
				if (strcmp(getNombre(getIni(p_afnd_o->transiciones[i])), getNombre(p_afnd_o->estados[j])) == 0
					&& strcmp(getNombre(getFinal(p_afnd_o->transiciones[i])), getNombre(p_afnd_o->estados[k])) == 0) {
					
					n_trans1 = (char *) malloc ((strlen(getNombre(p_afnd_o->estados[j])) + strlen(nombre) + 1) * sizeof(char));
					strcpy(n_trans1, nombre);
					strcat(n_trans1, getNombre(p_afnd_o->estados[j]));

					n_trans2 = (char *) malloc ((strlen(getNombre(p_afnd_o->estados[k])) + strlen(nombre) + 1) * sizeof(char));
					strcpy(n_trans2, nombre);
					strcat(n_trans2, getNombre(p_afnd_o->estados[k]));

					AFNDInsertaTransicion(p_afnd_n, n_trans1, getSimbolo(p_afnd_o->transiciones[i]), n_trans2);
					free(n_trans1);
					free(n_trans2);
				}
			}
		}
	}
	return p_afnd_n;
}

char * AFND1ONuevoNombre(AFND * afnd_1, AFND * afnd_2, char * mid, char * suf){
	char * c;
	if (afnd_1 == NULL || suf == NULL) {
		return NULL;
	}
	if (afnd_2 == NULL && mid == NULL){
		c = (char *) malloc ((strlen(afnd_1->nombre) + strlen(suf) + 1) * sizeof(char));
	} else {
		c = (char *) malloc ((strlen(afnd_1->nombre) + strlen(afnd_2->nombre) + strlen(mid) + strlen(suf) + 1) * sizeof(char));
	}
	strcpy(c, afnd_1->nombre);
	if (afnd_2 != NULL && mid != NULL){
		strcat(c, mid);
		strcat(c, afnd_2->nombre);
	}

	strcat(c, suf);
	return c;
}

AFND * AFNDExportarMatrices(AFND * afnd_n, AFND * afnd_o, int pos){
	int i, j;
	if (afnd_n == NULL || afnd_o == NULL || pos < 0){
		return NULL;
	}
	for (i = 0; i < getTam(afnd_o->lambdatrix); i++){
		for (j = 0; j < getTam(afnd_o->lambdatrix); j++){
			afnd_n->lambdatrix->matrix[i + pos][j + pos] = getMatrixData(afnd_o->lambdatrix, i, j);
			if (afnd_n->relacion_inicial_i != NULL){
				afnd_n->relacion_inicial_i->matrix[i + pos][j + pos] = getMatrixData(afnd_o->relacion_inicial_i, i, j);		
				afnd_n->potencia_i->matrix[i + pos][j + pos] = getMatrixData(afnd_o->potencia_i, i, j);
			}	
		}		
	}
	return afnd_n;
}
