#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "estado.h"


Estado * nuevoEstado(char * nombre, TIPO tipo, int n){
	Estado * e = NULL;
	if (nombre == NULL) {
		return NULL;
	}
	e = (Estado*) malloc(sizeof(Estado));
	e->n = (char*) malloc(strlen(nombre) + 1);

	e->t = tipo;
	strcpy(e->n, nombre);
	e->id = n;
	return e;
}

void liberarEstado(Estado *estado){
	free(estado->n);
	free(estado);
}

char *getNombre(Estado *e){
	return e->n;
}

int getId(Estado *e){
	return e->id;
}

TIPO getTipo(Estado *e){
	return e->t;
}

char * tipoPrinteable(TIPO t, char *str){
	switch(t){
		case 0:
			sprintf(str,"INICIAL");
			break;
		case 1:
			sprintf(str,"NORMAL");
			break;
		case 2:
			sprintf(str,"FINAL");
			break;
		case 3:
			sprintf(str,"INICIAL_Y_FINAL");
			break;
	}
	return str;
}

BOOL equalEstado(Estado *e1,Estado *e2){
	if (strcmp (getNombre(e1), getNombre(e2)) == 0 ){
		return TRUE;
	}
	return FALSE;
}
