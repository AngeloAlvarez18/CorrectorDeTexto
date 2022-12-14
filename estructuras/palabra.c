#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include "palabra.h"
#include "tablahash.h"
#include "glist.h"


Palabra palabra_crear(char *palabra, unsigned len) {
  Palabra nueva_palabra = malloc(sizeof(struct _Palabra));
  assert(nueva_palabra != NULL);

  nueva_palabra->str = malloc(sizeof(char) * (len + 1));
  assert(nueva_palabra->str != NULL);
  nueva_palabra->str = memcpy(nueva_palabra->str, palabra, len);
  nueva_palabra->str[len] = '\0';

  nueva_palabra->len = len;

  return nueva_palabra;
}

Palabra palabra_copia(Palabra palabra) {
  Palabra copia = palabra_crear(palabra->str, palabra->len);
  return copia;
}


void palabra_destruir(Palabra palabra) {
  free(palabra->str);
  free(palabra);
}


int palabra_comparar(Palabra palabra1, Palabra palabra2) {
  if (palabra1->len != palabra2->len)
    return 1;

  return strcmp(palabra1->str, palabra2->str);
}

void palabra_dividir(Palabra palabra, TablaHash tabla, Sugerencia sug) {
  char *buff1 = malloc(sizeof(char) * (palabra->len + 2));
  assert(buff1 != NULL);
  char *buff2 = malloc(sizeof(char) * palabra->len);
  assert(buff2 != NULL);
  Palabra word;

  for (unsigned i = 1; i < palabra->len; i++) {
    memcpy(buff1, palabra->str, i);
    buff1[i] = '\0';
    // Tomamos la primera palabra dividida.
    Palabra palabra1 = palabra_crear(buff1, i);

    memcpy(buff2, palabra->str + i, palabra->len - i);
    buff2[palabra->len - i] = '\0';
    // Tomamos la segunda palabra dividida.
    Palabra palabra2 = palabra_crear(buff2, palabra->len - i);

    // Nos fijamos si ambas palabras estan en el diccionario.
    if (tablahash_buscar(tabla, palabra1) && tablahash_buscar(tabla, palabra2)) {
      buff1[i] = ' ';
      // Unimos las dos palabras y le agregamos un espacio.
      memcpy(buff1 + i + 1, buff2, palabra->len - i + 1);
      word = palabra_crear(buff1, palabra->len + 1);
      // Agregamos esta palabra a las sugerencias.
      sug->list = glist_agregar_final(sug->list, word, tabla->copia);
      sug->cant_sug++;
      palabra_destruir(word);
    }

    palabra_destruir(palabra1);
    palabra_destruir(palabra2);

    // SI ya hay 5 sugerencias, no seguimos buscando.
    if (sug->cant_sug >= 5) {
      free(buff1);
      free(buff2);
      return;
    }
  }

  free(buff1);
  free(buff2);
  return;
}

void palabra_permutar(Palabra palabra, TablaHash tabla, Sugerencia sug,
                             GList * not_found, TablaHash tne, int dist) {
  char aux;

  for (unsigned i = 1; i < palabra->len; i++) {
    aux = palabra->str[i - 1];
    if (aux != palabra->str[i]) {
      // Permutamos los caracteres adyacentes.
      palabra->str[i - 1] = palabra->str[i];
      palabra->str[i] = aux;
      // Chequeamos si la palabra esta en el diccionario.
      if (tablahash_buscar(tabla, palabra)
          && !glist_buscar(sug->list, palabra, tabla->comp)) {
        // Agregamos la palabra a las sugerencias.            
        sug->list = glist_agregar_final(sug->list, palabra,
                                (FuncionCopiadora) palabra_copia);
        sug->cant_sug++;
      }

      // Si no esta en el diccionario, agregamos la palabra en una lista
      // para posteriormente aplicarle otra vez las reglas a esta palabra.
      if ((dist < 3) && !tablahash_buscar(tne, palabra)) {
        *not_found = glist_agregar_final(*not_found, palabra, tabla->copia);
        tablahash_insertar(tne, palabra);
      }

      // Volvemos la palabra a como estaba originalmente.
      palabra->str[i] = palabra->str[i - 1];
      palabra->str[i - 1] = aux;
      if (sug->cant_sug >= 5)
        return;
    }
  }

  return;
}


