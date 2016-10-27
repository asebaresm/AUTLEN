#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "transicion.h"

/*NOTA: completar con paso por referencia de los estados*/
Transicion * nuevaTransicion(Estado * e_inicial, char *simbolo, Estado * e_final){
	Transicion * t = NULL;
	t = (Transicion*) malloc (sizeof(Transicion));

	return t;
}

void liberarTransicion(Transicion * transicion){
	free(transicion->s);
	free(transicion->id);
	free(transicion);
}

Estado * getIni(Transicion *t){
	return t->e_ini;
}

Estado *getFinal(Transicion *t){
	return t->e_fin;
}

char *getSimbolo(Transicion * t){
	return t->s;
}

char * buildTransId(char *nombre_estado_i, char *nombre_simbolo_entrada, char *nombre_estado_f){
	char * id = (char *) malloc (strlen(nombre_estado_i) + strlen(nombre_simbolo_entrada) + strlen(nombre_estado_f) + 1);
	strcpy(id, nombre_estado_i);
	strcat(id, nombre_simbolo_entrada);
	strcat(id, nombre_estado_f);
	return id;
}

