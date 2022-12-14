# Makefile para Corrector de Texto

# Compilador y banderas
CC = gcc
FLAGS = -Wall -Werror -Wextra -std=c99

# Archivos fuente
MAIN = src/main
GLIST = estructuras/glist
PALABRA = estructuras/palabra
SUGERENCIAS = estructuras/sugerencias
TABLAHASH = estructuras/tablahash_d
UTILS = src/utils
IO = src/io

SOURCE = $(UTILS).o $(IO).o
ESTRUCTURAS = $(GLIST).o $(PALABRA).o $(TABLAHASH).o $(SUGERENCIAS).o

# Compilar el programa
all: $(MAIN).o $(SOURCE) $(ESTRUCTURAS)
	$(CC) $(FLAGS) $^ -o main 


# Crear .o desde archivos .c
%.o: %.c
	$(CC) -g -c $(CFLAGS) $^ -o $@

# Compilar para depuracion
.PHONY: debug
debug: $(CC) -g -o main $(FLAGS) $(MAIN).c $(GLIST).c $(PALABRA).c \
	   $(SUGERENCIAS).c $(TABLAHASH).c $(UTILS).c

# Remover archivos .o
.PHONY: clean
clean:
	rm *.o