/*IACOB Andrei - 313CB*/
#include "graf-L.h"
float *scor;

TGL* AlocG(int nr) {    /* aloca spatiu pentru descriptor graf si
                        vector de (n+1) adrese liste arce */      
    TGL *g = (TGL*)malloc(sizeof(TGL));    /* spatiu descriptor */
    if (!g) {
        return NULL;
    }
    g->x = (AArc*)calloc((nr+1), sizeof(AArc)); /* spatiu vector adr. liste arce */
    if (!g->x) {
        free(g);
        return NULL;
    }
    g->n = nr;
    return g;
}

/*
    Se elibereaza intai memoria fiecarei liste(celula cu celula),
    apoi cea a vectorului de liste si dupa a grafului
*/
void DistrG(TGL **ag) { 
    int i;
    AArc p, aux;
    for (i = 1; i <= (*ag)->n; i++) { 
        p = (*ag)->x[i];                    
        while (p) {
            aux = p;
            p = p->urm;
            free(aux);
        }
    }
    free((*ag)->x);                         
    free(*ag);
    *ag = NULL;
}

/*
    Fiecare locatie care apare in descrierea grafului e introdusa
    intr-o lista de stringuri ordonata crescator lexico-grafic
*/
TLista cit_dictionar(FILE *fin) {
    int n, m, x;
    char s_str[30], d_str[30];
    fscanf(fin, "%i %i", &n, &m);
    TLista dict = NULL;
    while (fscanf(fin, "%s %s %i", s_str, d_str, &x) == 3) {
        if (InserareOrd(&dict, s_str) == -1) {
            DistrugeLista(&dict);
            return NULL;
        }
        if (InserareOrd(&dict, d_str) == -1) {
            DistrugeLista(&dict);
            return NULL;
        }
    }
    return dict;
}

/*
    
*/
TGL* CitGraf_neor(TLista dict, FILE *fin) { 
    TGL *g = NULL;
    char s_str[30], d_str[30];
    int s, d, x, n, m;
    AArc *p, aux;
    if (!fscanf (fin, "%i %i", &n, &m) || n <= 0) {
        return NULL;
    }
    g = AlocG(n);
    if (!g) {
        return NULL;
    }

    while (fscanf(fin, "%s %s %i", s_str, d_str, &x) == 3) {
        s = CautaElem(dict, s_str);     //codificarea locatiei/nodului sursa
        d = CautaElem(dict, d_str);     //codificarea locatiei/nodului destinatie
        p = g->x + s;                   //adresa primului arc din lista
        while (*p && (*p)->d < d) {
            p = &(*p)->urm;             //adresa campului urm
        }
        aux = (TCelArc *)calloc(1, sizeof(TCelArc));
        if (!aux) {
            DistrG(&g);
            return NULL;
        }                     
        aux->urm = *p;                  //urm-ul efectiv al celulei
        *p = aux;                       //actualizez campul urm     
        aux->d = d;
        aux->c = x;

        //analog pentru arcul (d, s)
        p = g->x + d;
        while (*p && (*p)->d < s) {
            p = &(*p)->urm;
        }
        aux = (TCelArc *)calloc(1, sizeof(TCelArc));
        if (!aux) {
            DistrG(&g);
            return NULL;
        }                     
        aux->urm = *p;
        *p = aux;
        aux->d = s;
        aux->c = x;
    }
    return g;
}

TGL* CitGraf_or(TLista dict, FILE *fin, int **adancimi, int *greutate) {
    TGL *g = NULL;
    char s_str[30], d_str[30], nod_str[30];
    int n, m, s, d, x, i, nod;
    AArc *p, aux;
    if (!fscanf (fin, "%i %i", &n, &m) || n <= 0) {
        return NULL;
    }
    g = AlocG(n);
    if (!g) {
        return NULL;
    }
    for (i = 1; i <= m; i++) {
        fscanf(fin, "%s %s %i", s_str, d_str, &x);
        s = CautaElem(dict, s_str);             //codificarile locatiei/nodului sursa
        d = CautaElem(dict, d_str);             //codificarile locatiei/nodului destinatie
        p = g->x + s;                           //adresa primului arc din lista
        while (*p && (*p)->d < d) {
            p = &(*p)->urm;                     //adresa campului urm
        }
        aux = (TCelArc *)calloc(1, sizeof(TCelArc));
        if (!aux) {
            DistrG(&g);
            return NULL;
        }                     
        aux->urm = *p;                          //urm-ul efectiv al celulei
        *p = aux;                               //actualizez campul urm
        aux->d = d;
        aux->c = x;
    }
    *adancimi = calloc(g->n + 1, sizeof(int));
    if (!adancimi) {
        DistrG(&g);
        return NULL; 
    }
    for (i = 1; i <= n; i++) {
        fscanf(fin, "%s %i", nod_str, &x);
        nod = CautaElem(dict, nod_str);         //codificare nod
        (*adancimi)[nod] = x;
    }
    nod = CautaElem(dict, "Corabie");
    (*adancimi)[nod] = 1;
    nod = CautaElem(dict, "Insula");
    (*adancimi)[nod] = 1;
    fscanf(fin, "%i", greutate);
    
    return g;
}


