#ifndef __GLIST_H__
#define __GLIST_H__

#include "common.h"

typedef struct _GNode {
  void *data;
  struct _GNode *next;
  struct _GNode *ult;
} GNode;

typedef GNode *GList;

/**
 * Devuelve una lista vacía.
 */
GList glist_crear();

/**
 * Destruccion de la lista.
 */
void glist_destruir(GList lista, FuncionDestructora destruir);

/**
 * Determina si la lista es vacía.
 */
int glist_vacia(GList lista);

/**
 * Agrega un elemento al inicio de la lista.
 */
GList glist_agregar_inicio(GList lista, void *dato, FuncionCopiadora copy);

/**
 * Agrega un elemento al final de la lista.
*/
GList glist_agregar_final(GList list, void *dato, FuncionCopiadora copy);

/**
 * Recorrido de la lista, utilizando la funcion pasada.
 */
void glist_recorrer(GList lista, FuncionVisitante visitar);

/**
 * Determina si un elemento esta en la lista. Devuelve el elemento si este
 * se encuentra, y devuelve NULL en caso contrario.
 */
int glist_buscar(GList list, void* dato, FuncionComparadora cmp);

/**
 * Si el dato pasado se encuentra en la lista, lo elimina de esta
 */
GList glist_eliminar(GList list, void* dato, FuncionDestructora destroy, FuncionComparadora cmp);

#endif /* __GLIST_H__ */
