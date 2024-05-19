#define _DEFAULT_SOURCE 1
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/resource.h>

#include "reemplazar.h"

#define SIZE 20000
#define NINTENTOS 5
#define TOLERANCIA 60

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
// Funcion que construye un string grande para el test de robustez

// Entrega en str "a3+a3+a3+ ... etc. ... a3+" (a3+ se repite SIZE veces)
// En *preemplazo entrega "zzzz... etc. ...zzz" (son SIZE zetas)

static void string_grande(char *str, char **ppal) {
  // Primero se construye *ppal
  char *pal= malloc(SIZE+1);
  *ppal= pal;
  char *top_pal= pal+SIZE;
  while (pal<top_pal) {
    *pal++= 'z';
  }
  *pal++= 0;
  // Ahora el string en donde se realizaran los reemplazos
  int len_str= SIZE*3;
  char *top_str= str+len_str;
  char *s= str;
  while (s<top_str) {
    s[0]='a';
    s[1]='3';
    s[2]='+';
    s += 3;
  }
  *s= 0;
}

static void verificar_resultado(char *res, char *str, char *pal) {
  // Se verifica que pal no se haya alterado
  char *p= pal;
  char *top_pal= pal+SIZE;
  while (p<top_pal) {
    if (*p!='z') {
      fprintf(stderr, "Error: se modifico el parametro pal\n");
      exit(1);
    }
    p++;
  }
  if (*p!=0) {
    fprintf(stderr, "Error: se modifico la terminacion de pal\n");
    exit(1);
  }
  if (str!=NULL) {
    // Se invoco reemplazo.  No se debe modificar el string original
    int len_str= SIZE*3;
    char *top_str= str+len_str;
    char *s= str;
    while (s<top_str) {
      if (s[0]!='a' || s[1]!='3' || s[2]!='+') {
        fprintf(stderr, "Error: se modifico el string original\n");
        exit(1);
      }
      s += 3;
    }
    if (*s!=0) {
      fprintf(stderr,
              "Error: se modifico la terminacion del string original\n");
      exit(1);
    }
  }
  // Se verifica que el resultado sea correcto
  int len_res= SIZE*2+SIZE*SIZE;
  char *top_res= res+len_res;
  while (res<top_res) {
    if (strncmp(res, pal, SIZE)!=0) {
      fprintf(stderr, "Error: resultado incorrecto\n");
      exit(1);
    }
    res += SIZE;
    if (res[0]!='3' || res[1]!='+') {
      fprintf(stderr, "Error: resultado incorrecto\n");
      exit(1);
    }
    res += 2;
  }
  if (*res!=0) {
    fprintf(stderr, "Error: la terminacion del resultado es incorrecta\n");
    exit(1);
  }
}

