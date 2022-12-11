#ifndef __IO_H__
#define __IO_H__
#include "../estructuras/tablahash.h"
#include <stdio.h>
#include "../estructuras/sugerencias.h"
#include "../estructuras/palabra.h"

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
 * Lee el archivo de cache, insertando Sugerencias a la tabla hash.
*/
void leer_cache(FILE* archivo, TablaHash cache);

/**
 * Lee el archivo de cache y guardas las palabras con sus sugerencias.
*/
void escribir_cache(char* arch, Sugerencias sug);

/**
 * Escribe las palabras no encontradas en el diciconario con sus 
 * respectivas sugerencias, en el archivo de salida.
*/
void escribir_sugerencias(FILE* arch, Sugerencias sug, int linea);


#endif /* __IO_H__ */