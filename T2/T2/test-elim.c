#define _XOPEN_SOURCE 500
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#include "elim.h"

#ifdef OPT
#define SIZE (300000000)
#define DUP 30
#else
#define SIZE (5000000)
#define DUP 10
#endif

#define TOLERANCIA 80
#define PAT "abcdefghijklm"

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
// Verifica que el resultado de eliminados coincida con el valor de referencia
//

static void check(char *arg, char *pat, char *ref) {
  char *copia= strdup(arg);
  char *copiaPat= strdup(pat);
  char *s= eliminados(copia, copiaPat);
  if (strcmp(s, ref)!=0) {
    fprintf(stderr, "Valor incorrecto para eliminados(\"%s\",\"%s\")\n"
                    "el resultado fue \"%s\"\n"
                    "el resultado debio ser \"%s\"\n", arg, pat, s, ref);
    exit(1);
  }
  if (strcmp(copia, arg)!=0) {
    fprintf(stderr, "El argumento cambio: \"%s\"\n"
                    "El original era: \"%s\"\n",
                    copia, arg);
    exit(1);
  }
  if (strcmp(copiaPat, pat)!=0) {
    fprintf(stderr, "El patron cambio: \"%s\"\n"
                    "El original era: \"%s\"\n",
                    copiaPat, pat);
    exit(1);
  }
  free(copia);
  free(copiaPat);
  free(s);
}

// ----------------------------------------------------
// Verifica que el resultado de eliminar coincida con el valor de referencia
//

static void checkImp(char *arg, char *pat, char *ref) {
  char *s= strdup(arg);
  char *copiaPat= strdup(pat);
  eliminar(s, copiaPat);
  if (strcmp(s, ref)!=0) {
    fprintf(stderr, "Valor incorrecto para eliminar(\"%s\",\"%s\")\n"
                    "el resultado fue \"%s\"\n"
                    "el resultado debio ser \"%s\"\n", arg, pat, s, ref);
    exit(1);
  }
  if (strcmp(copiaPat, pat)!=0) {
    fprintf(stderr, "El patron cambio: \"%s\"\n"
                    "El original era: \"%s\"\n",
                    copiaPat, pat);
    exit(1);
  }
  free(copiaPat);
  free(s);
}

// ----------------------------------------------------
// Construye un string enorme
//

char *enorme(int len, char *pat, char **pref) {
  char lastChar= pat[strlen(pat)-1];
  char *res= malloc(len+1);
  int refLen= 0;
  {
    char *s= res;
    char *p= s, *top= p+len;
    *top= 0;
    
    for (;;) {
      for (char d= 'a'; d<='z'; d++) {
        for (char c= '0'; c<='9'; c++) {
          for (char e= '0'; e<=c; e++) {
            *p++ = e;
            refLen++;
            if (p>=top)
              goto buildRef;
          }
          for (char e= 'a'; e<=d; e++) {
            *p++ = e;
            if (d<lastChar || e>lastChar)
              refLen++;
            if (p>=top)
              goto buildRef;
          }
        }
      }
    }
  }

  buildRef:

  {
    char *s= malloc(refLen+1);
    *pref= s;
    char *p= s, *top= p+refLen;
    *top= 0;
    
    for (;;) {
      for (char d= 'a'; d<='z'; d++) {
        for (char c= '0'; c<='9'; c++) {
          for (char e= '0'; e<=c; e++) {
            *p++ = e;
            if (p>=top)
              return res;
          }
          for (char e= d>=lastChar ? lastChar+1 : 'a'; e<=d; e++) {
            *p++ = e;
            if (p>=top)
              return res;
          }
        }
      }
    }
  }
}

int equals(char *s, char *r) {
  char *p= s;
  char *q= r;
  while (*p == *q && *p!=0) {
    p++; q++;
  }
  if (*p == 0 && *q==0)
    return 1;
  else
    return 0;
}
 
