#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include "utils.h"
#include "palabra.h"
#include "tablahash.h"
#include "glist.h"


/**
 * Funcion de hash para strings propuesta por Kernighan & Ritchie en "The C
 * Programming Language (Second Ed.)".
 */
unsigned KRHash(Palabra word) {
  unsigned hashval;
  char *s = word->str;
  for (hashval = 0; *s != '\0'; ++s) {
    hashval = *s + 31 * hashval;
  }
  return hashval;
}


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

// Lee y agrega las palabras al diccionario
void leer_diccionario(char *path, TablaHash tabla) {
  FILE *file = fopen(path, "r");
  unsigned size = 30;
  char buff[size];
  int c;
  size_t i = 0;

  while ((c = getc(file)) != EOF) {
    // Si no llega al final de linea, sigue leyendo
    if (c != '\n')
      buff[i++] = c;

    // Llega al final de linea, agrega el caracter terminador
    // y luego agrega la palabra al diccionario
    else {
      buff[i] = '\0';
      Palabra palabra = palabra_crear(buff, i);
      tablahash_insertar(tabla, palabra);
      palabra_destruir(palabra);
      i = 0;
    }
  }
  fclose(file);
  return;
}

void readfile(char *entrada, char *salida, TablaHash tabla) {
  FILE *file1 = fopen(entrada, "r");
  FILE *file2 = fopen(salida, "w");
  int linea = 1, flag = 1;
  Palabra palabra;
  Sugerencias sugerencias;
  TablaHash palabras_chequeadas = tablahash_crear(10,
                                  (FuncionComparadora)palabra_comparar,
                                  (FuncionDestructora)palabra_destruir,
                                  (FuncionHash) djb2,
                                  (FuncionCopia) palabra_copia);

  // FILE* arch = fopen("cache.txt", "r");
  // TablaHash cache = leer_cache(arch);
  // fclose(arch);

  while (flag) {
    palabra = leer_palabra(file1, &linea, &flag);
    sugerencias = crear_sugerencias(palabra->str,0,0);
    if (!tablahash_buscar(palabras_chequeadas, palabra)
        && !tablahash_buscar(tabla, palabra)) {
      fprintf(file2, "Linea %d, %s no esta en el diccionario.\n", linea,
              palabra->str);
      buscar_sugerencias(palabra, tabla, sugerencias, file2,
                         palabras_chequeadas);
      fprintf(file2, "\n\n");
    }
    palabra_destruir(palabra);
    sugerencias_destruir(sugerencias);
  }
  tablahash_destruir(palabras_chequeadas);
  fclose(file1);
  fclose(file2);
  return;
}

void readfile2(char *entrada, char *salida, TablaHash tabla) {
  FILE *file1 = fopen(entrada, "r");
  FILE *file2 = fopen(salida, "w");
  int linea = 1, flag = 1;
  Palabra palabra;
  Sugerencias sugerencias;

  // Tabla en donde se va a almacenar palabras a las que ya se le
  // encontraron sugerencias o palabras que estan en el cache
  TablaHash chequeadas = tablahash_crear(10,
                                (FuncionComparadora)sugerencia_comparar,
                                (FuncionDestructora)sugerencias_destruir,
                                (FuncionHash) djb2_sug,
                                (FuncionCopia) sugerencia_copia);
  
  FILE* arch = fopen("cache.txt", "r");
  leer_cache(arch, chequeadas);
  fclose(arch);

  while (flag) {
    palabra = leer_palabra(file1, &linea, &flag);
    sugerencias = crear_sugerencias(palabra->str,0,0);

    // Nos fijamos si la palabra no esta en el diccionario
    if (!tablahash_buscar(tabla, palabra)){
                                                                                   
      // Nos fijamos si ya chequeamos la palabra
      if (tablahash_buscar(chequeadas,sugerencias)){
        sugerencias = buscar_en_cache(chequeadas, sugerencias);
        // La palabra esta en el cache, asi que la escribimos
        // en el archivo de salida.
        if (sugerencias->cache)
          escribir_sugerencias(file2, sugerencias,linea);
      }
      // La palabra no la chequeamos, le buscamos sugerencias
      else{
      sugerencias = buscar_sugerencias2(palabra,tabla,sugerencias,chequeadas);
      escribir_sugerencias(file2, sugerencias,linea);
      }
    }
    palabra_destruir(palabra);
    //sugerencias_destruir(sugerencias);
  }
  // ARREGLAR ESTO
  tablahash_destruir(chequeadas);
  fclose(file1);
  fclose(file2);
  return;
}

