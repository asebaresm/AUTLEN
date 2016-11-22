#==================================================================================
# BRIEF: Makefile de la librería de AFNDs, Práctica1
# AUTHOR: Alfonso Sebares, Juan Soberon 
#==================================================================================

#==================================================================================
# Entorno de desarrollo y opciones
#==================================================================================
# Compilador
CC=gcc -ansi -pedantic 
CFLAGS=-g -Wall

# Directorio en el que se encuentra el proyecto
DIR=P3
# Ejecutables
EJECUTABLES=P3
# eliminado 'testP1' para la version de entrega
#==================================================================================
# Construccion de los ejecutables
#==================================================================================
all: $(EJECUTABLES)

P3: estado.o transicion.o relaciones.o afnd.o pr_er_enunciado.o
	@echo Generando ejecutable P3...
	$(CC) $(CFLAGS) -o P3 pr_er_enunciado.o afnd.o estado.o transicion.o relaciones.o 

#==================================================================================
# Generacion .o/.c
#==================================================================================
pr_er_enunciado.o: pr_er_enunciado.c
	@echo Compilando pr_er_enunciado.c...
	$(CC) $(CFLAGS) -c pr_er_enunciado.c

afnd.o: afnd.c
	@echo Compilando adnd.c...
	$(CC) $(CFLAGS) -c afnd.c

transicion.o: transicion.c
	@echo Compilando transicion.c...
	$(CC) $(CFLAGS) -c transicion.c

estado.o: estado.c
	@echo Compilando estado.c...
	$(CC) $(CFLAGS) -c estado.c

relaciones.o: relaciones.c
	@echo Compilando relaciones.c...
	$(CC) $(CFLAGS) -c relaciones.c
#==================================================================================
# Otras acciones del make
#==================================================================================
clean:
	rm -rf *.o *.core *~ *\.a *\.salida *\.out $(EJECUTABLES)

test_practica:
	./P3

prueba_dinamica: pr_afnd_s3
	make valgrind_test

valgrind_test: P2
	valgrind --leak-check=full --show-leak-kinds=all ./P3


#valgrind_testP1: testP1
#	valgrind --leak-check=full --show-leak-kinds=all ./testP1
#==================================================================================
#
#
#==================================================================================
