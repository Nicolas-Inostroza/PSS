#define _DEFAULT_SOURCE 1
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/resource.h>

#include "comprimir.h"

#define NRANDTESTS 100000000
#define NINTENTOS 5
#define TOLERANCIA 80

// ----------------------------------------------------
// Funcion que entrega el tiempo de CPU usado desde
// que se invoco resetTime()

static long long time0= 0;

static long long getTime0() {
  struct rusage ru;
  int rc= getrusage(RUSAGE_SELF, &ru);
  if (rc!=0) {
    perror("getTime");
    exit(1);
  }
  return (long long)ru.ru_utime.tv_sec*1000000+ru.ru_utime.tv_usec;
}

static void resetTime() {
  time0= getTime0();
}

static int getTime() {
  return (getTime0()-time0+500)/1000;
}

// ----------------------------------------------------
// Funcion que convierte un entero a notacion binaria

static char *to_bin(uint x) {
  int len= 0;
  uint z= x;
  do {
    z >>= 1;
    len++;
  } while (z!=0);
  char *s= malloc(len+1);
  char *end = s+len;
  *end-- = 0;
  z= x;
  do {
    *end-- = '0' + (z&1);
    z >>= 1;
  } while (z!=0);
  return s;
}

// ----------------------------------------------------
// Funcion que convierte un entero de nbits bits a notacion binaria

static char *to_nbits(uint x, int nbits) {
  char *s= malloc(nbits+1);
  char *end = s+nbits;
  *end-- = 0;
  for (int i= 0; i<nbits; i++) {
    *end-- = '0' + (x&1);
    x >>= 1;
  }
  return s;
}

// ----------------------------------------------------
// Funcion que escribe en out un arreglo de enteros de nbits cada uno

void printBinArray(FILE *out, uint a[], int nbits) {
  int n= (sizeof(uint)<<3) / nbits;
  for (int i= 0; i<n; i++) {
    char *elem= to_nbits(a[i], nbits);
    fprintf(out, "a[%d]=%s ", i, elem);
    free(elem);
  }
}

// ----------------------------------------------------
// Funcion que descomprime x con enteros de nbits en el arreglo a

static void descomprimir(uint a[], uint x, int nbits) {
  int n= (sizeof(uint)<<3) / nbits;
  uint mask= (1U<<(nbits-1)<<1)-1;
  int quedan= sizeof(uint)<<3;
  int i= n-1;
  while (quedan>=nbits) {
    a[i--] = x & mask;
    x >>= nbits;
    quedan -= nbits;
  }
}

// ----------------------------------------------------
// Funcion que chequea de comprimir funciona correctamente
// con un arreglo almacenado en el strings con enteros de nbits bits

int verbose= 1;

void check(char *s, int nbits) {
  int n= (sizeof(uint)<<3) / nbits;
  uint a[n];
  uint x= 0;
  for (int i= 0; i<n; i++) {
    a[i]= 0;
    for (int j=0; j<nbits; j++) {
      if (*s!='0' && *s!='1') {
        fprintf(stderr, "Error en el programa de prueba\n");
        exit(1);
      }
      a[i]= a[i]*2+(*s-'0');
      x= x*2+(*s-'0');
      s++;
    }
  }
  if (*s!=0) {
    fprintf(stderr, "Error en el programa de prueba\n");
    exit(1);
  }
  uint y= comprimir(a, nbits);
  if (verbose || x!=y) {
    char *bin_y= to_bin(y);
    printf("============= Arreglo ====================\n");
    printBinArray(stdout, a, nbits);
    printf("\nResultado es: %s\n", bin_y);
    free(bin_y);
  }
  if (x!=y) {
    char *bin_x= to_bin(x);
    fprintf(stderr, "El resultado es incorrecto. Debio ser %s\n", bin_x);
    free(bin_x);
    exit(1);
  }
}

// ----------------------------------------------------
// Funcion que chequea de comprimir funciona correctamente
// con un arreglo almacenado en x con enteros de nbits bits

void checkUInt(uint x, int nbits) {
  int n= (sizeof(uint)<<3) / nbits;
  uint mask= ( (1U<<(n*nbits-1))<<1 ) - 1;
  x &= mask;
  uint a[n];
  descomprimir(a, x, nbits);
  uint y= comprimir(a, nbits);
  if (verbose || x!=y) {
    char *bin_y= to_bin(y);
    printf("============= Arreglo ====================\n");
    printBinArray(stdout, a, nbits);
    printf("\nResultado es: %s\n", bin_y);
    free(bin_y);
  }
  if (x!=y) {
    char *bin_x= to_bin(x);
    fprintf(stderr, "El resultado es incorrecto. Debio ser %s\n", bin_x);
    free(bin_x);
    exit(1);
  }
}

int main(int argc, char **argv) {
  int tiempo_ref= 0;
  int bench= 0;            // si se hace o no el benchmark

  if (argc==2) {
    bench= 1;
    if (strcmp(argv[1], "ref")!=0)
      tiempo_ref= atoi(argv[1]);
  }

  printf("Tests unitarios\n");
  //     12345678901234567890123456789012
  check("00000000000000000000000000000000", 1);
  check("00000000000000000000000000000000", 2);
  check(  "000000000000000000000000000000", 3);
  check("00000000000000000000000000000000", 32);
  check("11111111111111111111111111111111", 1);
  check(    "1111111111111111111111111111", 7);
  check(         "11111111111111111111111", 23);
  check("11111111111111111111111111111111", 32);
  check("00000001001100100101011111001111", 4);
  check(  "111110101100011010001000111001", 3);
  check(  "000011001111000100011111111010", 5);
  check(     "101011000101101011001010000", 9);

  printf("\n\nTests de %d llamadas aleatorias a comprimir\n\n", NRANDTESTS);
  verbose= 0;
  
  uint *all_x= malloc(NRANDTESTS*sizeof(uint));
  char *all_nbits= malloc(NRANDTESTS);
  for (int i= 0; i<NRANDTESTS; i++) {
    int nbits= (random()%31) + 1;
    if (nbits>32 || nbits<1) {
      fprintf(stderr, "Error en el programa de prueba\n");
      exit(1);
    }
    all_x[i]= (random()<<1)+(random()&1);
    all_nbits[i]= nbits;
    checkUInt(all_x[i], nbits);
  }

  if (bench) {
    printf("Benchmark de %d llamadas a comprimir\n", NRANDTESTS);
    printf("Tiempo tomando por la solucion del profesor: %d milisegundos\n",
           tiempo_ref);
    int intento= 1;
    while (intento<=NINTENTOS) {
      resetTime();
      for (int i= 0; i<NRANDTESTS; i++) {
        checkUInt(all_x[i], all_nbits[i]);
      }
      int tiempo= getTime();
      printf("Tiempo de cpu usado por su version: %d milisegundos\n", tiempo);
      if (argc==2) {
        if (tiempo_ref!=0) {
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
            intento++;
          }
        }
        else {
          FILE *out= fopen("tiempo-ref.txt", "w");
          if (out==NULL) {
            perror("tiempo-ref.txt");
            exit(1);
          }
          fprintf(out, "%d\n", tiempo);
          fclose(out);
          break;
        }
      }
    }
  }


  free(all_x);
  free(all_nbits);
  printf("Bien, felicitaciones\n");
  return 0;
}
