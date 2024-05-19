#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "sort.h"

#define NFRASES 10

static int verboso;

static char *data[]= {
"Soy inseguro, ¿o no?", // 4
"Defina  universo y de 2  ejemplos", // 6
"Vida : enfermedad mortal de transmision sexual", // 7
"Para entender la recursividad debe entender la recursividad  ", // 8
"El futuro ya   no es lo que solia ser", // 9
"Como se llama   el animalito, lo ignoro, bonito   el loignorito", // 10
"Erase una vez un cuento corto que vivio feliz para siempre", // 11
"    Los 4 puntos cardinales   son 3 : el norte y el sur", // 12
"No creo en el horoscopo porque soy cancer y los cancer somos excepticos", // 13
"Hay 10 tipos de personas : las que entienden binario y las que no", // 14
};

static int res[]= { 4, 6, 7, 8, 9, 10, 11, 12, 13, 14 };

static char *desordenado[]= {
"Vida : enfermedad mortal de transmision sexual", // 7
"Hay 10 tipos de personas : las que entienden binario y las que no", // 14
"No creo en el horoscopo porque soy cancer y los cancer somos excepticos", // 13
"Para entender la recursividad debe entender la recursividad  ", // 8
"Erase una vez un cuento corto que vivio feliz para siempre", // 11
"    Los 4 puntos cardinales   son 3 : el norte y el sur", // 12
"Como se llama   el animalito, lo ignoro, bonito   el loignorito", // 10
"El futuro ya   no es lo que solia ser", // 9
"Defina  universo y de 2  ejemplos", // 6
"Soy inseguro, ¿o no?", // 4
};

static void help(int line, char *s0, char *s1) {
  printf("Para depurar use estos comandos:\n");
  printf("make sort-rv-wc.ddd\n");
  printf("o tambien make sort-c-wc.ddd\n");
  printf("Ingrese estos comandos en el panel de comandos de ddd:\n");
  printf("break test2\n");
  printf("cond 3 s0==%p && s1==%p\n", s0, s1);
  printf("continue\n");
  printf("Abra el panel de assembler seleccionando View -> "
         "Machine Code Window\n");
  printf("Avance con next hasta la invocacion de la funcion sort.\n");
  printf("Enseguida depure con stepi para avanzar lentamente.\n");
  printf("El arreglo tiene solo 2 elementos, no va a demorarse mucho.\n");
  exit(1);
}

static void dump(char *a[], int n) {
  for (int i= 0; i<n; i++) {
    printf("  \"%s\"\n", a[i]);
  }
}

static void test2(char *msg, char *s0, char *s1, int ordenado) {
  if (verboso)
    printf("%s", msg);
  if (verboso)
    printf("Arreglo es:\n  \"%s\"\n  \"%s\"\n", s0, s1);
  char *str[ ]= { s0, s1 };
  sort(str, 2);
  if ( (  ordenado && (strcmp(str[0], s0)!=0 || strcmp(str[1], s1)!=0) ) ||
       ( !ordenado && (strcmp(str[1], s0)!=0 || strcmp(str[0], s1)!=0) ) ) {
    if (!verboso) {
      printf("%s", msg);
      printf("Arreglo %s es:\n  \"%s\"\n  \"%s\"\n",
             ordenado ? "ordenado" : "desordenado", s0, s1);
    }
    fprintf(stderr, "El resultado es incorrecto:\n");
    dump(str, 2);
    help(98, s0, s1);
    exit(1);
  }
}

int main(int argc, char *argv[]) {
  verboso= 1;
  printf("Primera serie de tests con arreglos de solo 2 frases\n");
  test2("Ambas frases vacias\n", "", "", 1);
  test2("\nFrases iguales\n", "=", "=", 1);
  test2("Primera frase menor que la segunda\n", "", "a", 1);
  test2("Primera frase mayor que la segunda\n", "b", "", 0);
  printf("\nFrases con palabras de mas de un caracter y varios espacios\n");
  test2("Frase con igual numero de palabras\n", " aa ", "  b  ", 1);
  test2("Primera frase menor que la segunda\n", "  aaa  ", "b b", 1);
  test2("Primera frase mayor que la segunda\n", " abc def ", "ggggggg", 0);
  test2("Muchas palabras, menor\n", "  a     b  c  ", "d e f g", 1);
  test2("Muchas palabras, mayor\n", "  a  b  c  ", "           d    e    ", 0);

  printf("\nSegunda serie de tests con chistes cortos\n");
  verboso= 0;

  for (int i= 0; i<NFRASES; i++)
    for (int j= 0; j<NFRASES; j++)
      test2("alfabetico\n", data[i], data[j], res[i]<=res[j]);

  printf("Aprobado\n");

  printf("\nTercera serie de tests con todo el arreglo de chistes\n");

  printf("El arreglo desordenado es:\n");
  printf("--------------------------\n");
  for (int i= 0; i<NFRASES; i++) {
    printf("%s\n", desordenado[i]);
  }
  printf("\n");
  sort(desordenado, NFRASES);
  printf("\n");
  printf("El arreglo ordenado es:\n");
  printf("-----------------------\n");
  for (int i= 0; i<NFRASES; i++) {
    printf("%s\n", desordenado[i]);
  }
  printf("\n");
  for (int i= 0; i<NFRASES; i++) {
    if (data[i]!=desordenado[i]) {
      fprintf(stderr, "El %d-esimo elemento es incorrecto\n", i);
      fprintf(stderr, "  \"%s\"\n debio ser\n  \"%s\"\n",
              desordenado[i], data[i]);
      exit(1);
    }
  }

  printf("Felicitaciones: todos los tests aprobados\n");
  return 0;
}
