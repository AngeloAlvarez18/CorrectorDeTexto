#include "io.h"
#include <ctype.h>
#include <string.h>


void leer_diccionario(char *path, TablaHash tabla) {
  FILE *file = fopen(path, "r");
  // Chequeamos si se pudo abrir el archivo correctamente.
  if (file == NULL) 
    quit("leer_diccionario.fopen");

  unsigned size = 30;
  char buff[size];
  int c;
  unsigned i = 0;

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

void leer_y_corregir(char *entrada, char *salida, char *cache,TablaHash tabla) {
  FILE *arch_entrada = fopen(entrada, "r");
  // Chequeamos si se pudo abrir el archivo correctamente.
  if(arch_entrada == NULL)
    quit("readfile.fopen");

  FILE *arch_salida = fopen(salida, "w");

  // flag será una bandera que nos indique si ya terminamos
  // de leer el archivo de entrada.
  int linea = 1, flag = 1;
  Palabra palabra;
  Sugerencia sugerencias;

  // Tabla en donde se va a almacenar palabras a las que ya se le
  // encontraron sugerencias o palabras que estan en el cache
  TablaHash chequeadas = tablahash_crear(10,
                                (FuncionComparadora)sugerencia_comparar,
                                (FuncionDestructora)sugerencias_destruir,
                                (FuncionHash) djb2_sug,
                                (FuncionCopiadora) sugerencia_copia);
  
  FILE* arch_cache = fopen(cache, "r");
  // Chequeamos si se pudo abrir el archivo correctamente.
  if (arch_cache == NULL)
    quit("readfile.fopen");
  
  // Leemos el archivo cache, almacenando las palabras en
  // la tabla chequeadas.
  leer_cache(arch_cache, chequeadas);
  fclose(arch_cache);

  while (flag) {
    palabra = leer_palabra(arch_entrada, &linea, &flag);

    // Nos fijamos si la palabra no esta en el diccionario
    if (!tablahash_buscar(tabla, palabra)){
      // Creamos una sugerencia, para posteriormente poder 
      // fijarnos si la palabra esta en el cache o si ya la corregimos.
      sugerencias = crear_sugerencias(palabra->str,0,0);                                                                                   
      // Nos fijamos si ya chequeamos la palabra
      if (tablahash_buscar(chequeadas,sugerencias)){
        sugerencias = buscar_en_cache(chequeadas, sugerencias);
        // La palabra esta en el cache, asi que la escribimos
        // en el archivo de salida.
        if (sugerencias->cache)
          escribir_sugerencias(arch_salida, sugerencias,linea);
      }
      // La palabra no la chequeamos, le buscamos sugerencias
      else{
      sugerencias_destruir(sugerencias);
      sugerencias = buscar_sugerencias(palabra, tabla,
                                      chequeadas, cache);
      escribir_sugerencias(arch_salida, sugerencias,linea);
      sugerencias_destruir(sugerencias);
      }
    }
    palabra_destruir(palabra);
  }

  tablahash_destruir(chequeadas);
  fclose(arch_entrada);
  fclose(arch_salida);
  return;
}

Palabra leer_palabra(FILE * archivo, int *linea, int *flag) {
  char buff[30];
  int i = 0;
  char c;
  // Bandera para que no tome los espacios como palabras.
  int bandera_espacio = 0;
  Palabra palabra;

  while ((c = getc(archivo)) != EOF) {
    c = tolower(c);

    // Sigue leyendo mientras sea un caracter.
    if (isalpha(c)){
      buff[i++] = c;
      bandera_espacio = 1;
    }

    // Llegamos al final de la palabra, terminamos de leer.
    else if (((c == ' ') || (c == '\n')) && bandera_espacio){
      buff[i] = '\0';
      palabra = palabra_crear(buff, i);
      if (c == '\n')
        (*linea)++;
      return palabra;
    }
  }

  // Indicamos a nuestra bandera que ya terminamos de
  // leer el archivo.
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
  Sugerencia sug;
  Palabra word;

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
  
      if (word->len)
        sug->list = glist_agregar_final(sug->list, word, (FuncionCopiadora) palabra_copia);
      palabra_destruir(word);
      i = 0;
    }

    // Terminar de leer todas las sugerencias, luego agrega
    // la Sugerencia a la tabla hash.
    if ((count == 2) && (c == '\n')) {
      tablahash_insertar(cache, sug);
      sugerencias_destruir(sug);
      count = 0;
    }
  }
  
  return;
}


void escribir_cache(char* arch, Sugerencia sug) {
  FILE* arch_cache = fopen(arch, "a");
  if (arch_cache == NULL)
    quit("escribir_cache.fopen");

  if (sug->cant_sug == 0) {
    fprintf(arch_cache, "%s, ", sug->palabra);
    fprintf(arch_cache, "%d, \n", sug->cant_sug);
    return;
  }

  fprintf(arch_cache, "%s, ", sug->palabra);
  fprintf(arch_cache, "%d, ", sug->cant_sug);

  for (GList node = sug->list; node != NULL; node = node->next)
    fprintf(arch_cache, "%s, ", ((Palabra) (node->data))->str);
  
  fprintf(arch_cache, "\n");
  fclose(arch_cache);
  return;
}

void escribir_sugerencias(FILE* arch, Sugerencia sug, int linea){
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