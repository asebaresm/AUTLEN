cmake_minimum_required(VERSION 3.6)
project(P1)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror")

set(SOURCE_FILES testmain.c afnd.c afnd.h estado.c estado.h transicion.c transicion.h)
add_executable(P1 ${SOURCE_FILES})