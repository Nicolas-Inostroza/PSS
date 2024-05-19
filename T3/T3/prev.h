typedef struct nodo {
  int x;
  struct nodo *izq, *der, *prev, *prox;
} Nodo;

void asignarPrev(Nodo *t, Nodo **pprev);
