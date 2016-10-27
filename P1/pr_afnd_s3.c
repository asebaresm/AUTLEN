#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "alfabeto.h"
#include "palabra.h"
#include "estado.h"
#include "afnd.h"



int main(int argc, char ** argv)
{

	AFND * p_afnd1;
	AFND * p_afnd2;
	AFND * p_afnd3;
	AFND * p_afnd4;

/********************************************************************************/

	p_afnd1 = AFNDNuevo("afs3_1",3,2);


	AFNDInsertaSimbolo(p_afnd1,"0");
	AFNDInsertaSimbolo(p_afnd1,"1");

	AFNDInsertaEstado(p_afnd1,"q0",INICIAL);
	AFNDInsertaEstado(p_afnd1,"q1",NORMAL);
	AFNDInsertaEstado(p_afnd1,"qf",FINAL);

	AFNDInsertaTransicion(p_afnd1, "q0", "0", "q0");
	AFNDInsertaTransicion(p_afnd1, "q0", "1", "q0");
	AFNDInsertaTransicion(p_afnd1, "q0", "1", "q1");
	AFNDInsertaTransicion(p_afnd1, "q1", "1", "qf");
	AFNDInsertaTransicion(p_afnd1, "qf", "0", "qf");
	AFNDInsertaTransicion(p_afnd1, "qf", "1", "qf");

	fprintf(stdout,"\n****************** AFND *********************\n");
	AFNDImprime(stdout,p_afnd1);
	fprintf(stdout,"\n*********************************************\n");

	p_afnd1= AFNDInsertaLetra(p_afnd1,"0");
	p_afnd1= AFNDInsertaLetra(p_afnd1,"0");
	p_afnd1= AFNDInsertaLetra(p_afnd1,"0");
	p_afnd1= AFNDInsertaLetra(p_afnd1,"1");
	p_afnd1= AFNDInsertaLetra(p_afnd1,"0");
	p_afnd1= AFNDInsertaLetra(p_afnd1,"0");
	p_afnd1= AFNDInsertaLetra(p_afnd1,"1");
	p_afnd1= AFNDInsertaLetra(p_afnd1,"1");
	p_afnd1= AFNDInsertaLetra(p_afnd1,"0");

	p_afnd1 = AFNDInicializaEstado(p_afnd1);

	fprintf(stdout,"\n**************** PROCESA CADENA *************\n");
	AFNDImprimeCadenaActual(stdout,p_afnd1);
	fprintf(stdout,"\n*********************************************\n");

	AFNDProcesaEntrada(stdout,p_afnd1);




/********************************************************************************/


	fprintf(stdout,"\n**************** PROCESA CADENA *************\n");
	AFNDImprimeCadenaActual(stdout,p_afnd1);
	fprintf(stdout,"\n*********************************************\n");

	p_afnd1 = AFNDInicializaEstado(p_afnd1);
	AFNDProcesaEntrada(stdout,p_afnd1);


	AFNDElimina(p_afnd1);



/********************************************************************************/


	p_afnd2 = AFNDNuevo("afs3_2",3,2);


	AFNDInsertaSimbolo(p_afnd2,"0");
	AFNDInsertaSimbolo(p_afnd2,"1");

	AFNDInsertaEstado(p_afnd2,"q0",INICIAL_Y_FINAL);
	AFNDInsertaEstado(p_afnd2,"q1",NORMAL);
	AFNDInsertaEstado(p_afnd2,"qf",FINAL);

	AFNDInsertaTransicion(p_afnd2, "q0", "0", "q0");
	AFNDInsertaTransicion(p_afnd2, "q0", "1", "q0");
	AFNDInsertaTransicion(p_afnd2, "q0", "1", "q1");
	AFNDInsertaTransicion(p_afnd2, "q1", "1", "qf");
	AFNDInsertaTransicion(p_afnd2, "qf", "0", "qf");
	AFNDInsertaTransicion(p_afnd2, "qf", "1", "qf");

	fprintf(stdout,"\n****************** AFND *********************\n");
	AFNDImprime(stdout,p_afnd2);
	fprintf(stdout,"\n*********************************************\n");

	p_afnd2= AFNDInsertaLetra(p_afnd2,"0");
	p_afnd2= AFNDInsertaLetra(p_afnd2,"0");
	p_afnd2= AFNDInsertaLetra(p_afnd2,"0");
	p_afnd2= AFNDInsertaLetra(p_afnd2,"1");
	p_afnd2= AFNDInsertaLetra(p_afnd2,"0");
	p_afnd2= AFNDInsertaLetra(p_afnd2,"0");
	p_afnd2= AFNDInsertaLetra(p_afnd2,"0");
	p_afnd2= AFNDInsertaLetra(p_afnd2,"1");
	p_afnd2= AFNDInsertaLetra(p_afnd2,"0");

	p_afnd2 = AFNDInicializaEstado(p_afnd2);

	fprintf(stdout,"\n**************** PROCESA CADENA *************\n");
	AFNDImprimeCadenaActual(stdout,p_afnd2);
	fprintf(stdout,"\n*********************************************\n");

	AFNDProcesaEntrada(stdout,p_afnd2);

/********************************************************************************/


	fprintf(stdout,"\n**************** PROCESA CADENA *************\n");
	AFNDImprimeCadenaActual(stdout,p_afnd2);
	fprintf(stdout,"\n*********************************************\n");

	p_afnd2 = AFNDInicializaEstado(p_afnd2);
	AFNDProcesaEntrada(stdout,p_afnd2);



	AFNDElimina(p_afnd2);


/********************************************************************************/


	p_afnd3 = AFNDNuevo("afs3_3",3,2);


	AFNDInsertaSimbolo(p_afnd3,"0");
	AFNDInsertaSimbolo(p_afnd3,"1");

	AFNDInsertaEstado(p_afnd3,"q0",INICIAL);
	AFNDInsertaEstado(p_afnd3,"q1",NORMAL);
	AFNDInsertaEstado(p_afnd3,"qf",FINAL);

	AFNDInsertaTransicion(p_afnd3, "q0", "0", "q0");
	AFNDInsertaTransicion(p_afnd3, "q1", "0", "qf");
	AFNDInsertaTransicion(p_afnd3, "qf", "0", "qf");
	AFNDInsertaTransicion(p_afnd3, "qf", "1", "qf");

	fprintf(stdout,"\n****************** AFND *********************\n");
	AFNDImprime(stdout,p_afnd3);
	fprintf(stdout,"\n*********************************************\n");

	p_afnd3= AFNDInsertaLetra(p_afnd3,"0");
	p_afnd3= AFNDInsertaLetra(p_afnd3,"1");
	p_afnd3= AFNDInsertaLetra(p_afnd3,"0");
	p_afnd3= AFNDInsertaLetra(p_afnd3,"0");
	p_afnd3= AFNDInsertaLetra(p_afnd3,"0");
	p_afnd3= AFNDInsertaLetra(p_afnd3,"1");

	p_afnd3 = AFNDInicializaEstado(p_afnd3);

	fprintf(stdout,"\n**************** PROCESA CADENA *************\n");
	AFNDImprimeCadenaActual(stdout,p_afnd3);
	fprintf(stdout,"\n*********************************************\n");

	AFNDProcesaEntrada(stdout,p_afnd3);


	AFNDElimina(p_afnd3);
	

/********************************************************************************/


	p_afnd4 = AFNDNuevo("afs3_4",7,2);


	AFNDInsertaSimbolo(p_afnd4,"0");
	AFNDInsertaSimbolo(p_afnd4,"1");

	AFNDInsertaEstado(p_afnd4,"q0",INICIAL_Y_FINAL);
	AFNDInsertaEstado(p_afnd4,"q1",NORMAL);
	AFNDInsertaEstado(p_afnd4,"q2",NORMAL);
	AFNDInsertaEstado(p_afnd4,"q3",NORMAL);
	AFNDInsertaEstado(p_afnd4,"q4",FINAL);
	AFNDInsertaEstado(p_afnd4,"q5",NORMAL);
	AFNDInsertaEstado(p_afnd4,"q6",FINAL);

	AFNDInsertaTransicion(p_afnd4, "q0", "0", "q1");
	AFNDInsertaTransicion(p_afnd4, "q0", "1", "q1");
	AFNDInsertaTransicion(p_afnd4, "q1", "0", "q1");
	AFNDInsertaTransicion(p_afnd4, "q1", "1", "q1");
	AFNDInsertaTransicion(p_afnd4, "q1", "1", "q2");
	AFNDInsertaTransicion(p_afnd4, "q2", "0", "q3");
	AFNDInsertaTransicion(p_afnd4, "q3", "1", "q4");

	AFNDInsertaTransicion(p_afnd4, "q0", "0", "q5");
	AFNDInsertaTransicion(p_afnd4, "q5", "0", "q6");
	AFNDInsertaTransicion(p_afnd4, "q6", "0", "q6");
	AFNDInsertaTransicion(p_afnd4, "q6", "1", "q6");

	fprintf(stdout,"\n****************** AFND *********************\n");
	AFNDImprime(stdout,p_afnd4);
	fprintf(stdout,"\n*********************************************\n");

	p_afnd4= AFNDInsertaLetra(p_afnd4,"0");
	p_afnd4= AFNDInsertaLetra(p_afnd4,"0");
	p_afnd4= AFNDInsertaLetra(p_afnd4,"1");
	p_afnd4= AFNDInsertaLetra(p_afnd4,"1");
	p_afnd4= AFNDInsertaLetra(p_afnd4,"0");
	p_afnd4= AFNDInsertaLetra(p_afnd4,"1");

	p_afnd4 = AFNDInicializaEstado(p_afnd4);

	fprintf(stdout,"\n**************** PROCESA CADENA *************\n");
	AFNDImprimeCadenaActual(stdout,p_afnd4);
	fprintf(stdout,"\n*********************************************\n");

	AFNDProcesaEntrada(stdout,p_afnd4);

/********************************************************************************/


	fprintf(stdout,"\n**************** PROCESA CADENA *************\n");
	AFNDImprimeCadenaActual(stdout,p_afnd4);
	fprintf(stdout,"\n*********************************************\n");

	p_afnd4 = AFNDInicializaEstado(p_afnd4);
	AFNDProcesaEntrada(stdout,p_afnd4);


/********************************************************************************/

	p_afnd4 = AFNDInicializaCadenaActual(p_afnd4);
	p_afnd4= AFNDInsertaLetra(p_afnd4,"1");
	p_afnd4= AFNDInsertaLetra(p_afnd4,"0");
	p_afnd4= AFNDInsertaLetra(p_afnd4,"0");
	p_afnd4= AFNDInsertaLetra(p_afnd4,"1");
	p_afnd4= AFNDInsertaLetra(p_afnd4,"1");
	p_afnd4= AFNDInsertaLetra(p_afnd4,"0");
	p_afnd4= AFNDInsertaLetra(p_afnd4,"1");

	p_afnd4 = AFNDInicializaEstado(p_afnd4);

	fprintf(stdout,"\n**************** PROCESA CADENA *************\n");
	AFNDImprimeCadenaActual(stdout,p_afnd4);
	fprintf(stdout,"\n*********************************************\n");

	AFNDProcesaEntrada(stdout,p_afnd4);


/********************************************************************************/

	p_afnd4 = AFNDInicializaCadenaActual(p_afnd4);
	p_afnd4= AFNDInsertaLetra(p_afnd4,"0");
	p_afnd4= AFNDInsertaLetra(p_afnd4,"0");
	p_afnd4= AFNDInsertaLetra(p_afnd4,"1");
	p_afnd4= AFNDInsertaLetra(p_afnd4,"1");
	p_afnd4= AFNDInsertaLetra(p_afnd4,"1");

	p_afnd4 = AFNDInicializaEstado(p_afnd4);

	fprintf(stdout,"\n**************** PROCESA CADENA *************\n");
	AFNDImprimeCadenaActual(stdout,p_afnd4);
	fprintf(stdout,"\n*********************************************\n");

	AFNDProcesaEntrada(stdout,p_afnd4);



	AFNDElimina(p_afnd4);
	
	return 0;

}