void AfiGrafL(TGL *g, TLista dict) {
    AArc l;
    int i;
    char *s, *d; 
    for(i = 1; i <= g->n; i++) {
        l = g->x[i];
        s = CautLocatie(dict, i);           //locatia aferenta codificarii din graf
        if (!l) {
            printf("%s: - \n", s);
        } else {
            printf("%s: ", s);
            for (; l != NULL; l = l->urm) {
                d = CautLocatie(dict, l->d);
                printf("%s (%d) ", d, l->c);
            }
            printf("\n");
        }
    }
}

/*
    Pana se trece prin toate nodurile din componenta comexa se procedeada astfel:
    -pt fiecare nod vizitat, se analizeaza arcele catre nodurile nevizitate
    -se retine nodul destinatie al arcului care aduce costul cel mai mic
    -se marcheaza ca vizitat nodul ales si se aduna costul gatit la costul total al parcurgerii
*/
int Prim_comp(TGL *g, int *v, int start) {
    int cost = 0, i, min, min_nod, gata = 0;
    AArc p;
    v[start] = 1;
    while (!gata) {
        min = INF;
        for(i = 1; i <= g->n; i++) {
            if (v[i] == 1) {
                for (p = g->x[i]; p; p = p->urm) {
                    if (p->c < min && v[p->d] == 0) {
                        min = p->c;
                        min_nod = p->d;
                    }
                }
            }
        }
        v[min_nod] = 1;
        if(min != INF){
            cost += min;
        } else {
            gata = 1;
        }
    }
    return cost;
}

int cmp(void const *a, void const *b) {
    return *(int *)a - *(int *)b;
}

/*
    Se aplica Prim_comp pana la vizitarea tuturor nodurilor si
    se retine intr-un vector costul fiecarei componente conexe.
    Numarul de apeluri al algoritmul ne da nr componentelor conexe.
    Se sorteaza crescator vectorul de costuri si se afiseaza.
*/
void Prim(TGL *g, FILE *fo) {
    if(g->n == 0) {
        return;
    }
    int *v = calloc(g->n + 1, sizeof(int));
    if(!v) {
        return;
    }
    int *costuri = calloc(g->n, sizeof(int));
    if(!v) {
        free(v);
        return;
    }
    int comp = 1, i;
    costuri[0] = Prim_comp(g, v, 1);
    for (i = 2; i <= g->n; i++) {
        if (v[i] == 0) {
            costuri[comp++] = Prim_comp(g, v, i);
        }
    }
    fprintf(fo, "%i\n", comp);
    qsort(costuri, comp, sizeof(int), cmp);
    for(i = 0; i < comp; i++) {
        fprintf(fo, "%i\n", costuri[i]);
    }
    free(v);
    free(costuri);
}

