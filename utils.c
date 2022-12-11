#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "palabra.h"
#include "sugerencias.h"
#include "io.h"

unsigned djb2(Palabra word) {
  unsigned hash = 5381;
  int c;
  char *str = word->str;

  while ((c = *str++))
    hash = ((hash << 5) + hash) + c;    /* hash * 33 + c */

  return hash;
}

unsigned djb2_sug(Sugerencias sug) {
  unsigned hash = 5381;
  int c;
  char *str = sug->palabra;

  while ((c = *str++))
    hash = ((hash << 5) + hash) + c;    /* hash * 33 + c */

  return hash;
}

Sugerencias distancia_n(Palabra palabra, TablaHash tabla, Sugerencias sug,
                        GList * no_encontradas, TablaHash tne, int dist) {

  if (sug->cant_sug < 5)
    sug = palabra_dividir(palabra, tabla, sug);

  if (sug->cant_sug < 5)
    sug = palabra_permutar(palabra, tabla, sug, no_encontradas, tne, dist);

  if (sug->cant_sug < 5)
    sug = palabra_borrar_caracter(palabra, tabla, sug, no_encontradas,
                                  tne, dist);

  if (sug->cant_sug < 5)
    sug = palabra_cambiar_caracter(palabra, tabla, sug, no_encontradas, tne,
                                  dist);

  if (sug->cant_sug < 5)
    sug = palabra_agregar_caracter(palabra, tabla, sug, no_encontradas, tne,
                                   dist);

  return sug;

}

Sugerencias buscar_sugerencias(Palabra palabra, TablaHash tabla, Sugerencias sug,
                                TablaHash chequeadas, char* cache) {

  // Lista en la que guardaremos palabras a las cuales se les aplicará 
  // las reglas 2 y 3 en caso de ser necesario
  GList no_encontradas = glist_crear();
  // Tabla en la que se guardaran las palabras a
  // las cuales se les aplico alguna regla
  TablaHash tabla_no_encontradas = tablahash_crear(10,
                                  (FuncionComparadora)palabra_comparar,
                                  (FuncionDestructora)palabra_destruir,
                                  (FuncionHash) djb2,
                                  (FuncionCopiadora)palabra_copia);

  int bandera = 1, dist = 1;
  // Buscamos sugerencias a distancia 1
  sug = distancia_n(palabra, tabla, sug, &no_encontradas, tabla_no_encontradas,
                    dist);
  dist++;

  GList no_encontradas2;
  GList aux;

  // No se encontraron 5 sugerencias, asi que buscamos sugerencias
  // a distancia 2 y 3 
  if (sug->cant_sug < 5) {
    while (bandera && dist <= 3) {
      no_encontradas2 = glist_crear();
      for (GList node = no_encontradas; node != NULL && sug->cant_sug < 5;) {
        sug = distancia_n(node->data, tabla, sug, &no_encontradas2,
                          tabla_no_encontradas, dist);
        node = node->next;
      }
      aux = no_encontradas;
      no_encontradas = no_encontradas2;
      glist_destruir(aux, tabla->destr);
      if (sug->cant_sug >= 5)
        bandera = 0;
      dist++;
    }
  }
  
  escribir_cache(cache, palabra->str, sug->cant_sug, sug->list);

  tablahash_insertar(chequeadas, sug);
  glist_destruir(no_encontradas, tabla->destr);
  tablahash_destruir(tabla_no_encontradas);
  return sug;   
}

Sugerencias buscar_en_cache(TablaHash cache, Sugerencias sug){
  unsigned idx = cache->hash(sug) % cache->capacidad;
  GList lista = cache->elems[idx];
  Sugerencias sug_ret = sug;
  int flag = 1;
  for (GList node = lista; node != NULL && flag; node = node->next){  
    if (strcmp(((Sugerencias)(node->data))->palabra, sug->palabra) == 0){
      sug_ret = node->data;
      sugerencias_destruir(sug);
      flag = 0;
    }
  }
  return sug_ret;
}

void error(){
  puts("Error:");
  puts("Para ejecutar el programa correctamente:");
  puts("main archivo_entrada archivo_salida archivo_cache");
  exit(EXIT_FAILURE);
}

void quit(char* str){
  perror(str);
  exit(EXIT_FAILURE);
}