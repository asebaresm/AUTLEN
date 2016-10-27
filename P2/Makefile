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
DIR=P1
# Ejecutables
EJECUTABLES=P1 pr_afnd_s3
# eliminado 'testP1' para la version de entrega
#==================================================================================
# Construccion de los ejecutables
#==================================================================================
all: $(EJECUTABLES)

P1: estado.o transicion.o afnd.o main.o
	@echo Generando ejecutable P1...
	$(CC) $(CFLAGS) -o P1 main.o afnd.o estado.o transicion.o 

testP1: estado.o transicion.o afnd.o testmain.o
	@echo Generando ejecutable de pruebas...
	$(CC) $(CFLAGS) -o testP1 testmain.o afnd.o estado.o transicion.o

pr_afnd_s3: estado.o transicion.o afnd.o pr_afnd_s3.o
	@echo Generando ejecutable de pruebas...
	$(CC) $(CFLAGS) -o pr_afnd_s3 pr_afnd_s3.o afnd.o estado.o transicion.o
#==================================================================================
# Generacion .o/.c
#==================================================================================
main.o: main.c
	@echo Compilando main.c...
	$(CC) $(CFLAGS) -c main.c

testmain.o: testmain.c
	@echo Compilando testmain.c...
	$(CC) $(CFLAGS) -c testmain.c

pr_afnd_s3.o: pr_afnd_s3.c
	@echo Compilando pr_afnd_s3.c...
	$(CC) $(CFLAGS) -c pr_afnd_s3.c

afnd.o: afnd.c
	@echo Compilando adnd.c...
	$(CC) $(CFLAGS) -c afnd.c

transicion.o: transicion.c
	@echo Compilando transicion.c...
	$(CC) $(CFLAGS) -c transicion.c

estado.o: estado.c
	@echo Compilando estado.c...
	$(CC) $(CFLAGS) -c estado.c
#==================================================================================
# Otras acciones del make
#==================================================================================
clean:
	rm -rf *.o *.core *~ *\.a *\.salida *\.out $(EJECUTABLES)

test_practica:
	./P1

prueba_dinamica: pr_afnd_s3
	make valgrind_test_pr_afnd_s3

valgrind_test: P1
	valgrind --leak-check=full --show-leak-kinds=all ./P1

valgrind_test_pr_afnd_s3: pr_afnd_s3
	valgrind --leak-check=full --show-leak-kinds=all ./pr_afnd_s3

#valgrind_testP1: testP1
#	valgrind --leak-check=full --show-leak-kinds=all ./testP1
#==================================================================================
#
#
#==================================================================================