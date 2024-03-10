/*IACOB Andrei - 313CB*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef LISTA_H
#define LISTA_H

typedef struct Celula {
    char *info;                 //locatie
    struct Celula *urm;
} TCelula, *TLista;

TLista AlocCelula (char *info);
void AfisareLista (TLista L, FILE *fo);
void DistrugeLista (TLista *aL);
int InserareOrd(TLista *aL, char *x);
int CautaElem(TLista L, char *x);
char* CautLocatie(TLista L, int x);
int InserareInceput(TLista *aL, char *x);

#endif