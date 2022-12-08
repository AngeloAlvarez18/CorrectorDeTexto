#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "tablahash.h"
#include "glist.h"
#include "utils.h"
#include "palabra.h"

/** Capacidad inicial para la tabla hash */
#define CAPACIDAD_INICIAL 20

int main() {
  
  // Diccionario que usaremos
  char* dicc = "diccionario.txt";

  //Iniciar tabla hash
  TablaHash tabla = tablahash_crear(CAPACIDAD_INICIAL,
                  (FuncionComparadora) palabra_comparar, 
                  (FuncionDestructora) palabra_destruir,
                  (FuncionHash) djb2, (FuncionCopia) palabra_copia);

  // Leemos el diccionario y lo guardamos en la tabla
  leer_diccionario(dicc, tabla);

  readfile2("test.txt", "salida.txt",tabla);

  //Liberar memoria
  tablahash_destruir(tabla);
  // TablaHash tabla;
  // FILE* cache = fopen("cache.txt", "r");
  // tabla = leer_cache(cache);
  // tablahash_imprimir(tabla, (FuncionVisitante)sugerencia_imprimir_palabra);
  // fclose(cache);
  return 0;
}