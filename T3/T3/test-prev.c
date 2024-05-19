#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/time.h>
#include <sys/resource.h>

#include "prev.h"

#pragma GCC diagnostic ignored "-Wunused-function"

#ifdef OPT
#define BENCH 1
#endif

// ----------------------------------------------------
// Funcion que entrega el tiempo transcurrido desde el lanzamiento del
// programa en milisegundos

static long long time0= 0;

static long long getTime0() {
#ifdef RISCV
    return 0;
#else
    struct rusage ru;
    int rc= getrusage(RUSAGE_SELF, &ru);
    if (rc!=0) {
      perror("getTime");
      exit(1);
    }
    return (long long)ru.ru_utime.tv_sec*1000000+ru.ru_utime.tv_usec;
#endif
}

static void resetTime() {
  time0= getTime0();
}

static int getTime() {
  return (getTime0()-time0+500)/1000;
}

// ----------------------------------------------------
// revisar_int: verifica que un entero es un valor especifico

void revisar_int(int n, int nref) {
  if (n!=nref) {
    fprintf(stderr, "El valor retornado debio ser %d, no %d\n", nref, n);
    exit(1);
  }
}

// ----------------------------------------------------
// Funcion que construye un arbol

static Nodo *abb(int x, Nodo *izq, Nodo *der) {
  Nodo *a= malloc(sizeof(*a));
  a->x= x;
  a->izq= izq;
  a->der= der;
  return a;
}

// ----------------------------------------------------
// Funcion que verifica que 2 arboles binarios son iguales

static void revisar_abb(Nodo *a, Nodo *b) {
  if (a==NULL) {
    if (b==NULL)
      return;
    fprintf(stderr, "Arboles distintos: a es NULL, b tiene etiqueta %d\n",
                    b->x);
    exit(1);
  }
  else if (b==NULL) {
    fprintf(stderr, "Arboles distintos: b es NULL, a tiene etiqueta %d\n",
                    a->x);
    exit(1);
  }
  else {
    if (a->x!=b->x) {
      fprintf(stderr, "Etiquetas de arboles son distintas: %d != %d\n",
                      a->x, b->x);
      exit(1);
    }
    revisar_abb(a->izq, b->izq);
    revisar_abb(b->der, b->der);
  }
}

// ----------------------------------------------------
// Funcion que verifica que 2 arboles binarios sean el mismo

static void revisar_mismo_nodo(Nodo *a, Nodo *b) {
  if (a!=b) {
    if (a==NULL || b==NULL)
      fprintf(stderr, "No son el mismo arbol.  Uno es NULL, el otro no\n");
    else
      fprintf(stderr, "No son el mismo arbol.  Etiquetas son %d %d\n",
                      a->x, b->x);
    exit(1);
  }
}

// ----------------------------------------------------
// Funcion que verifica que el campo prev este bien asignado

static void revisar_prev_prox(Nodo *a) {
  if (a==NULL)
    return;
  else {
    revisar_mismo_nodo(a->prev->prox, a);
    revisar_prev_prox(a->izq);
    revisar_prev_prox(a->der);
  }
}

// ----------------------------------------------------
// Libera un arbol binario

static void liberar(Nodo *a) {
  if (a!=NULL) {
    liberar(a->izq);
    liberar(a->der);
    free(a);
  }
}

// ----------------------------------------------------
// Crea una copia de un arbol binario

static Nodo *copia(Nodo *a) {
  if (a==NULL)
    return NULL;
 
  Nodo *an= malloc(sizeof(Nodo));
  an->x= a->x;
  an->izq= copia(a->izq);
  an->der= copia(a->der);
  return an;
}

// ----------------------------------------------------
// Crea un arbol binario de busqueda bien equilibrado en donde 
// los x van de i a j

static Nodo *equilibrado(int i, int j) {
  if (i>j)
    return NULL;
  int k= (i+j+1)/2;
  Nodo *a= malloc(sizeof(Nodo));
  a->x= k;
  a->izq= equilibrado(i, k-1);
  a->der= equilibrado(k+1, j);
  return a;
}

// ----------------------------------------------------
// Crea un arbol binario desequilibrado por la izquierda en donde 
// los x van de i a j

static Nodo *desequilibrado_izq(int i, int j) {
  if (i>j) {
    return NULL;
  }
  Nodo *a= malloc(sizeof(Nodo));
  a->x= j;
  a->izq= desequilibrado_izq(i, j-1);
  a->der= NULL;
  return a;
}