void palabra_borrar_caracter(Palabra palabra, TablaHash tabla,
                                    Sugerencia sug, GList * not, TablaHash tne,
                                    int dist) {
  char *buff = malloc(sizeof(char) * palabra->len);
  assert(buff != NULL);
  Palabra word;

  for (unsigned i = 0; i < palabra->len; i++) {
    // Copiamos los caracteres de la palabra menos el que
    // queremos eliminar.
    memcpy(buff, palabra->str, i);
    memcpy(buff + i, palabra->str + i + 1, palabra->len - i);
    word = palabra_crear(buff, palabra->len - 1);
    // Chequeamos si la palabra esta en el dicccionario.
    if (tablahash_buscar(tabla, word)
        && !glist_buscar(sug->list, word, tabla->comp)) {
      sug->list = glist_agregar_final(sug->list, word, tabla->copia);
      sug->cant_sug++;
    }

    // Si no esta en el diccionario, agregamos la palabra en una lista
    // para posteriormente aplicarle otra vez las reglas a esta palabra.
    if ((dist < 3) && !tablahash_buscar(tne, palabra)) {
      *not = glist_agregar_final(*not, word, tabla->copia);
      tablahash_insertar(tne, word);
    }
    palabra_destruir(word);

    if (sug->cant_sug >= 5) {
      free(buff);
      return;
    }
  }
  free(buff);
  return;
}

void palabra_cambiar_caracter(Palabra palabra, TablaHash tabla,
                                     Sugerencia sug, GList *not,
                                     TablaHash tne, int dist) {
  char aux;

  for (unsigned i = 0; i < palabra->len; i++) {
    aux = palabra->str[i];
    for (char c = 'a'; c <= 'z'; c++) {
      // Cambiamos el caracter.
      palabra->str[i] = c;
      // Nos fijamos si la palabra esta en el diccionario.
      if (tablahash_buscar(tabla, palabra)
          && !glist_buscar(sug->list, palabra, tabla->comp)) {
        // SI esta, agregamos la palabra a las sugerencias.            
        sug->list = glist_agregar_final(sug->list, palabra,
                                      (FuncionCopiadora) palabra_copia);
        sug->cant_sug++;
      }
      // Si no esta en el diccionario, agregamos la palabra en una lista
      // para posteriormente aplicarle otra vez las reglas a esta palabra.
      if ((dist < 3) && !tablahash_buscar(tne, palabra)) {
        *not = glist_agregar_final(*not, palabra, tabla->copia);
        tablahash_insertar(tne, palabra);
      }

      if (sug->cant_sug >= 5) {
        palabra->str[i] = aux;
        return;
      }
    }
    // Volvemos a reemplazar el caracter a como estaba originalmente
    palabra->str[i] = aux;
  }
  return;
}

void palabra_agregar_caracter(Palabra palabra, TablaHash tabla,
                                     Sugerencia sug, GList * not,
                                     TablaHash tne, int dist) {
  char *buff = malloc(sizeof(char) * (palabra->len + 2));
  assert(buff != NULL);
  Palabra word;

  for (unsigned i = 0; i <= palabra->len; i++) {
    for (char c = 'a'; c <= 'z'; c++) {
      // Agregamos el caracter en la posicion i de la palabra.
      memcpy(buff, palabra->str, i);
      buff[i] = c;
      memcpy(buff + i + 1, palabra->str + i, palabra->len - i + 1);
      word = palabra_crear(buff, palabra->len + 1);
      // Nos fijamos si la palabra esta en el diccionario.
      if (tablahash_buscar(tabla, word)
          && !glist_buscar(sug->list, word, tabla->comp)) {
        // Si esta, agregamos la palabra a las sugerencias.
        sug->list = glist_agregar_final(sug->list, word, tabla->copia);
        sug->cant_sug++;
      }

      // Si no esta en el diccionario, agregamos la palabra en una lista
      // para posteriormente aplicarle otra vez las reglas a esta palabra.
      if ((dist < 3) && !tablahash_buscar(tne, palabra)) {
        *not = glist_agregar_final(*not, word, tabla->copia);
        tablahash_insertar(tne, word);
      }

      palabra_destruir(word);
      if (sug->cant_sug >= 5) {
        free(buff);
        return;
      }
    }
  }
  free(buff);
  return;
}
