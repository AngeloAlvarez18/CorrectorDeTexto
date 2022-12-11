#include <stdio.h>
#include "../estructuras/tablahash.h"
#include "utils.h"
#include "../estructuras/palabra.h"
#include "io.h"

/** Capacidad inicial para la tabla hash */
#define CAPACIDAD_INICIAL 20

int main(int argc, char** argv) {
  
  // Chequeamos que se hayan pasado la cantidad 
  // de argumentos correctos.
  if (argc != 4)
    error();
  
  // Diccionario que usaremos.
  char dicc[30];
  printf("Ingrese el diccionario: ");
  scanf("%s", dicc);

  //Iniciar tabla hash.
  TablaHash tabla = tablahash_crear(CAPACIDAD_INICIAL,
                  (FuncionComparadora) palabra_comparar, 
                  (FuncionDestructora) palabra_destruir,
                  (FuncionHash) djb2, (FuncionCopiadora) palabra_copia);


  // Leemos el diccionario y lo guardamos en la tabla creada.
  leer_diccionario(dicc, tabla);

  // Leemos y corregimos las palabras que no estan el diccionario
  // del archivo de entrada.
  leer_y_corregir(argv[1], argv[2], argv[3], tabla);

  //Liberar memoria
  tablahash_destruir(tabla);
  
  return 0;
}