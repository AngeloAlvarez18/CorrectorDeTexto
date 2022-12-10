#include <stdlib.h>
#include <assert.h>
#include "tablahash.h"
#include "glist.h"


/**
 * Crea una nueva tabla hash vacia, con la capacidad dada.tablahash/tablahash_d.c
 */
TablaHash tablahash_crear(unsigned capacidad,
                          FuncionComparadora comp, FuncionDestructora destr,
                          FuncionHash hash, FuncionCopiadora copia) {

  // Pedimos memoria para la estructura principal y las casillas.
  TablaHash tabla = malloc(sizeof(struct _TablaHash));
  assert(tabla != NULL);
  tabla->elems = malloc(sizeof(GList) * capacidad);
  assert(tabla->elems != NULL);
  tabla->numElems = 0;
  tabla->capacidad = capacidad;
  tabla->copia = copia;
  tabla->comp = comp;
  tabla->destr = destr;
  tabla->hash = hash;

  // Inicializamos las casillas con datos nulos.
  for (unsigned idx = 0; idx < capacidad; ++idx) {
      tabla->elems[idx] = glist_crear();
  }
  return tabla;
}

/**
 * Retorna el numero de elementos de la tabla.
 */
int tablahash_nelems(TablaHash tabla) { return tabla->numElems; }

/**
 * Retorna la capacidad de la tabla.
 */
int tablahash_capacidad(TablaHash tabla) { return tabla->capacidad; }

/**
 * Destruye la tabla.
 */
void tablahash_destruir(TablaHash tabla) {

  // Destruir cada uno de los datos.
  for (unsigned idx = 0; idx < tabla->capacidad; ++idx)
    if (tabla->elems[idx] != NULL)
      glist_destruir(tabla->elems[idx], (FuncionDestructora) tabla->destr);

  // Liberar el arreglo de casillas y la tabla.
  free(tabla->elems);
  free(tabla);
  return;
}

/**
 * Inserta un dato en la tabla, o lo reemplaza si ya se encontraba.
 */
void tablahash_insertar(TablaHash tabla, void *dato) {

  if (tabla->numElems * 10 >= tabla->capacidad * 5)
    tablahash_redimensionar(tabla);

  // Calculamos la posicion del dato dado, de acuerdo a la funcion hash.
  unsigned idx = tabla->hash(dato) % tabla->capacidad;

  // SI la palabra ya se encuentra en la tabla, no hace nada
  if (glist_buscar(tabla->elems[idx],dato,(FuncionComparadora)tabla->comp)){
    return;
  }

  // En caso contrario, la agrega a la tabla
  else{
    tabla->numElems++;
    tabla->elems[idx] = glist_agregar_final(tabla->elems[idx], dato,(FuncionCopiadora) tabla->copia);
    return;
  }
}


/**
 * Retorna el dato de la tabla que coincida con el dato dado, o NULL si el dato
 * buscado no se encuentra en la tabla.
 */
int tablahash_buscar(TablaHash tabla, void *dato) {

  // Calculamos la posicion del dato dado, de acuerdo a la funcion hash.
  unsigned idx = tabla->hash(dato) % tabla->capacidad;
  
  // Retornar NULL si la casilla estaba vacia.
  return glist_buscar(tabla->elems[idx],dato, 
                      (FuncionComparadora)tabla->comp);
}

/**
 * Elimina el dato de la tabla que coincida con el dato dado.
 */
void tablahash_eliminar(TablaHash tabla, void *dato) {

  // Calculamos la posicion del dato dado, de acuerdo a la funcion hash.
  unsigned idx = tabla->hash(dato) % tabla->capacidad;

  // Retornar si la casilla estaba vacia.
  if (glist_vacia(tabla->elems[idx]))
    return;
    
  // Eliminar el elemento de la lista de la casilla si hay coincidencia
  else if (glist_buscar(tabla->elems[idx],dato,(FuncionComparadora)tabla->comp)) {
    tabla->numElems--;
    tabla->elems[idx] = glist_eliminar(tabla->elems[idx], dato,
    (FuncionDestructora) tabla->destr, (FuncionComparadora) tabla->comp);
    return;
  }
  else{
    return;
  }
}

// void tablahash_imprimir(TablaHash tabla, FuncionVisitante visit){
//   for(unsigned idx = 0; idx < tabla->capacidad; idx++){
//     printf("Casilla %d: ", idx);
//     glist_recorrer(tabla->elems[idx],visit);
//     printf("\n");
//   }
//   return;
// }

void tablahash_redimensionar(TablaHash tabla) {
  GList* tmp_elems = tabla->elems;
  unsigned tmp_capacidad = tabla->capacidad;
  tabla->numElems = 0;
  tabla->capacidad *= 2;
  tabla->elems = calloc(tabla->capacidad, sizeof(struct _GNode));
  assert(tabla->elems != NULL);
  
  // Itera sobre cada nodo de cada lista, agregando cada
  // elemento a la tabla redimensionada
  for (unsigned i = 0; i < tmp_capacidad; ++i) {
    GList node = tmp_elems[i], tmp;
    while (node != NULL) {
      tablahash_insertar(tabla, node->data);
      tmp = node->next;
      tabla->destr(node->data);
      free(node);
      node = tmp;
    }
  }
  free(tmp_elems);
  return;
}