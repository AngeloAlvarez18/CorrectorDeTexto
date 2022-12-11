#ifndef __UTILS_H__
#define __UTILS_H__
#include "../estructuras/palabra.h"
#include <stdio.h>
#include "../estructuras/tablahash.h"

/**
 * Funcion de hash para palabras.
*/
unsigned djb2(Palabra word);

/**
 * Funcion de hash para sugerencias.
*/
unsigned djb2_sug(Sugerencias sug);


/**
 * Mientras no se haya encontrado 5 sugerencias para la palabra pasada,
 * aplica las reglas de correcion a la palabra.
*/
Sugerencias distancia_n(Palabra palabra, TablaHash tabla, Sugerencias sug,
                        GList* no_encontradas, TablaHash tne, int dist);

/**
 * Busca posibles sugerencias para la palabra pasada.
*/
Sugerencias buscar_sugerencias(Palabra palabra, TablaHash tabla, Sugerencias sug,
                                TablaHash chequeadas, char* cache);

/**
 * Se fija si una palabra esta en la tabla hash de cache, en caso
 * de estar, retorna una estructura Sugerencia de esta palabra.
*/
Sugerencias buscar_en_cache(TablaHash cache, Sugerencias sug);

/**
 * Retorna un mensaje de error.
*/
void error();

/**
 * Retorna un mensaje de error.
*/
void quit(char* str);

#endif /** __UTILS_H__ */