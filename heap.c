/*IACOB Andrei - 313CB*/
#include "heap.h"

THeap* AlocaHeap(int nrMax, TFCmp comp) {
    THeap* h = (THeap*)malloc(sizeof(struct Heap));
    if (!h) {
        return NULL;
    }
    h->v = (int *)malloc(nrMax * sizeof(int));
    if (!h->v) {
		free(h);
		return NULL;
	}
    h->nrMax = nrMax;
    h->nrElem = 0;
    h->comp = comp;
    return h;
}

int RelMinHeap(int a, int b) {
    extern float *scor;
	return scor[a] < scor[b];
}

void AfisareHeap(THeap* h, int pos) {
    if (pos >= h->nrElem) {      
        printf("-"); 
        return;
    }
    printf(" %d ", h->v[pos]);
	printf("(");
	
    AfisareHeap(h, 2 * pos + 1);
	printf(",");
	
    AfisareHeap(h, 2 * pos + 2);
	printf(")");
}

void DistrugeHeap(THeap **h) {
	free((*h)->v);
	free(*h);
	*h = NULL; 
}

/*
    -pornind cu valorea se inserata din pozitia de la finalalul heap-ului
    -interschimbam valoarea fiului si a tatalui si urcam in arbore pana ajungem la 
     locatia potrivita pentru a insera cu succes noua valoare in heap 
*/
void InsHeap(THeap *h, int val) {
    if (h->nrElem == h->nrMax) {
        printf("err\n");
        return;
    }
    int i = h->nrElem;
    while (i && h->comp(h->v[(i-1)/2], val) == 0) {
        h->v[i] = h->v[(i-1)/2];
        i = (i-1)/2;
    }
    h->v[i] = val;
    h->nrElem++;
}
/*
    -salvam radacina
    -aducem ultimul element in varful heap-ului
    -alegem fiul care are valoarea cea mai mare si nu respecta relatia de minheap
    -interschimbam sursa cu fiul selectat si ne mutam pe nodul fiul selectat
    -continuam pana ajungem cu elementul cu care am plecat pe pozitia potrivita
*/
int ExtHeap(THeap *h) {
    int val, s, d = 0, aux;
    val = h->v[0];
    h->v[0] = h->v[h->nrElem - 1];
    h->nrElem--;
    while(2 * d + 1 < h->nrElem) {
        s = 2 * d + 1;
        if(2 * d + 2 < h->nrElem && h->comp(h->v[s], h->v[2 * d + 2]) == 0) {
            s = 2 * d + 2;
        }
        if (h->comp(h->v[d], h->v[s])) {
            break;
        } else {
            aux = h->v[d];
            h->v[d] = h->v[s];
            h->v[s] = aux;
            d = s;
        }
    }
    return val;

}

/*
    pornim, pe rand, din fiecare nod care nu e frunza(de la ultimul catre primul),
    alegem fiul care are valoarea cea mai mare si nu respecta relatia de minheap
    interschimbam sursa cu fiul selectat si ne mutam pe nodul fiu selectat
    in final ajungem ca tot "arborele" sa respecte relatia de minheap
*/
void heapify(THeap *h) {
    int s, d, aux, i;
    for (i = h->nrElem / 2 - 1; i >= 0; i--) {
        d = i;
        while(2 * d + 1 < h->nrElem) {
            s = 2 * d + 1;
            if (2 * d + 2 < h->nrElem && h->comp(h->v[s], h->v[2 * d + 2]) == 0) {
                s = 2 * d + 2;
            }
            if (h->comp(h->v[d], h->v[s])) {
                break;
            } else {
                aux = h->v[d];
                h->v[d] = h->v[s];
                h->v[s] = aux;
                d = s;
            }
        }
    }
}