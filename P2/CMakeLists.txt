cmake_minimum_required(VERSION 3.6)
project(P2)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror")

set(SOURCE_FILES main.c afnd.c estado.c transicion.c relaciones.c)
add_executable(P2 ${SOURCE_FILES})