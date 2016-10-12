#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "estado.h"


Estado * nuevoEstado(char * nombre, TIPO tipo){
	Estado * e = NULL;
	if (nombre == NULL) {
		return NULL;
	}
	e = (Estado*) malloc(sizeof(Estado));
	e->n = (char*) malloc(strlen(nombre));
	e->t = tipo;
	return e;
}

void liberarEstado(Estado *estado){
	free(estado->n);
	free(estado);
}
