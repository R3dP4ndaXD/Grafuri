/*IACOB Andrei - 313CB*/
/*--- graf-L.h --- Graf reprezentat cu liste --*/

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include "lista.h"
#include "heap.h"
#define INF 100000

#ifndef GRAF_L_H
#define GRAF_L_H

typedef struct celarc 
{ 
    int d;              /* destinatie arc */
    int c;              /* cost arc */
    struct celarc *urm; /* adresa urmatorul adiacent */
} TCelArc, *AArc;

typedef struct 
{ 
    int n;              /* numar noduri */
    AArc* x;            /* vector adrese liste arce */
} TGL;

TLista cit_dictionar(FILE *fin);
TGL* CitGraf_neor(TLista dict, FILE *fin);
TGL* CitGraf_or(TLista dict, FILE *fin, int **adancimi, int *greutate);
void AfiGrafL(TGL *, TLista dict); /* afisare liste de adiacente */
void DistrG(TGL** ag);

int Prim_comp(TGL *g, int *v, int start) ;
void Prim(TGL *g, FILE *fo);
void Dijkstra(TGL *g, int* adancimi, TLista dict, int s, int greutate, FILE *fo);
void ParcDFS(TGL *g, int *vis, int start);
int sePoate(TGL *g, TLista dict, FILE *fo);

#endif
