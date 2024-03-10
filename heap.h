/*IACOB Andrei - 313CB*/
#include <stdio.h>
#include <stdlib.h>

#ifndef HEAP_H
#define HEAP_H

typedef int (*TFCmp)(int, int);

typedef struct Heap {
    int nrMax, nrElem;
    int *v;
    TFCmp comp;
} THeap;

THeap* AlocaHeap(int nrMax, TFCmp comp);
int RelMinHeap(int a, int b);
void AfisareHeap(THeap *h, int pos);
void DistrugeHeap(THeap **h);
void InsHeap(THeap *h, int val);
int ExtHeap(THeap *h);
void heapify(THeap *h);

#endif