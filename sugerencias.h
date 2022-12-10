#ifndef __SUGERENCIAS_H__
#define __SUGERENCIAS_H__
#include "glist.h"

typedef struct _Sugerencias *Sugerencias;

struct _Sugerencias{
    char* palabra;
    GList list;
    unsigned cant_sug;
    int cache;
};

Sugerencias crear_sugerencias(char* word, int cant_sug, int cache);

void sugerencias_destruir(Sugerencias sug);

Sugerencias sugerencia_copia(Sugerencias sug);

int sugerencia_comparar(Sugerencias sug1, Sugerencias sug2);

#endif /* __SUGERENCIAS_H__ */