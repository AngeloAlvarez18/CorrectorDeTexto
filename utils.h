#ifndef __UTILS_H__
#define __UTILS_H__
#include "palabra.h"
#include <stdio.h>
#include "tablahash.h"

/**
 * Funcion de hash para palabras.
*/
unsigned djb2(Palabra word);

/**
 * Funcion de hash para sugerencias.
*/
unsigned djb2_sug(Sugerencias sug);

/**
 * Lee y agrega palabras al diccionario.
 */
void leer_diccionario(char* path, TablaHash tabla);

/**
 * Lee palabras del archivo de entrada, si la palabra no esta en el
 * diccionario, corrige dicha palabra.
*/
void leer_y_corregir(char *entrada, char *salida, char *cache,TablaHash tabla);

/**
 * Lee una palabra del archivo de entrada y retorna
 * una estructura Palabra.
*/
Palabra leer_palabra(FILE* archivo, int* linea, int* flag);

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
                                TablaHash chequeadas);

/**
 * Lee el archivo de cache, insertando Sugerencias a la tabla hash.
*/
void leer_cache(FILE* archivo, TablaHash cache);

/**
 * Lee el archivo de cache y guardas las palabras con sus sugerencias.
*/
void escribir_cache(FILE* arch, char* palabra, int cant_sugs, GList list);

/**
 * Escribe las palabras no encontradas en el diciconario con sus 
 * respectivas sugerencias, en el archivo de salida.
*/
void escribir_sugerencias(FILE* arch, Sugerencias sug, int linea);

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