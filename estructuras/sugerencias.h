#ifndef __SUGERENCIAS_H__
#define __SUGERENCIAS_H__
#include "glist.h"

typedef struct _Sugerencias *Sugerencia;

// Estructura principal de Sugerencia.
struct _Sugerencias{
    char* palabra;
    GList list;
    unsigned cant_sug;
    int cache;
};

/**
 * Crea una Sugerencia.
*/
Sugerencia crear_sugerencias(char* word, int cant_sug, int cache);

/**
 * Destruye una sugerencia.
*/
void sugerencias_destruir(Sugerencia sug);

/**
 * Crea una copia fisica del dato.
*/
Sugerencia sugerencia_copia(Sugerencia sug);

/**
 * Compara dos sugerencias, devolviendo 0 si son iguales, y otro
 * numero en caso contrario.
*/
int sugerencia_comparar(Sugerencia sug1, Sugerencia sug2);

#endif /* __SUGERENCIAS_H__ */