#ifndef __PALABRA_H__
#define __PALABRA_H__
#include <stddef.h>
#include "tablahash.h"
#include "glist.h"


typedef struct _Sugerencias *Sugerencias;

struct _Sugerencias{
    char* palabra;
    GList list;
    unsigned cant_sug;
    int cache;
};

struct _Palabra{
    char* str;
    unsigned len;
};

typedef struct _Palabra *Palabra;

// Este include va aca porque o sino tengo una importacion circular
#include "utils.h"

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
 * Toma una palabra y la divide en dos palabras
*/
Sugerencias palabra_dividir(Palabra palabra, TablaHash tabla, Sugerencias sug);

/**
 * Toma una palabra y permuta todas los caracteres adyacentes
*/
Sugerencias palabra_permutar(Palabra palabra, TablaHash tabla, Sugerencias sug,
                            GList* not_found, TablaHash tne, int dist);

Sugerencias palabra_borrar_caracter(Palabra palabra, TablaHash tabla,
                                    Sugerencias sug, GList* not, TablaHash tne, int dist);

Sugerencias palabra_cambiar_caracter(Palabra palabra, TablaHash tabla,
                                    Sugerencias sug, GList* not, TablaHash tne, int dist);

Sugerencias palabra_agregar_caracter(Palabra palabra, TablaHash tabla,
                                    Sugerencias sug, GList* not, TablaHash tne, int dist);

Sugerencias crear_sugerencias(char* word, int cant_sug, int cache);

void sugerencias_destruir(Sugerencias sug);

void sugerencia_imprimir_palabra(Sugerencias sug);

Sugerencias sugerencia_copia(Sugerencias sug);

int sugerencia_comparar(Sugerencias sug1, Sugerencias sug2);


#endif /* __PALABRA_H__ */