// ----------------------------------------------------
// Crea un arbol binario desequilibrado por la derecha en donde 
// los x van de i a j

static Nodo *desequilibrado_der(int i, int j) {
  if (i>j) {
    return NULL;
  }
  Nodo *a= malloc(sizeof(Nodo));
  a->x= i;
  a->izq= NULL;
  a->der= desequilibrado_der(i+1, j);
  return a;
}

int main(int argc, char *argv[]) {
  printf("===============================\n");
  printf("Tests de la funcion asignarPrev\n");
  printf("===============================\n\n");
  {
    printf("Caso arbol nulo: debe continuar vacio\n");
    Nodo *a= NULL;
    Nodo *prev= NULL;
    asignarPrev(a, &prev);
    revisar_abb(a, NULL);
    revisar_abb(prev, NULL);
    printf("Caso arbol de un nodo con x=5\n");
    a= abb(5, NULL, NULL);
    Nodo *ref= copia(a);
    asignarPrev(a, &prev);
    revisar_abb(a, ref);
    revisar_mismo_nodo(prev, a);
    revisar_mismo_nodo(a->prev, NULL);
    revisar_mismo_nodo(a->prox, NULL);
    liberar(a);
    liberar(ref);
  }

  {
    printf("Caso abb de un nodo x=1 con subarbol derecho x=2\n");
    Nodo *n2= abb(2, NULL, NULL);
    Nodo *n1= abb(1, NULL, n2);
    Nodo *ref= copia(n1);
    Nodo *prev= NULL;
    asignarPrev(n1, &prev);
    revisar_abb(n1, ref);
    revisar_mismo_nodo(prev, n2);
    revisar_mismo_nodo(n2->prev, n1);
    revisar_mismo_nodo(n1->prev, NULL);
    revisar_mismo_nodo(n1->prox, n2);
    revisar_mismo_nodo(n2->prox, NULL);
    liberar(n1);
    liberar(ref);
  }

  {
    printf("Caso abb de un nodo x=2 con subarbol izquierdo x=1\n");
    Nodo *n1= abb(1, NULL, NULL);
    Nodo *n2= abb(2, n1, NULL);
    Nodo *ref= copia(n2);
    Nodo *prev= NULL;
    asignarPrev(n2, &prev);
    revisar_abb(n2, ref);
    revisar_mismo_nodo(prev, n2);
    revisar_mismo_nodo(n2->prev, n1);
    revisar_mismo_nodo(n1->prev, NULL);
    revisar_mismo_nodo(n1->prox, n2);
    revisar_mismo_nodo(n2->prox, NULL);
    liberar(n2);
    liberar(ref);
  }

  {
    printf("Caso abb de un nodo x=2 con subarboles izquierdo y derecho\n");
    Nodo *n1= abb(1, NULL, NULL);
    Nodo *n3= abb(1, NULL, NULL);
    Nodo *n2= abb(2, n1, n3);
    Nodo *ref= copia(n2);
    Nodo *prev= NULL;
    asignarPrev(n2, &prev);
    revisar_abb(n2, ref);
    revisar_mismo_nodo(prev, n3);
    revisar_mismo_nodo(n3->prev, n2);
    revisar_mismo_nodo(n2->prev, n1);
    revisar_mismo_nodo(n1->prev, NULL);
    revisar_mismo_nodo(n1->prox, n2);
    revisar_mismo_nodo(n2->prox, n3);
    revisar_mismo_nodo(n3->prox, NULL);
    liberar(n2);
    liberar(ref);
  }

  {
    printf("Caso abb del enunciado\n");
    Nodo *n1= abb(1, NULL, NULL);
    Nodo *n3= abb(3, NULL, NULL);
    Nodo *n34= abb(4, n3, NULL);
    Nodo *n1234= abb(2, n1, n34);
    Nodo *n6= abb(6, NULL, NULL);
    Nodo *n67= abb(7, n6, NULL);
    Nodo *n1234567= abb(5, n1234, n67);
    Nodo *ref= copia(n1234567);
    Nodo *prev= NULL;
    asignarPrev(n1234567, &prev);
    revisar_abb(n1234567, ref);
    revisar_mismo_nodo(n1->prev, NULL);
    revisar_mismo_nodo(n1234->prev, n1);
    revisar_mismo_nodo(n3->prev, n1234);
    revisar_mismo_nodo(n34->prev, n3);
    revisar_mismo_nodo(n1234567->prev, n34);
    revisar_mismo_nodo(n6->prev, n1234567);
    revisar_mismo_nodo(n67->prev, n6);
    revisar_mismo_nodo(n1->prox, n1234);
    revisar_mismo_nodo(n1234->prox, n3);
    revisar_mismo_nodo(n3->prox, n34);
    revisar_mismo_nodo(n34->prox, n1234567);
    revisar_mismo_nodo(n1234567->prox, n6);
    revisar_mismo_nodo(n6->prox, n67);
    revisar_mismo_nodo(n67->prox, NULL);
    liberar(ref);
    liberar(n1234567);
  }

  {
    int n= 100;
    printf("Caso abbs de 1 a %d nodos, equilibrado, desequilibrado por "
           "izquierda y derecha\n", n);
    for (int i=1; i<=n; i++) {

      {
        Nodo *equilib= equilibrado(1, i);
        Nodo dummy= {0, NULL, NULL, NULL, NULL};
        Nodo *prev= &dummy;
        asignarPrev(equilib, &prev);
        revisar_prev_prox(equilib);
        revisar_int(prev->x, i);
        revisar_mismo_nodo(prev->prox, NULL);
        revisar_int(dummy.prox->x, 1);
        revisar_mismo_nodo(dummy.prox->prev, &dummy);
        liberar(equilib);
      }

      {
        Nodo *deseq_izq= desequilibrado_izq(1, i);
        Nodo dummy= {0, NULL, NULL, NULL, NULL};
        Nodo *prev= &dummy;
        asignarPrev(deseq_izq, &prev);
        revisar_prev_prox(deseq_izq);
        revisar_int(prev->x, i);
        revisar_mismo_nodo(prev->prox, NULL);
        revisar_int(dummy.prox->x, 1);
        revisar_mismo_nodo(dummy.prox->prev, &dummy);
        liberar(deseq_izq);
      }

      {
        Nodo *deseq_der= desequilibrado_der(1, i);
        Nodo dummy= {0, NULL, NULL, NULL, NULL};
        Nodo *prev= &dummy;
        asignarPrev(deseq_der, &prev);
        revisar_prev_prox(deseq_der);
        revisar_int(prev->x, i);
        revisar_mismo_nodo(prev->prox, NULL);
        revisar_int(dummy.prox->x, 1);
        revisar_mismo_nodo(dummy.prox->prev, &dummy);
        liberar(deseq_der);
      }
    }
  }

#ifdef BENCH
#define N1 10000

  printf("\n==================================\n");
  printf(  "Benchmark de asignarPrev\n");
  printf(  "==================================\n\n");

#else

#define N1 1000

#endif

  int tiempo;

  {
    int n= N1;
    resetTime();
    for (int i=1; i<=n; i++) {

      {
        Nodo *equilib= equilibrado(1, i);
        Nodo *prev= NULL;
        asignarPrev(equilib, &prev);
        liberar(equilib);
      }

      {
        Nodo *deseq_izq= desequilibrado_izq(1, i);
        Nodo *prev= NULL;
        asignarPrev(deseq_izq, &prev);
        liberar(deseq_izq);
      }

      {
        Nodo *deseq_der= desequilibrado_der(1, i);
        Nodo *prev= NULL;
        asignarPrev(deseq_der, &prev);
        liberar(deseq_der);
      }
    }
    tiempo= getTime();
    if (tiempo!=0) {
      printf("Tiempo = %d milisegundos\n", tiempo);
    }
  }

  if (argc>=2) {
    if (strcmp(argv[1], "ref")==0) {
      FILE *out= fopen("tiempo-ref.txt", "w");
      if (out==NULL) {
        perror("tiempo-ref.txt");
        exit(1);
      }
      fprintf(out, "%d\n", tiempo);
      fclose(out);
    }
    else {
      int tiempo_ref= atoi(argv[1]);
      double q= (double)tiempo/(double)tiempo_ref;
      int porciento= (q-1.)*100+0.5;
      printf("Tiempo de referencia = %d milisegundos\n",
             tiempo_ref);
      printf("Porcentaje de sobrecosto de asignarPrev: %d %%\n", porciento);
      if (porciento>80) {
        fflush(stdout);
        fprintf(stderr, "Lo siento: su solucion es demasiado lenta\n");
        exit(1);
      }
    }
  }

#ifdef BENCH
  printf("Felicitaciones: su solucion es correcta y eficiente\n");
#else
  printf("Felicitaciones: su solucion es correcta\n");
#endif

  return 0;
}