int main(int argc, char *argv[]) {
  int tiempo_eliminar_prof= 0;
  int tiempo_eliminados_prof= 0;
  if (argc==3) {
    tiempo_eliminar_prof= atoi(argv[1]);
    tiempo_eliminados_prof= atoi(argv[2]);
  }

  {
    printf("Tests de eliminar\n");
    checkImp("", "", "");
    // checkImp("abc", "", "abc");
    checkImp("", "abcd", "");
    checkImp("a", "b", "a");
    checkImp("a", "a", "");
    checkImp("aaabcddd", "bc", "aaaddd");
    checkImp("aaabcdeebcdffff", "bcd", "aaaeeffff");
    checkImp("abccbdddbeeeeb", "b", "accdddeeee");
    checkImp("bccbdddbeeeeb", "b", "ccdddeeee");
    checkImp("bccbdddbeeee", "b", "ccdddeeee");
  }

  {
    printf("Tests de eliminados\n");
    check("", "", "");
    // check("abc", "", "abc");
    check("", "abcd", "");
    check("a", "b", "a");
    check("a", "a", "");
    check("aaabcddd", "bc", "aaaddd");
    check("aaabcdeebcdffff", "bcd", "aaaeeffff");
    check("abccbdddbeeeeb", "b", "accdddeeee");
    check("bccbdddbeeeeb", "b", "ccdddeeee");
    check("bccbdddbeeee", "b", "ccdddeeee");
  }

  if (argc!=2 || strcmp(argv[1], "ref")!=0) {
    printf("Test de uso de memoria de eliminados\n");
    char *str= malloc(SIZE+1);
    char *top= str+SIZE;
    char *s= str;
    while (s<top) {
      if (s-str==SIZE/2)
        *s= '1';
      else
        *s= 'a';
      s++;
    }
    char **array= malloc(DUP*sizeof(char*));
    for (int i= 0; i<DUP; i++)
      array[i]= eliminados(str, "a");
    for (int i= 0; i<DUP; i++)
      free(array[i]);
    free(array);
    free(str);
  }

  int tiempo_eliminar= 0;
  int tiempo_eliminados= 0;

  {
    printf("---------------------\n");
    printf("Benchmark de eliminar\n");
    printf("---------------------\n");
    if (tiempo_eliminar_prof!=0)
      printf("Tiempo para la version del profesor= %d milisegundos\n",
             tiempo_eliminar_prof);
    int intento= 1;
    while (intento<=5) {
      printf("Intento= %d\n", intento);
      char *ref;
      char *s= enorme(SIZE, PAT, &ref);
      int start= getTime();
      eliminar(s, PAT);
      int tiempo= getTime()-start;
      tiempo_eliminar= tiempo;
      if (!equals(s, ref)) {
        fprintf(stderr, "Resultado incorrecto en benchmark de eliminar\n");
        exit(1);
      }
      free(s);
      free(ref);
      printf("Tiempo para su version= %d milisegundos\n", tiempo);
      if (tiempo_eliminar_prof==0)
        break;
      double q= (double)tiempo/(double)tiempo_eliminar_prof;
      int porciento= (q-1.)*100;
      printf("Porcentaje de sobrecosto: %d %%\n", porciento);
      if (porciento<=TOLERANCIA) {
        break;
      }
      printf("Excede en mas del %d %% la version del profesor\n", TOLERANCIA);
      if (intento<5)
        printf("Se hara un nuevo intento\n");
      intento++;
    }
    if (intento>5) {
      fprintf(stderr, "Lo siento: Despues de 5 intentos no satisface "
                      "la eficiencia requerida\n");
      fprintf(stderr,
              "Coloque su computador en modo alto rendimiento, porque el\n"
              "economizador de bateria puede alterar los resultados.\n"
              "No ejecute este programa junto a otros programas que hagan\n"
              "un uso intensivo de la CPU.  En windows puede lanzar el\n"
              "administrador de tareas para verificar que el uso de CPU\n"
              "sea bajo.\n");

      exit(1);
    }
    printf("Aprobado\n");
  }

  {
    printf("-----------------------\n");
    printf("Benchmark de eliminados\n");
    printf("-----------------------\n");
    if (tiempo_eliminados_prof!=0)
      printf("Tiempo para la version del profesor= %d milisegundos\n",
             tiempo_eliminados_prof);
    int intento= 1;
    while (intento<=5) {
      printf("Intento= %d\n", intento);
      char *ref;
      char *s= enorme(SIZE, PAT, &ref);
      int tiempo= getTime();
      char *res= eliminados(s, PAT);
      tiempo= getTime()-tiempo;
      tiempo_eliminados= tiempo;
      if (!equals(res, ref)) {
        fprintf(stderr, "Resultado incorrecto en benchmark de eliminados\n");
        exit(1);
      }
      free(s);
      free(res);
      free(ref);
      printf("Tiempo para su version= %d milisegundos\n", tiempo);
      if (tiempo_eliminados_prof==0)
        break;
      double q= (double)tiempo/(double)tiempo_eliminados_prof;
      int porciento= (q-1.)*100;
      printf("Porcentaje de sobrecosto: %d %%\n", porciento);
      if (porciento<=TOLERANCIA) {
        break;
      }
      printf("Excede en mas del %d %% la version del profesor\n", TOLERANCIA);
      if (intento<5)
        printf("Se hara un nuevo intento\n");
      intento++;
    }
    if (intento>5) {
      fprintf(stderr, "Lo siento: Despues de 5 intentos no satisface "
                      "la eficiencia requerida\n");
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

  if (argc==2 && strcmp(argv[1], "ref")==0) {
    FILE *out= fopen("tiempo-ref.txt", "w");
    if (out==NULL) {
      perror("tiempo-ref.txt");
      exit(1);
    }
    fprintf(out, "%d %d\n", tiempo_eliminar, tiempo_eliminados);
    fclose(out);
  }

  printf("Felicitaciones!  Aprobo todos los tests.\n");

  return 0;
}
