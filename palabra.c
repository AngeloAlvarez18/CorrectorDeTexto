#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include "palabra.h"
#include "tablahash.h"
#include "glist.h"
#include "utils.h"


Palabra palabra_crear(char* palabra, unsigned len){
    Palabra nueva_palabra = malloc(sizeof(struct _Palabra));
    assert(nueva_palabra != NULL);
    
    nueva_palabra->str = malloc(sizeof(char) * (len + 1));
    assert(nueva_palabra->str != NULL);
    nueva_palabra->str = memcpy(nueva_palabra->str, palabra, len);
    nueva_palabra->str[len] = '\0';

    nueva_palabra->len = len;

    return nueva_palabra;
}

Palabra palabra_copia(Palabra palabra){
    Palabra copia = palabra_crear(palabra->str, palabra->len);
    return copia;
}

Palabra palabra_identidad(Palabra palabra){
  return palabra;
}

void palabra_destruir(Palabra palabra){
    free(palabra->str);
    free(palabra);
}

void palabra_imprimir(Palabra palabra){
    printf("%s ", palabra->str);
    printf("\n");
    return;
}

int palabra_comparar(Palabra palabra1, Palabra palabra2){
    if (palabra1->len != palabra2->len)
      return 1;

    return strcmp(palabra1->str, palabra2->str);
}

Sugerencias palabra_dividir(Palabra palabra, TablaHash tabla, Sugerencias sug){
    char* buff1 = malloc(sizeof(char) * (palabra->len + 2));
    char* buff2 = malloc(sizeof(char) * palabra->len);
    Palabra word;

    for(unsigned i = 1; i < palabra->len; i++){
        memcpy(buff1, palabra->str, i);
        buff1[i] = '\0';
        Palabra palabra1 = palabra_crear(buff1, i);

        memcpy(buff2, palabra->str + i, palabra->len - i);
        buff2[palabra->len - i] = '\0';
        Palabra palabra2 = palabra_crear(buff2, palabra->len - i);

        if(tablahash_buscar(tabla, palabra1) && tablahash_buscar(tabla, palabra2)){
            buff1[i] = ' ';
            memcpy(buff1 + i + 1, buff2, palabra->len - i + 1);
            word = palabra_crear(buff1, palabra->len + 1);
            sug->list = glist_agregar_final(sug->list, word, tabla->copia);
            sug->cant_sug++;
            palabra_destruir(word);                
        }
        
        palabra_destruir(palabra1);
        palabra_destruir(palabra2);

        if (sug->cant_sug >= 5){
            free(buff1);
            free(buff2);        
            return sug;
        }
    }
    
    free(buff1);
    free(buff2);
    return sug;
}

Sugerencias palabra_permutar(Palabra palabra, TablaHash tabla, Sugerencias sug,
                             GList* not_found, TablaHash tne, int dist){
  char aux;

  for(unsigned i = 1; i < palabra->len; i++){
    aux = palabra->str[i-1];
    if(aux != palabra->str[i]){
      palabra->str[i-1] = palabra->str[i];
      palabra->str[i] = aux;
      if (tablahash_buscar(tabla, palabra) && !glist_buscar(sug->list,palabra,tabla->comp)){
        sug->list = glist_agregar_final(sug->list, palabra, (FuncionCopia)palabra_copia);
        sug->cant_sug++;
      }

      if((dist < 3) && !tablahash_buscar(tne, palabra)){
        *not_found = glist_agregar_final(*not_found, palabra, tabla->copia);
        tablahash_insertar(tne, palabra);
      }
      
      palabra->str[i] = palabra->str[i-1];
      palabra->str[i-1] = aux;
      if (sug->cant_sug >= 5)
        return sug;
    }
  }

  return sug;
}


Sugerencias palabra_borrar_caracter(Palabra palabra, TablaHash tabla, Sugerencias sug,
                                    GList* not, TablaHash tne, int dist){
    char* buff = malloc(sizeof(char) * palabra->len);
    Palabra word;

    for(unsigned i = 0; i < palabra->len; i++){
        memcpy(buff,palabra->str, i);
        memcpy(buff + i, palabra->str + i + 1, palabra->len - i);
        word = palabra_crear(buff, palabra->len - 1);
        if (tablahash_buscar(tabla, word) && !glist_buscar(sug->list,word,tabla->comp)){
            sug->list = glist_agregar_final(sug->list, word, tabla->copia);
            sug->cant_sug++;
        }

        if((dist < 3) && !tablahash_buscar(tne, palabra)){
            *not = glist_agregar_final(*not, word, tabla->copia);
            tablahash_insertar(tne, word);
        }
        palabra_destruir(word);
        
        if(sug->cant_sug >= 5){
            free(buff);
            return sug;
        }
    }
    free(buff);
    return sug;
}

Sugerencias palabra_cambiar_caracter(Palabra palabra, TablaHash tabla, Sugerencias sug,
                                    GList* not, TablaHash tne, int dist){
  char aux;

  for(unsigned i = 0; i < palabra->len; i++){
    aux = palabra->str[i];
    for(char c = 'a'; c <= 'z'; c++){
      palabra->str[i] = c;
      if(tablahash_buscar(tabla, palabra) && !glist_buscar(sug->list,palabra,tabla->comp)){
        sug->list = glist_agregar_final(sug->list, palabra, (FuncionCopia)palabra_copia);
        sug->cant_sug++;
      }
      
      if((dist < 3) && !tablahash_buscar(tne, palabra)) {
        *not = glist_agregar_final(*not, palabra, tabla->copia);
        tablahash_insertar(tne, palabra);
      }

      if (sug->cant_sug >= 5){
        palabra->str[i] = aux;
        return sug;
      }        
    }    
    palabra->str[i] = aux;   
  }
  return sug;
}

Sugerencias palabra_agregar_caracter(Palabra palabra, TablaHash tabla, Sugerencias sug,
                                    GList* not, TablaHash tne, int dist){
    char* buff = malloc(sizeof(char) * (palabra->len + 2));
    Palabra word;

    for(unsigned i = 0; i <= palabra->len; i++){
        for(char c = 'a'; c <= 'z'; c++){
            memcpy(buff,palabra->str, i);
            buff[i] = c;
            memcpy(buff + i + 1, palabra->str + i, palabra->len - i + 1);
            word = palabra_crear(buff, palabra->len + 1);
            if (tablahash_buscar(tabla, word) && !glist_buscar(sug->list,word,tabla->comp)){
                sug->list = glist_agregar_final(sug->list, word, tabla->copia);
                sug->cant_sug++;
            }          
            
            if((dist < 3) && !tablahash_buscar(tne, palabra)){
                *not = glist_agregar_final(*not, word, tabla->copia);
                tablahash_insertar(tne, word);
            }
            
            palabra_destruir(word);
            if (sug->cant_sug >= 5){
                free(buff);
                return sug;
            }
        }
    }
    free(buff);
    return sug;
}

Sugerencias crear_sugerencias(char* word, int cant_sug, int cache){
    Sugerencias sug = malloc(sizeof(struct _Sugerencias));

    sug->palabra = malloc(sizeof(char) * 30);
    strcpy(sug->palabra, word);
    sug->list = glist_crear();
    sug->cant_sug = cant_sug;
    sug->cache = cache;
    return sug;
}

void sugerencia_imprimir_palabra(Sugerencias sug){
  printf("%s ", sug->palabra);
  return;
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
    sug_copia->list = glist_agregar_final(sug_copia->list,node->data,(FuncionCopia)palabra_copia);
  return sug_copia;
}
