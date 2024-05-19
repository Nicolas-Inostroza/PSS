#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "sort.h"

#define NLETRAS 26

static int verboso;

static char *alfa[ ] = {
        "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z",
        "a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m"
};

static char *data[]= {
        "o",
        "p",
        "t",
        "u",
        "i",
        "w",
        "x",
        "r",
        "y",
        "g",
        "z",
        "v",
        "a",
        "b",
        "c",
        "d",
        "e",
        "q",
        "h",
        "m",
        "n",
        "j",
        "f",
        "s",
        "k",
        "l",
};

void help(int line, char *s0, char *s1) {
  printf("Para depurar use estos comandos:\n");
  printf("make sort-rv-rot13.ddd\n");
  printf("o tambien make sort-c-rot13.ddd\n");
  printf("Ingrese estos comandos en el panel de comandos de ddd:\n");
  printf("break test-sort.c:%d\n", line);
  printf("cond 3 s0==%p && s1==%p\n", s0, s1);
  printf("continue\n");
  printf("Abra el panel de assembler seleccionando View -> "
         "Machine Code Window\n");
  printf("Avance con stepi hasta entrar a sort-rv-rot13.s\n");
  printf("Enseguida depure con stepi para avanzar lentamente.\n");
  printf("O con step para entrar a sort-c-rot13.c\n");
  printf("El arreglo tiene solo 2 elementos, no va a demorarse mucho\n");
  exit(1);
}

static void dump(char *a[], int n) {
  printf("Contenido del arreglo\n");
  for (int i= 0; i<n; i++) {
    printf("%s\n", a[i]);
  }
}

static char *rot13(char *s) {
  int len= strlen(s);
  char *r= malloc(len+1);
  char *u= r;
  char c;
  do {
    c= *s;
    if ('a'<=c) {
      if (c<='m')
        c += 13;
      else if (c<='z')
        c -= 13;
    }
    *u= c;
    s++; u++;
  } while (c!=0);
  return r;
}

static void test2(char *msg, char *s0, char *s1, int ordenado) {
  if (verboso)
    printf("%s", msg);
  char *rs0= rot13(s0);
  char *rs1= rot13(s1);
  if (verboso)
    printf("  Arreglo %s es: \"%s\" \"%s\" (rotado es \"%s\" \"%s\")\n",
           ordenado ? "ordenado" : "desordenado",
           s0, s1, rs0, rs1);
  char *str[ ]= { s0, s1 };
  sort(str, 2);
  if ( (  ordenado && (strcmp(str[0], s0)!=0 || strcmp(str[1], s1)!=0) ) ||
       ( !ordenado && (strcmp(str[1], s0)!=0 || strcmp(str[0], s1)!=0) ) ) {
    if (!verboso) {
      printf("%s", msg);
      printf( "  Arreglo %s es: \"%s\" \"%s\" (rotado es \"%s\" \"%s\")\n",
              ordenado ? "ordenado" : "desordenado",
              s0, s1, rs0, rs1 );
    }
    fprintf(stderr, "Resultado incorrecto\n");
    dump(str, 2);
    if (!verboso)
      help(98, s0, s1);
    exit(1);
  }
  free(rs0); free(rs1);
}

int main(int argc, char *argv[]) {
  verboso= 1;
  printf("Primera serie de tests con arreglos de solo 2 strings\n");
  printf("Sin caracteres alfabeticos y strings de largo 1\n");
  test2("Ambos strings vacios\n", "", "", 1);
  test2("\nStrings iguales\n", "=", "=", 1);
  test2("Primer string menor que el segundo\n", "<", ">", 1);
  test2("Primer string mayor que el segundo\n", "-", "+", 0);
  printf("\nStrings de largo mayor que 1\n");
  test2("Strings iguales\n", "!$%&/()=?", "!$%&/()=?", 1);
  test2("Primer string menor que el segundo\n", "=<", "=>", 1);
  test2("Primer string mayor que el segundo\n", "=-", "=+", 0);

  printf("\nSegunda serie de tests ahora con caracteres alfabeticos\n");
  printf("Strings de largo 1\n");
  test2("Strings iguales\n", "a", "a", 1);
  test2("Strings iguales\n", "z", "z", 1);
  test2("Primer string menor que el segundo\n", "n", "a", 1);
  test2("Primer string mayor que el segundo\n", "m", "z", 0);
  printf("\nStrings de largo mayor que 1\n");
  test2("Strings iguales\n", "agmnsz", "agmnsz", 1);
  test2("Primer string menor que el segundo\n", "az", "aa", 1);
  test2("Primer string mayor que el segundo\n", "am", "an", 0);
  printf("\nStrings de largo distinto\n");
  test2("Primer string menor que el segundo\n", "", "abc", 1);
  test2("Primer string menor que el segundo\n", "az", "azm", 1);
  test2("Primer string menor que el segundo\n", "az", "azn", 1);
  test2("Primer string mayor que el segundo\n", "zyx", "", 0);
  test2("Primer string mayor que el segundo\n", "aza", "az", 0);
  test2("Primer string mayor que el segundo\n", "azz", "az", 0);

  printf("\nTercera serie: todos los caracteres alfabeticos\n");
  verboso= 0;

  for (int i= 0; i<NLETRAS; i++)
    for (int j= 0; j<NLETRAS; j++)
      test2("alfabetico\n", alfa[i], alfa[j], i<=j);

  printf("Aprobado\n");

  printf("\nCuarta serie: todo el arreglo de letras\n");

  printf("El arreglo desordenado es:\n");
  printf("--------------------------\n");
  for (int i= 0; i<NLETRAS; i++) {
    printf("%s ", data[i]);
  }
  printf("\n");
  sort(data, NLETRAS);
  printf("\n");
  printf("El arreglo ordenado es:\n");
  printf("-----------------------\n");
  for (int i= 0; i<NLETRAS; i++) {
    printf("%s ", data[i]);
  }
  printf("\n");
  for (int i= 0; i<NLETRAS; i++) {
    if (strcmp(data[i], alfa[i])!=0) {
      fprintf(stderr, "El %d-esimo elemento es incorrecto\n", i);
      exit(1);
    }
  }

  printf("Felicitaciones: todos los tests aprobados\n");
  return 0;
}