/*
    -se aloca si intializeaza vectorii pentru scoruri, distante, noduri precedente si noduri vizitate
    -se adauga in coada de prioritati toate nodurile in afara de nodul sursa
    -se calculeaza scorurile si distantele pentru nodurile catre care pleaca arce din sursa
    -se rearanjeaza coada de prioritati
    pe rand, se extrage cate un nod u din coada
    -pentru arcele care pleaca din nodul u si duc catre noduri nevizitate v se calculeaza ce scor am
     obtine pentru drumul de la sursa la nodul v daca am trece prin arcul (u,v)  
    -daca scorul calculat e mai mic, actualizam costul pentru nodul v si distana pana la v pentru
     ca drumul sa treaca prin arcul (u,v)
    -dupa ce am terminat de verificat arcele cere pleaca din u, marcam ca vizitat u,
     rearanjam coada si revenim la pasul in care se extrage un nod din coada
*/
void Dijkstra(TGL *g, int* adancimi, TLista dict, int s, int greutate, FILE *fo) {
    int i, u, v, temp_dist;
    float temp_scor;
    AArc p;
    THeap *h = AlocaHeap(g->n, RelMinHeap);
    if(!h) {
        return;
    }
    scor = calloc(g->n + 1, sizeof(int));
    if (!scor) {
        DistrugeHeap(&h);
        return;
    }
    int *prev = calloc(g->n + 1, sizeof(int));
    if (!prev) {
        free(scor);
        DistrugeHeap(&h);
        return;
    }
    int *dist = calloc(g->n + 1, sizeof(int));
    if (!dist) {
        free(prev);
        free(scor);
        DistrugeHeap(&h);
        return;
    }
    int *vis = calloc(g->n + 1, sizeof(int));
    if (!vis) {
        free(dist);
        free(prev);
        free(scor);
        DistrugeHeap(&h);
        return;
    }
    for (v = 1; v <= g->n; v++) {
        scor[v] = INF;
        dist[v] = INF;
        prev[v] = 0;
        if (v != s) {
            InsHeap(h,v);
        }
    }
    for (p = g->x[s]; p; p = p->urm) {
        scor[p->d] = (float)p->c / adancimi[p->d];
        dist[p->d] = p->c;
        prev[p->d] = s;
    }
    scor[s] = 0;
    vis[s] = 1;
    heapify(h);
    while (h->nrElem) {
        u = ExtHeap(h);
        for (p = g->x[u]; p; p = p->urm) {
            if (!vis[p->d]) {
                temp_scor = scor[u] + p->c / adancimi[p->d];
                temp_dist = dist[u] + p->c;
                if (temp_scor < scor[p->d] || (temp_scor == scor[p->d] && temp_dist < dist[p->d])) {
                    scor[p->d] = temp_scor;
                    dist[p->d] = temp_dist;
                    prev[p->d] = u;
                }
            }
        }
        vis[u] = 1;
        heapify(h);  
    }
    // printf("\n\n");
    // for(i = 1; i <= g->n; i++) {
    //     printf("%s: %s (%f %i)  %i\n", CautLocatie(dict, i), CautLocatie(dict, prev[i]), scor[i], dist[i], vis[i]);
    // }

    /*
        -Pornind de la nodul cu codificarea pentru corabie,
         retinem intr-o lista locatia anterioara nodului curent si dupa trecem la nodul anterior 
        -in acelasi timp retinem adancimea minima care apare in nodurile intalnite
        -in lista inseram mereu la inceput, iar prin parcuregere obtinem locatiile in ordine inversa(insula -> ... -> corabie)
        -se calculeaza numarul de drumuri si se afisam informatiile cerute
    */
    int insula_id, corabie_id, distanta_tot, adancime_min = INF, nr_drumuri;
    char *locatie;
    TLista L = NULL;
    corabie_id = CautaElem(dict, "Corabie");
    insula_id = CautaElem(dict, "Insula");
    i = corabie_id;
    distanta_tot = dist[i];
    while (i) {
        locatie = CautLocatie(dict, i);
        if(!InserareInceput(&L, locatie)) {
            DistrugeLista(&L);
            free(vis);
            free(dist);
            free(prev);
            free(scor);
            DistrugeHeap(&h);
            return;
        }
        if (i != corabie_id &&  i != insula_id  && adancimi[i] < adancime_min) {
            adancime_min = adancimi[i];
        }
        i = prev[i];
    }
    AfisareLista(L, fo);
    nr_drumuri = (greutate + 1) / adancime_min;
    fprintf(fo, "%i\n%i\n%i\n", distanta_tot, adancime_min, nr_drumuri);

    DistrugeLista(&L);
    free(vis);
    free(dist);
    free(prev);
    free(scor);
    DistrugeHeap(&h);
}

/*
    -Plecand dintr-un nod de start, se marcheaza nodul sursa ca vizitat,
    -se trece prin toate arcele acestuia si pentru cele care duc in noduri
     nevizitate se aplica acelasi predeau
    Algoritmul se incheie la parcurgerea toturor nodurilor adicente cu nodul de start
*/
void ParcDFS(TGL *g, int *vis, int start) {
    vis[start] = 1;
    AArc p;
    for (p = g->x[start]; p; p = p->urm) {
        if (vis[p->d] == 0) {
            ParcDFS(g, vis, p->d);
        }
    }
}
/*
    -se parcuge graful plecand de la corabie si daca insula ramana nevizitata, se afiseasa primul mesaj
    -se parcuge graful plecand de la insula si daca corabia ramana nevizitata, se afiseasa al doilea mesaj
    -daca nu se indeplineste niciuna din conditiile precizate, putem rezolva cerinta 2
*/
int sePoate(TGL *g, TLista dict, FILE *fo) {
    int insula_id, corabie_id, i, dus , intors;
    corabie_id = CautaElem(dict, "Corabie");
    insula_id = CautaElem(dict, "Insula");
    int *vis = calloc(g->n + 1, sizeof(int));
    ParcDFS(g, vis, corabie_id);
    dus = vis[insula_id];
    for(i = 1; i <= g->n; i++) {
        vis[i] = 0;
    }
    ParcDFS(g, vis, insula_id);
    intors = vis[corabie_id];
    if(!dus) {
        fprintf(fo, "Echipajul nu poate ajunge la insula\n");
        free(vis);
        return 0;
    }
    if(!intors) {
        fprintf(fo, "Echipajul nu poate transporta comoara inapoi la corabie\n");
        free(vis);
        return 0;
    }
    free(vis);
    return 1;
}
