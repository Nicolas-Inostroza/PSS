#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>

#include "pss.h"

typedef struct {
  char *nom;
  int edad;
} Persona;

// Ejemplo que ordena un arreglo de punteros

int cmpPersonas(void *ptr, int i, int j) {
  Persona **a= ptr;
  return a[i]->edad - a[j]->edad;
}

void ordenarArregloDePunteros() {
  Persona pedro= {"pedro", 5}, juan={"juan", 10}, diego={"diego", 3};
  Persona *ppersonas[ ] = { &pedro, &juan, &diego };
  sortPtrArray(ppersonas, 0, 2, cmpPersonas);
  for (int i= 0; i<3; i++)
    printf("%s %d\n", ppersonas[i]->nom, ppersonas[i]->edad);
}

// Ejemplo que ordena un arreglo de elementos que no son punteros

int cmpPersonas2(void *ptr, int i, int j) {
  Persona *a= ptr;
  return a[i].edad - a[j].edad;
}

void swapPersonas2(void *ptr, int i, int j) {
  Persona *personas= ptr;
  Persona tmp= personas[i];
  personas[i]= personas[j];
  personas[j]= tmp;
}

void ordenarArregloGeneral() {
  Persona personas[ ]= { {"juan", 10}, {"diego", 3}, {"pedro", 5}, };
  sort(personas, 0, 2, cmpPersonas2, swapPersonas2);
  for (int i= 0; i<3; i++)
    printf("%s %d\n", personas[i].nom, personas[i].edad);
}

int main() {
  ordenarArregloDePunteros();
  printf("\n");
  ordenarArregloGeneral();
  return 0;
}
