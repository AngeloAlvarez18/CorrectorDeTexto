#ifndef __PALABRA_H__
#define __PALABRA_H__
#include <stddef.h>
#include "tablahash.h"
#include "sugerencias.h"

// Estructura principal de Palabra.
struct _Palabra{
    char* str;
    unsigned len;
};

typedef struct _Palabra *Palabra;

// Se incluye este header aca para no generar una importacion circular.
#include "../src/utils.h"

/**
 * Crea una palabra
*/
Palabra palabra_crear(char* str, unsigned len);

/**
 * Crea una copia fisica de una palabra
*/
Palabra palabra_copia(Palabra palabra);

/**
 * Destruye una palabra
*/
void palabra_destruir(Palabra palabra);

/**
 * Imprime una palabra
*/
void palabra_imprimir(Palabra palabra);

/**
 * Compara una palabra, devolviendo 0 si son iguales y otra entero
 * en caso contrario
*/
int palabra_comparar(Palabra palabra1, Palabra palabra2);

/**
 * Toma una palabra y la divide en dos palabras, probando todas las combinaciones
*/
void palabra_dividir(Palabra palabra, TablaHash tabla, Sugerencia sug);

/**
 * Toma una palabra y permuta todas los caracteres adyacentes,
 * probando todas las combinaciones.
*/
void palabra_permutar(Palabra palabra, TablaHash tabla, Sugerencia sug,
                            GList* not_found, TablaHash tne, int dist);

/**
 * Toma una palabra y borra un caracter, probando todas las combinaciones.
*/
void palabra_borrar_caracter(Palabra palabra, TablaHash tabla,
                                    Sugerencia sug, GList* not, TablaHash tne, int dist);

/**
 * Toma una palabra y cambia un caracter, probando todas las combinaciones.
*/
void palabra_cambiar_caracter(Palabra palabra, TablaHash tabla,
                                    Sugerencia sug, GList* not, TablaHash tne, int dist);

/**
 * Toma una palabra y agrega un caracter, probando todas las combinaciones.
*/
void palabra_agregar_caracter(Palabra palabra, TablaHash tabla,
                                    Sugerencia sug, GList* not, TablaHash tne, int dist);


#endif /* __PALABRA_H__ */
