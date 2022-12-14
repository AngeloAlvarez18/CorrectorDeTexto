#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "glist.h"


/**
 * Devuelve una lista vacía.
 */
GList glist_crear() { return NULL; }

/**
 * Destruccion de la lista.
 * destroy es una función que libera el dato almacenado.
 */
void glist_destruir(GList list, FuncionDestructora destroy) {
  GNode *nodeToDelete;
  while (list != NULL) {
    nodeToDelete = list;
    list = list->next;
    destroy(nodeToDelete->data);
    free(nodeToDelete);
  }
}

/**
 * Determina si la lista es vacía.
 */
int glist_vacia(GList list) { return (list == NULL); }

/**
 * Agrega un elemento al inicio de la lista.
 * copy es una función que retorna una copia física del dato.
 */
GList glist_agregar_inicio(GList list, void *data, FuncionCopiadora copy) {
  GNode *newNode = malloc(sizeof(GNode));
  assert(newNode != NULL);
  newNode->next = list;
  newNode->data = copy(data);
  return newNode;
}

GList glist_agregar_final(GList list, void *dato, FuncionCopiadora copy){
  GNode *newNode = malloc(sizeof(GNode));
  assert(newNode != NULL);
  newNode->next = NULL;
  newNode->data = copy(dato);
  
  if (glist_vacia(list)){
    newNode->ult = newNode;
    return newNode;
  }
    
  else{
    list->ult->next = newNode;
    list->ult = newNode;
    return list;
  }
}

/**
 * Recorrido de la lista, utilizando la funcion pasada.
 */
void glist_recorrer(GList list, FuncionVisitante visit) {
  for (GNode *node = list; node != NULL; node = node->next)
    visit(node->data);
}

int glist_buscar(GList list, void* dato, FuncionComparadora cmp){
  for(GList node = list; node != NULL; node = node->next){
    if (cmp(node->data,dato) == 0)
      return 1;
  }
  return 0;
}

GList glist_eliminar(GList list, void* dato, FuncionDestructora destroy,
                     FuncionComparadora cmp) {
  
  if (glist_vacia(list))
    return list;

  else if (cmp(list->data, dato) == 0){
    GList nodo_a_eliminar = list;
    list = list->next;
    destroy(nodo_a_eliminar->data);
    free(nodo_a_eliminar);
    return list;
  }
  else{
    for(GList node = list; node->next != NULL; node = node->next){
        if (cmp(node->next->data, dato) == 0){
          GList nodo_a_eliminar = node->next;
          node->next = nodo_a_eliminar->next;
          destroy(nodo_a_eliminar->data);
          free(nodo_a_eliminar);
          return list;
        }
    }
  }
  return list;
}
