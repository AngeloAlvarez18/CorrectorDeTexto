#include "sugerencias.h"
#include "palabra.h"
#include <stdlib.h>
#include <string.h>


Sugerencias crear_sugerencias(char* word, int cant_sug, int cache){
    Sugerencias sug = malloc(sizeof(struct _Sugerencias));

    sug->palabra = malloc(sizeof(char) * 30);
    strcpy(sug->palabra, word);
    sug->list = glist_crear();
    sug->cant_sug = cant_sug;
    sug->cache = cache;
    return sug;
}


void sugerencias_destruir(Sugerencias sug){
  free(sug->palabra);
  glist_destruir(sug->list, (FuncionDestructora)palabra_destruir);
  free(sug);
  return;
}

int sugerencia_comparar(Sugerencias sug1, Sugerencias sug2){
  return strcmp(sug1->palabra, sug2->palabra);
}

Sugerencias sugerencia_copia(Sugerencias sug){
  Sugerencias sug_copia = crear_sugerencias(sug->palabra, sug->cant_sug, sug->cache);
  for (GList node = sug->list; node != NULL; node = node->next)
    sug_copia->list = glist_agregar_final(sug_copia->list, node->data,
                                        (FuncionCopiadora)palabra_copia);
  return sug_copia;
}