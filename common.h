#ifndef __COMMON_H__
#define __COMMON_H__

/**
 * Funcion que libera la funcion del dato pasado.
*/
typedef void (*FuncionDestructora)(void *dato);

/**
 * Funcion que retorna una copia fisica del dato pasado.
*/
typedef void *(*FuncionCopiadora)(void *dato);

/** Retorna un entero negativo si dato1 < dato2, 0 si son iguales
 * y un entero positivo si dato1 > dato2 
*/
typedef int (*FuncionComparadora)(void *dato1, void *dato2);

/** 
 * Retorna un entero sin signo para el dato
*/
typedef unsigned (*FuncionHash)(void *dato);


typedef void (*FuncionVisitante)(void *dato);

#endif /* __COMMON_H__ */