// void readfile_mejorado(char *entrada, char *salida, TablaHash tabla) {
//   FILE *file1 = fopen(entrada, "r");
//   FILE *file2 = fopen(salida, "w");
//   int linea = 1, flag = 1;
//   Palabra palabra;
//   Sugerencias sugerencias;
//   TablaHash palabras_chequeadas = tablahash_crear(10,
//                                   (FuncionComparadora)sugerencia_comparar,
//                                   (FuncionDestructora)sugerencias_destruir,
//                                   (FuncionHash) djb2_sug,
//                                   (FuncionCopia) sugerencia_copia);
//   FILE* arch = fopen("cache.txt", "r");
//   leer_cache(arch, palabras_chequeadas);
//   fclose(arch);
  
//   while (flag){
//     palabra = leer_palabra(file1, &linea, &flag);
//     sugerencias = crear_sugerencias(palabra->str,0,0);
//     if (!tablahash_buscar(tabla, palabra) && 
//         !tablahash_buscar(palabras_chequeadas, palabra->str)){
      
//       // Nos fijamos si la palabra esta en el cache
//       if (tablahash_buscar(cache[0], palabra)){
//         sugerencias = buscar_en_cache(cache[1], palabra);
//         escribir_sugerencias(file2, sugerencias, linea);
//       }
//       else{
//         sugerencias = buscar_sugerencias2(palabra, tabla, sugerencias,
//                           palabras_chequeadas);
//         escribir_sugerencias(file2, sugerencias, linea);                          
//       }
//     }
//     palabra_destruir(palabra);
//     sugerencias_destruir(sugerencias);
//   }
//   tablahash_destruir(tabla);
//   fclose(file1);
//   fclose(file2);
//   return;
// }

Palabra leer_palabra(FILE * archivo, int *linea, int *flag) {
  char buff[30];
  int i = 0;
  char c;
  Palabra palabra;

  while ((c = getc(archivo)) != EOF) {
    c = tolower(c);

    if (isalpha(c)) {
      buff[i++] = c;
    } else {
      buff[i] = '\0';
      palabra = palabra_crear(buff, i);
      if (c == '\n')
        (*linea)++;
      return palabra;
    }
  }
  if (c == EOF) {
    *flag = 0;
  }

  buff[i] = '\0';
  palabra = palabra_crear(buff, i);
  return palabra;
}


void leer_cache(FILE * archivo, TablaHash cache) {
  char buff[30];
  char c;
  int i = 0, count = 0, cant_sug;
  char palabra[30];
  Sugerencias sug;
  Palabra word;
  // jsdnsaj, 5, judas aj, sansa, sonsa, sonsas, sansas, 

  while ((c = getc(archivo)) != EOF) {

    // Leyendo la primera palabra
    if ((count == 0) && (c != ',') && (isalpha(c)))
      buff[i++] = c;

    // Termina de leer la primera palabra
    else if (count == 0) {
      count++;
      buff[i] = '\0';
      if(buff[0] == ' ')
        memcpy(palabra, buff+1, i + 1);
      else
        memcpy(palabra, buff, i + 1);
      i = 0;
    }

    // Leyendo la cantidad de sugerencias
    if ((count == 1) && (c != ',') && isalnum(c)) {
      cant_sug = c - '0';
      count++;
      sug = crear_sugerencias(palabra,cant_sug,1);
    }

    // Leyendo las sugerencias
    if ((count == 2) && (c != ',') && ((c == ' ') || isalpha(c))) {
      buff[i++] = c;
    }
    
    // Termina de leer una sugerencia
    else if ((count == 2) && (c == ',')) {
      buff[i] = '\0';
      word = palabra_crear(buff+1, i);
      // IMPORTANTE: ARREGLAR, ESTA LEYENDO EL ESPACIO DE ANTES
      // DE LAS PALABRAS
      if (word->len)
        sug->list = glist_agregar_inicio(sug->list, word, (FuncionCopia) palabra_copia);
      palabra_destruir(word);
      i = 0;
    }

    if ((count == 2) && (c == '\n')) {
      tablahash_insertar(cache, sug);
      sugerencias_destruir(sug);
      count = 0;
    }
  }
  
  return;
}


void escribir_cache(FILE * arch, char *palabra, int cant_sugs, GList list) {
  if (cant_sugs == 0) {
    fprintf(arch, "%s, ", palabra);
    fprintf(arch, "%d, \n", cant_sugs);
    return;
  }
  fprintf(arch, "%s, ", palabra);
  fprintf(arch, "%d, ", cant_sugs);
  for (GList node = list; node != NULL; node = node->next)
    fprintf(arch, "%s, ", ((Palabra) (node->data))->str);
  fprintf(arch, "\n");
  return;
}

