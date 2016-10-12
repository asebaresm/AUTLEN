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
EJECUTABLES=P1
#==================================================================================
# Construccion de los ejecutables
#==================================================================================
all: $(EJECUTABLES)

P1: estado.o transicion.o afnd.o main.o
	@echo Generando ejecutable...
	$(CC) $(CFLAGS) -o P1 main.o afnd.o estado.o transicion.o 

#==================================================================================
# Generacion .o/.c
#==================================================================================
main.o: main.c
	@echo Compilando main.c...
	$(CC) $(CFLAGS) -c main.c

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
	rm -rf *.o *.core *\.a *\.salida *\.out $(EJECUTABLES)

test1:
	./P1

valgrind_test:
	valgrind --leak-check=full --show-leak-kinds=all ./P1
#==================================================================================
#
#
#==================================================================================