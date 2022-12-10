#ifndef __TABLAHASH_H__
#define __TABLAHASH_H__

#include "glist.h"
#include "common.h"


typedef struct _TablaHash *TablaHash;

struct _TablaHash {
  GList *elems;
  unsigned numElems;
  unsigned capacidad;
  FuncionCopiadora copia;
  FuncionComparadora comp;
  FuncionDestructora destr;
  FuncionHash hash;
};

#define FACTOR_CARGA 0.7

/**
 * Crea una nueva tabla hash vacia, con la capacidad dada.
 */
TablaHash tablahash_crear(unsigned capacidad,
                          FuncionComparadora comp, FuncionDestructora destr,
                          FuncionHash hash, FuncionCopiadora copia);

/**
 * Retorna el numero de elementos de la tabla.
 */
int tablahash_nelems(TablaHash tabla);

/**
 * Retorna la capacidad de la tabla.
 */
int tablahash_capacidad(TablaHash tabla);

/**
 * Destruye la tabla.
 */
void tablahash_destruir(TablaHash tabla);

/**
 * Inserta un dato en la tabla, o lo reemplaza si ya se encontraba.
 */
void tablahash_insertar(TablaHash tabla, void *dato);

/**
 * Retorna el dato de la tabla que coincida con el dato dado, o NULL si el dato
 * buscado no se encuentra en la tabla.
 */
int tablahash_buscar(TablaHash tabla, void *dato);

/**
 * Elimina el dato de la tabla que coincida con el dato dado.
 */
void tablahash_eliminar(TablaHash tabla, void *dato);

/**
 * Imprime todos los datos de la tabla
 */
void tablahash_imprimir(TablaHash tabla, FuncionVisitante visit);

void tablahash_redimensionar(TablaHash tabla);


#endif /* __TABLAHASH_H__ */