Sugerencias distancia_n(Palabra palabra, TablaHash tabla, Sugerencias sug,
                        GList * no_encontradas, TablaHash tne, int dist) {

  if (sug->cant_sug < 5)
    sug = palabra_dividir(palabra, tabla, sug);

  if (sug->cant_sug < 5)
    sug = palabra_permutar(palabra, tabla, sug, no_encontradas, tne, dist);

  if (sug->cant_sug < 5)
    sug =
        palabra_borrar_caracter(palabra, tabla, sug, no_encontradas, tne, dist);

  if (sug->cant_sug < 5)
    sug =
        palabra_cambiar_caracter(palabra, tabla, sug, no_encontradas, tne,
                                 dist);

  if (sug->cant_sug < 5)
    sug =
        palabra_agregar_caracter(palabra, tabla, sug, no_encontradas, tne,
                                 dist);

  return sug;

}

void buscar_sugerencias(Palabra palabra, TablaHash tabla, Sugerencias sug,
                        FILE * file, TablaHash chequeadas) {

  // Lista en la que guardaremos palabras a las cuales se les aplicará 
  // las reglas 2 y 3 en caso de ser necesario
  GList no_encontradas = glist_crear();
  // Tabla en la que se guardaran las palabras a
  // las cuales se les aplico alguna regla
  TablaHash tabla_no_encontradas = tablahash_crear(10,
                                  (FuncionComparadora)palabra_comparar,
                                  (FuncionDestructora)palabra_destruir,
                                  (FuncionHash) djb2,
                                  (FuncionCopia)palabra_copia);

  int bandera = 1, dist = 1;
  // Buscamos sugerencias a distancia 1
  sug =
      distancia_n(palabra, tabla, sug, &no_encontradas, tabla_no_encontradas,
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
        sug =
            distancia_n(node->data, tabla, sug, &no_encontradas2,
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
  FILE *cache = fopen("cache.txt", "a");

  escribir_cache(cache, palabra->str, sug->cant_sug, sug->list);

  if (sug->cant_sug <= 0)
    fprintf(file, "No se encontraron sugerencias para esta palabra");

  else {
    fprintf(file, "Quizas quiso decir: ");
    for (GList node = sug->list; node != NULL; node = node->next){
      //printf("%s\n", ((Palabra) node->data)->str);
      fprintf(file, "%s, ", ((Palabra) node->data)->str);
    }
      
  }
  tablahash_insertar(chequeadas, palabra);
  fclose(cache);
  glist_destruir(no_encontradas, tabla->destr);
  tablahash_destruir(tabla_no_encontradas);
  return;
}


Sugerencias buscar_sugerencias2(Palabra palabra, TablaHash tabla, Sugerencias sug,
                                TablaHash chequeadas) {
  // Lista en la que guardaremos palabras a las cuales se les aplicará 
  // las reglas 2 y 3 en caso de ser necesario
  GList no_encontradas = glist_crear();
  // Tabla en la que se guardaran las palabras a
  // las cuales se les aplico alguna regla
  TablaHash tabla_no_encontradas = tablahash_crear(10,
                                  (FuncionComparadora)palabra_comparar,
                                  (FuncionDestructora)palabra_destruir,
                                  (FuncionHash) djb2,
                                  (FuncionCopia)palabra_copia);

  int bandera = 1, dist = 1;
  // Buscamos sugerencias a distancia 1
  sug =
      distancia_n(palabra, tabla, sug, &no_encontradas, tabla_no_encontradas,
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
        sug =
            distancia_n(node->data, tabla, sug, &no_encontradas2,
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
  FILE *cache = fopen("cache.txt", "a");

  escribir_cache(cache, palabra->str, sug->cant_sug, sug->list);
  fclose(cache);

  tablahash_insertar(chequeadas, sug);
  glist_destruir(no_encontradas, tabla->destr);
  tablahash_destruir(tabla_no_encontradas);
  return sug;   
}

void escribir_sugerencias(FILE* arch, Sugerencias sug, int linea){
  if (sug->cant_sug == 0){
    fprintf(arch, "Linea %d, %s no esta en el diccionario.\n", linea,
            sug->palabra);
    fprintf(arch, "No se encontraron sugerencias para esta palabra\n\n");
  }
  else{
    fprintf(arch, "Linea %d, %s no esta en el diccionario.\n", linea,
            sug->palabra);
    fprintf(arch, "Quizas quiso decir: ");
    for (GList node = sug->list; node != NULL; node = node->next)
      fprintf(arch, "%s, ", ((Palabra) node->data)->str);
    fprintf(arch, "\n\n");
  }
  return;
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