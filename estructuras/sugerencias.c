#include "sugerencias.h"
#include "palabra.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>


Sugerencia crear_sugerencias(char *word, int cant_sug, int cache) {
  Sugerencia sug = malloc(sizeof(struct _Sugerencias));
  assert(sug != NULL);

  sug->palabra = malloc(sizeof(char) * 30);
  strcpy(sug->palabra, word);
  sug->list = glist_crear();
  sug->cant_sug = cant_sug;
  sug->cache = cache;
  return sug;
}


void sugerencias_destruir(Sugerencia sug) {
  free(sug->palabra);
  glist_destruir(sug->list, (FuncionDestructora) palabra_destruir);
  free(sug);
  return;
}

int sugerencia_comparar(Sugerencia sug1, Sugerencia sug2) {
  return strcmp(sug1->palabra, sug2->palabra);
}

Sugerencia sugerencia_copia(Sugerencia sug) {
  Sugerencia sug_copia = crear_sugerencias(sug->palabra, sug->cant_sug,
                                            sug->cache);
                                            
  for (GList node = sug->list; node != NULL; node = node->next)
    sug_copia->list = glist_agregar_final(sug_copia->list, node->data,
                                          (FuncionCopiadora) palabra_copia);
  return sug_copia;
}
