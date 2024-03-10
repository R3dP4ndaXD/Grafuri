/*IACOB Andrei - 313CB*/
#include"lista.h"

TLista AlocCelula (char *info) {
    TLista aux = malloc(sizeof(TCelula));
    if (!aux)
        return NULL;
    aux->info = calloc(strlen(info) + 1, sizeof(char));
    if(!aux->info) {
        free(aux);
        return NULL;
    }
    strcpy(aux->info, info);
    aux->urm = NULL;
    return aux;
}

void AfisareLista (TLista L, FILE *fo) {
    while(L) {
        fprintf(fo, "%s ", L->info);
        L = L->urm;
    }
    fprintf(fo,"\n");
}

void DistrugeLista (TLista *aL) {
    TLista p;
    while(*aL) {
        p = *aL;
        *aL=(*aL)->urm;
        free(p->info);
        free(p);
    }
    *aL = NULL;
}

int InserareOrd(TLista *aL, char *x) {
    TLista p, ant = NULL, aux;
    for (p = *aL; p && strcmp(p->info, x) < 0 ; ant = p, p = p->urm);
    if (p && strcmp(p->info, x) == 0) {
        return 0;
    }
    aux = AlocCelula(x);
    if (!aux) {
        return -1;
    }
    aux->urm = p;
    if (!ant) {
        *aL = aux;
    }
    else{
        ant->urm= aux;
    }
    return 1;
}

int InserareInceput(TLista *aL, char *x) {
    TLista aux = AlocCelula(x);
    if (!aux) {
        return 0;
    }
    aux->urm = *aL;
    *aL = aux;
    return 1;
}

/*
    Parcurg lista pana la gasirea locatiei, si returnez numarul celulei cautate
*/
int CautaElem(TLista L, char *x) {
    if(!L) {
        return 0;
    }
    TLista p;
    int i = 1;
    for (p = L; p; p = p->urm, i++) {
        if (strcmp(p->info, x) == 0) {
            return i;
        }
    }
    return 0;
}

/*
    Parcurg lista pana a x-a celula si returnez informatia
*/
char* CautLocatie(TLista L, int x) {
    if(!L) {
        return NULL;
    }
    TLista p;
    int i;
    for (p = L, i = 1; p && i < x; p = p->urm, i++);
    if (p && x) {
        return p->info;
    }
    return NULL;   
}