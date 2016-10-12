#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "transicion.h"

Transicion * nuevaTransicion(Estado * e_inicial, char simbolo, Estado * e_final){
	Transicion * t = NULL;
	if ((t = (Transicion*) malloc (sizeof(Transicion))) == NULL) {
		return NULL;
	}
	return t;
}

void liberarTransicion(Transicion * transicion){

}

Estado * getIni(Transicion *t){
	return t->e_ini;
}

Estado *getFinal(Transicion *t){
	return t->e_fin;
}

