#ifndef __UTILS_H__
#define __UTILS_H__
#include "palabra.h"
#include "tablahash.h"

/**
 * Funcion de hash para strings propuesta por Kernighan & Ritchie en "The C
 * Programming Language (Second Ed.)".
 */
unsigned KRHash(Palabra word);
unsigned djb2(Palabra word);

/**
 * Lee y agrega palabras al diccionario
 */
void leer_diccionario(char* path, TablaHash tabla);

void readfile(char* path, char* salida, TablaHash tabla);

void readfile2(char *entrada, char *salida, TablaHash tabla);

void readfile_mejorado(char *entrada, char *salida, TablaHash tabla);

Palabra leer_palabra(FILE* archivo, int* linea, int* flag);

Sugerencias distancia_n(Palabra palabra, TablaHash tabla, Sugerencias sug,
                        GList* no_encontradas, TablaHash tne, int dist);

void buscar_sugerencias(Palabra palabra, TablaHash tabla, Sugerencias sug, FILE* file, TablaHash chequeadas);

Sugerencias distancia_dos(Palabra palabra, TablaHash tabla, Sugerencias sug, GList* not);

void leer_cache(FILE* archivo, TablaHash cache);

void escribir_cache(FILE* arch, char* palabra, int cant_sugs, GList list);

unsigned djb2_sug(Sugerencias sug);

void escribir_sugerencias(FILE* arch, Sugerencias sug, int linea);

Sugerencias buscar_sugerencias2(Palabra palabra, TablaHash tabla, Sugerencias sug,
                                TablaHash chequeadas);

Sugerencias buscar_en_cache(TablaHash cache, Sugerencias sug);

#endif /** __UTILS_H__ */