int main(int argc, char *argv[]) {
  int tiempo_ref_reemplazo= 0;
  int tiempo_ref_reemplazar= 0;
  int bench= 0;            // si se hace o no el benchmark

  if (argc>=2) {
    bench= 1;
    if (strcmp(argv[1], "ref")!=0) {
      tiempo_ref_reemplazo= atoi(argv[1]);
      tiempo_ref_reemplazar= atoi(argv[2]);
    }
  }

  {
    printf("Tests de reemplazo\n");
    char *src= "";
    char *res= reemplazo(src, 'z', "ca va");
    if (strcmp(res, src)!=0) {
      fprintf(stderr, "reemplazo(\"%s\", 'z', \"ca va\") es \"%s\", ", src, res);
      fprintf(stderr, "debio ser \"%s\"\n", src);
      exit(1);
    }
    free(res);

    src= "bonjour a tous";
    res= reemplazo(src, 'z', "ca va");
    if (strcmp(res, src)!=0) {
      fprintf(stderr, "reemplazo(\"%s\", 'z', \"ca va\") es \"%s\", ", src, res);
      fprintf(stderr, "debio ser \"%s\"\n", src);
      exit(1);
    }
    free(res);

    src= "hola que tal";
    res= reemplazo(src, 'a', "xyz");
    if (strcmp(res, "holxyz que txyzl")!=0) {
      fprintf(stderr, "reemplazo(\"%s\", 'a', \"xyz\") es \"%s\", ", src, res);
      fprintf(stderr, "debio ser \"holxyz que txyzl\"\n");
      exit(1);
    }
    free(res);

    src= "hola que tal";
    res= reemplazo(src, 'a', "");
    if (strcmp(res, "hol que tl")!=0) {
      fprintf(stderr, "reemplazo(\"%s\", 'a', \"\") es \"%s\", ", src, res);
      fprintf(stderr, "debio ser \"hol que tl\"\n");
      exit(1);
    }
    free(res);

    printf("Test de robustez\n");
    char *str= malloc(SIZE*3+1);
    char *pal;
    string_grande(str, &pal);
    res= reemplazo(str, 'a', pal);
    verificar_resultado(res, str, pal);
    free(pal);
    free(str);
    free(res);

    printf("Aprobado\n");
  }

  {
    printf("Tests de reemplazar\n");
    //                0123456789012 3456
    char src[25]="%&/(hola que tal\0    )=?";
    char *r= &src[4]; // "hola que tal"
    // La idea es que antes de r estan los caracters %&/( que no deben
    // modificarse.  Despues de r estan )=? que tampoco deben modificarse.
    reemplazar(r, 'a', "opa");
    // r es "holopa que topal"
    if (strcmp(r, "holopa que topal")!=0) {
      fprintf(stderr, "reemplazar(r, 'a', \"opa\") es \"%s\", ", r);
      fprintf(stderr, "debio ser \"holopa que topal\"\n");
      exit(1);
    }
    // verifica que no se haya modificado antes de r o despues de r
    if (strncmp(src,"%&/(",4)!=0) {
      fprintf(stderr, "reemplazar(r, 'a', \"opa\") modifico antes de r\n");
      exit(1);
    }
    if (strcmp(&src[4+17], ")=?")!=0) {
      fprintf(stderr, "reemplazar(r, 'a', \"opa\") modifico despues de r\n");
      exit(1);
    }

    reemplazar(r, 'o', "");
    //       01234567890123
    // r es "hlpa que tpal"
    if (strcmp(r, "hlpa que tpal")!=0) {
      fprintf(stderr, "reemplazar(r, 'o', \"\") es \"%s\", ", r);
      fprintf(stderr, "debio ser \"hlpa que tpal\"\n");
      exit(1);
    }
    // verifica que no se haya modificado antes de r o despues de r
    if (strncmp(src,"%&/(",4)!=0) {
      fprintf(stderr, "reemplazar(r, 'a', \"\") modifico antes de r\n");
      exit(1);
    }
    if (strcmp(&src[4+17], ")=?")!=0) {
      fprintf(stderr, "reemplazar(r, 'a', \"\") modifico despues de r\n");
      exit(1);
    }
    strcpy(&r[14], ")=?"); // para verificar que no se modifique despues de r
    reemplazar(r, 'a', "*");
    // r es "hlp* que tp*l"
    if (strcmp(r, "hlp* que tp*l")!=0) {
      fprintf(stderr, "reemplazar(r, 'a', \"*\") es \"%s\", ", r);
      fprintf(stderr, "debio ser \"hlp* que tp*l\"\n");
      exit(1);
    }
    // verifica que no se haya modificado antes de r o despues de r
    if (strncmp(src,"%&/(",4)!=0) {
      fprintf(stderr, "reemplazar(r, 'a', \"\") modifico antes de r\n");
      exit(1);
    }
    if (strcmp(&src[4+14], ")=?")!=0) {
      fprintf(stderr, "reemplazar(r, 'a', \"\") modifico despues de r\n");
      exit(1);
    }
    reemplazar(r, 'a', "*");

    printf("Test de robustez\n");
    char *str= malloc(SIZE*2+SIZE*SIZE+1);
    char *pal;
    string_grande(str, &pal);
    reemplazar(str, 'a', pal);
    verificar_resultado(str, NULL, pal);
    free(pal);
    free(str);

    printf("Aprobado\n");
  }

  int tiempo_reemplazo= 0, tiempo_reemplazar= 0;

  if (bench) {
    printf("Benchmark de reemplazo\n");
    printf("Tiempo tomado por la solucion del profesor: %d milisegundos\n",
           tiempo_ref_reemplazo);
    int intento= 1;
    while (intento<=NINTENTOS) {
      char *str= malloc(SIZE*3+1);
      char *pal;
      string_grande(str, &pal);

      // La medicion del tiempo de ejecucion
      resetTime();
      for (int k= 0; k<100; k++) {
        char *res= reemplazo(str, 'a', pal);
        free(res);
      }
      tiempo_reemplazo= getTime();
      printf("Tiempo de cpu usado por su version: %d milisegundos\n",
             tiempo_reemplazo);
      free(pal);
      free(str);

      if (argc!=3) {
        break;
      }
      else {
        if (tiempo_ref_reemplazo==0) {
          break;
        }
        else {
          double q= (double)tiempo_reemplazo/(double)tiempo_ref_reemplazo;
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
      }
    }

    printf("Benchmark de reemplazar\n");
    printf("Tiempo tomado por la solucion del profesor: %d milisegundos\n",
           tiempo_ref_reemplazar);
    intento= 1;
    while (intento<=NINTENTOS) {
      char *str_orig= malloc(SIZE*3);
      char *str= malloc(SIZE*2+SIZE*SIZE+1);
      char *pal;
      string_grande(str_orig, &pal);

      // La medicion del tiempo de ejecucion
      resetTime();
      for (int k= 0; k<100; k++) {
        strcpy(str, str_orig);
        reemplazar(str, 'a', pal);
      }
      tiempo_reemplazar= getTime();
      printf("Tiempo de cpu usado por su version: %d milisegundos\n",
             tiempo_reemplazar);
      free(pal);
      free(str);

      if (argc==3) {
        if (tiempo_ref_reemplazar==0) {
          break;
        }
        else {
          double q= (double)tiempo_reemplazar/(double)tiempo_ref_reemplazar;
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
      }
      else {
        FILE *out= fopen("tiempo-ref.txt", "w");
        if (out==NULL) {
          perror("tiempo-ref.txt");
          exit(1);
        }
        fprintf(out, "%d %d\n", tiempo_reemplazo, tiempo_reemplazar);
        fclose(out);
        break;
      }
    }
  }

  printf("Felicitaciones!\n");

  return 0;
}

