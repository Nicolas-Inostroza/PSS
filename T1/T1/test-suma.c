#define _DEFAULT_SOURCE 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/resource.h>

#include "suma.h"

#define NRANDTESTS 1024
#define REP 10000
#define NINTENTOS 5
#define TOLERANCIA 80

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

static void check(Bcd x, Bcd y, Bcd ref) {
  Bcd res= sumaBcd(x, y);
  if (sumaBcd(x, y) != ref) {
    fprintf(stderr, "Error: el resultado de 0x%llx + 0x%llx debio ser 0x%llx, "
                    "no 0x%llx\n", x, y, ref, res);
    exit(1);
  }
}

static Bcd ull2Bcd(unsigned long long xp) {
  Bcd res= 0;
  unsigned long long pos= 1;
  unsigned long long x= xp;
  for (int i= 0; i<16; i++) {
    unsigned long long dig= x%10;
    res += dig*pos;
    pos *= 16;
    x /= 10;
  }

  return res;
}

static unsigned long long randNum(void) {
  unsigned long long xl= random();
  unsigned long long xh= random();
  unsigned long long res= (((xl & 0x7fffffff)<<31) | (xh & 0x7fffffff));
  return res%9999999999999999LL;
}

int main(int argc, char **argv) {
  int tiempo_ref= 0;
  int bench= 0;            // si se hace o no el benchmark

  if (argc==2) {
    bench= 1;
    if (strcmp(argv[1], "ref")!=0)
      tiempo_ref= atoi(argv[1]);
  }

  printf("Tests de unidad \n");
  check(0x0, 0x0, 0x0);
  check(0x0, 0x1, 0x1);
  check(0x1, 0x1, 0x2);
  for (int i= 0; i<=9; i++) {
    for (int j= 0; j<=9; j++) {
      if (i+j<10)
        check(i, j, i+j);
    }
  }
  for (int i= 0; i<=9; i++) {
    for (int j= 0; j<=9; j++) {
      if (i+j>=10)
        check(i, j, 0x10 + (i+j)%10);
    }
  }

  check(0x0123456789, 0x9876543210, 0x9999999999);
  check(0x012345678, 0x87654321, 0x99999999);
  check(0x390160, 0x6176012, 0x6566172);
  check(0x8999999999999999LL, 0x1, 0x9000000000000000);
  check(0x9999999999999999LL, 0x1, (Bcd)-1);
  check(0x8000000000000000LL, 0x8000000000000000LL, (Bcd)-1 );

  printf("Tests aleatorios\n");
  Bcd ax[NRANDTESTS], ay[NRANDTESTS], az[NRANDTESTS];
  
  for (int i= 0; i<NRANDTESTS; i++) {
    unsigned long long x, y, z;
    do {
      x= randNum();
      y= randNum();
      z= x + y;
    } while (z>9999999999999999LL);
    ax[i]= ull2Bcd(x);
    ay[i]= ull2Bcd(y);
    az[i]= ull2Bcd(z); 
    check(ax[i], ay[i], az[i]);
  }

  if (bench) {
    printf("Benchmark de %d llamadas a sumaBcd\n", NRANDTESTS*REP);
    printf("Tiempo tomado por la solucion del profesor: %d milisegundos\n",
           tiempo_ref);
    int intento= 1;
    while (intento<=NINTENTOS) {
      resetTime();
      for (int k= 0; k<REP; k++) {
        for (int i= 0; i<NRANDTESTS; i++) {
          check(ax[i], ay[i], az[i]);
        }
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

  printf("Bien, felicitaciones\n");
  return 0;
}
