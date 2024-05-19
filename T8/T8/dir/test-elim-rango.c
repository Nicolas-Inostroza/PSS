#define _DEFAULT_SOURCE 1
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/resource.h>

#include "elim-rango.h"

#define TOLERANCIA 60
#define NINTENTOS 5
#define LEN 2055
#define LENG 25000023

// ----------------------------------------------------
// Funcion que entrega el tiempo de CPU usado desde
// que se invoco resetTime()

static int time0= 0;

static int getTime0() {
    struct timeval Timeval;
    gettimeofday(&Timeval, NULL);
    return Timeval.tv_sec*1000+Timeval.tv_usec/1000;
}

void resetTime() {
  time0= getTime0();
}

int getTime() {
  return getTime0()-time0;
}

// ----------------------------------------------------
// Mide el tiempo de ejecucion de fun(ptr)

static void bench(int (*fun)(void *ptr), void *ptr, int tiempo_ref) {
  for ( int intento= 1; ; intento++) {
    int tiempo= (*fun)(ptr);
    printf("Tiempo tomado por su solucion: %d milisegundos\n", tiempo);
    printf("Tiempo tomado por la solucion de referencia: %d milisegundos\n",
           tiempo_ref);
    if (tiempo_ref>0) {
      double q= (double)tiempo/(double)tiempo_ref;
      int porciento= (q-1.)*100;
      printf("Porcentaje de sobrecosto: %d %%\n", porciento);
      if (porciento<=TOLERANCIA) {
        break;
      }
      else {
        fprintf(stderr, "*** Su solucion es demasiado lenta ***\n");
        if (intento<NINTENTOS) {
          fprintf(stderr, "Volveremos a intentar\n");
        }
        else {
          fprintf(stderr, "Lo siento: despues de %d intentos supera "
                          "el sobrecosto tolerado de %d %%\n",
                          NINTENTOS, TOLERANCIA);
          fprintf(stderr,
            "Coloque su computador en modo alto rendimiento, porque el\n"
            "economizador de bateria puede alterar los resultados.\n"
            "No ejecute este programa junto a otros programas que hagan\n"
            "un uso intensivo de la CPU.  En windows puede lanzar el\n"
            "administrador de tareas para verificar que el uso de CPU\n"
            "sea bajo.\n");
          exit(1);
        }
      }
    }
    else if (tiempo_ref==0) {
      printf("Tiempo de referencia es 0: imposible calcular sobrecosto\n");
      break;
    }
    else {
      FILE *out= fopen("tiempo-ref.txt", "a");
      if (out==NULL) {
        perror("tiempo-ref.txt");
        exit(1);
      }
      fprintf(out, "%d ", tiempo);
      fclose(out);
      break;
    }
  }
}

// ----------------------------------------------------
// Entrega una permutacion de 0, 1, 2, ..., n-1

static void permutacion(int a[], int n) {
  // printf("Permutacion\n");
  for (int i= 0; i<n; i++)
    a[i]= i;
  for (int i= 0; i<n; i++) {
    int j= i + random()%(n-i);
    int t= a[i];
    a[i]= a[j];
    a[j]= t;
    // printf("%d ", a[i]);
  }
  // printf("\n");
}

// ----------------------------------------------------
// Entrega una nueva lista en donde se agrega x al comienzo de cola

static Nodo *lista(int x, Nodo *cola) {
  Nodo *nodo= malloc(sizeof(Nodo));
  if (nodo==NULL) {
    fprintf(stderr,
            "Se acabo la memoria.  Parace que Ud. no libera los nodos\n");
    exit(1);
  }
  nodo->x= x;
  nodo->prox= cola;
  return nodo;
}

// ----------------------------------------------------
// Construye una lista enlazada con nodos de i a j

static Nodo *granLista(int i, int j) {
  Nodo *nodo= NULL;
  for (int k= j; k>=i; k--)
    nodo= lista(k, nodo);
  return nodo;
}

// ----------------------------------------------------
// Calcula el largo de una lista

static int largo(Nodo *nodo) {
  int la= 0;
  while (nodo!=NULL) {
    la++;
    nodo= nodo->prox;
  }
  return la;
}

// ----------------------------------------------------

static Nodo *listaTest() {
  return lista(2, lista(4, lista(7, lista(8, lista(9, NULL)))));
}

static int iguales(Nodo *lis1, Nodo *lis2) {
  if (lis1==NULL)
    return lis2==NULL;
  if (lis2==NULL)
    return 0;
  return lis1->x==lis2->x && iguales(lis1->prox, lis2->prox);
}

static Nodo *lista29;
static Nodo *lista789;
static Nodo *lista24;

static void test() {
  Nodo *head= listaTest();
  eliminarRango(&head,4,8); 
  if (!iguales(head, lista29)) {
    fprintf(stderr, "Falla el primer test\n");
    exit(1);
  }
  eliminarRango(&head, 2, 9);
  if (head!=NULL) {
    fprintf(stderr, "Falla el segundo test\n");
    exit(1);
  }

  head= listaTest();
  eliminarRango(&head, 0, 5); 
  if (!iguales(head, lista789)) {
    fprintf(stderr, "Falla el 3er. test\n");
    exit(1);
  }
  eliminarRango(&head, 6, 10);
  if (head!=NULL) {
    fprintf(stderr, "Falla el 4to. test\n");
    exit(1);
  }

  head= listaTest();
  eliminarRango(&head, 5, 10); 
  if (!iguales(head, lista24)) {
    fprintf(stderr, "Falla el 5to. test\n");
    exit(1);
  }
  eliminarRango(&head, 2, 10);
  if (head!=NULL) {
    fprintf(stderr, "Falla el 6to. test\n");
    exit(1);
  }
}

typedef struct {
  int len, check;
} Param;

static int test_robustez(void *ptr) {
  Param *pparm= ptr;
  int len= pparm->len;
  int check= pparm->check;

  Nodo *nodo= granLista(0, len-1);

#define N 100

  int perm[N];
  permutacion(perm, N);
  int delta= len/N;
  int tam= largo(nodo);
  resetTime();
  for (int k= 0; k<N; k++) {
    int ini= perm[k]*delta;
    int fin= ini+delta-1;
    eliminarRango(&nodo, ini, fin);
    tam -= delta;
    if (check && largo(nodo)!=tam) {
      fprintf(stderr, "Largo de la lista debio ser %d, no %d\n",
              tam, largo(nodo));
      exit(1);
    }
  }
  eliminarRango(&nodo, 0, len);
  int tiempo= getTime();
  if (nodo!=NULL) {
    fprintf(stderr, "La lista debio estar vacia\n");
    exit(1);
  }
  return tiempo;
}

int main(int argc, char *argv[]) {
  lista29=  lista(2, lista(9, NULL));
  lista789= lista(7, lista(8, lista(9, NULL)));
  lista24= lista(2, lista(4, NULL));

  printf("Tests de unidad\n");
  test();

  printf("Test de robustez con lista de %d nodos\n", LEN);

  {
    Param parm= { LEN, 1 };
    test_robustez(&parm);
  }

  if (argc>1) {
    printf("Test de eficiencia con lista de %d nodos\n", LENG);
    int tiempo_ref= atoi(argv[1]);
    Param parm= { LENG, 0 };
    bench(test_robustez, &parm, tiempo_ref);
  }

  printf("\nFelicitaciones!  Paso todos los tests\n");

  return 0;
}
