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
	rm -rf *.o *.core *~ *\.a *\.salida *\.out *.dot *.gif $(EJECUTABLES)

test_practica:
	./P3

valgrind_test: P3
	valgrind --leak-check=full --show-leak-kinds=all ./P3

dot_creation: P3
	./P3
	dot -Tgif afnd1o_0.dot > afnd1o_0.gif
	dot -Tgif afnd1o_0_1_U_afnd1o_1_2.dot > afnd1o_0_1_U_afnd1o_1_2.gif
	dot -Tgif afnd1o_0_1_U_afnd1o_1_2X.dot > afnd1o_0_1_U_afnd1o_1_2X.gif
	dot -Tgif afnd1o_1_1_K_afnd1o_1_2_1_K_afnd1o_0_1_U_afnd1o_1_2X_2.dot > afnd1o_1_1_K_afnd1o_1_2_1_K_afnd1o_0_1_U_afnd1o_1_2X_2.gif
	dot -Tgif afnd1o_1_1_K_afnd1o_1_2.dot > afnd1o_1_1_K_afnd1o_1_2.gif
	dot -Tgif afnd1o_1.dot > afnd1o_1.gif
	dot -Tgif afnd1o_1X.dot > afnd1o_1X.gif


#valgrind_testP1: testP1
#	valgrind --leak-check=full --show-leak-kinds=all ./testP1
#==================================================================================
#
#
#==================================================================================
