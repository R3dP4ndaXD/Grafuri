/*IACOB Andrei - 313CB*/
#include "graf-L.h"

int main(int argc, char *argv[])
{
    FILE *fin = fopen("tema3.in", "r");
    FILE *fo = fopen("tema3.out", "w");
    if(!fin || !fo) {
        return -1;
    }
    TLista dict = cit_dictionar(fin);
    if (!dict) {
        fclose(fin);
        fclose(fo);
        return -1;
    }
    fseek(fin, 0, SEEK_SET);
    //AfisareLista(dict);
    TGL* g;
    if (!strcmp(argv[1], "1")) {
        g = CitGraf_neor(dict, fin);
        if(!g) {
            DistrugeLista(&dict);
            fclose(fin);
            fclose(fo);
            return -1;
        }
        //AfiGrafL(g, dict);
        Prim(g, fo);
        DistrG(&g);
        
    } else if(!strcmp(argv[1], "2")) {
        int *adancimi = NULL;
        int greutate = 0;
        g = CitGraf_or(dict, fin, &adancimi, &greutate);
        if(!g) {
            DistrugeLista(&dict);
            fclose(fin);
            fclose(fo);
            return -1;
        }
        //AfiGrafL(g, dict);
        if(sePoate(g, dict, fo)) {
            int insula_id = CautaElem(dict, "Insula");
            Dijkstra(g, adancimi, dict, insula_id, greutate, fo);
        }
        DistrG(&g);
        free(adancimi);
    }
    DistrugeLista(&dict);
    fclose(fin);
    fclose(fo);
    return 0;
}