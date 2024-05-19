#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdarg.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "pss.h"

// Defina aca la estructura para guardar nombre y tamanno
typedef struct{
  char *nombre;
  int tam;
} guardado;

// Agregue aca las funciones y variables globales adicionales que necesite
Queue *cola;

void listDir(char *nom) {
  struct stat st_nom;
  int rc;
  rc= stat(nom, &st_nom);

  if (rc!=0) {
    printf("%s no existe\n", nom);
    exit(0);
  }
  if (S_ISREG(st_nom.st_mode)) {
    // Es un archivo regular
    // No se hace nada en list-dir.bin
    guardado *archivo = (guardado *)malloc(sizeof(guardado));

    archivo->nombre = (char *)malloc(strlen(nom)+1);
    strcpy(archivo->nombre,nom);
    archivo->tam = st_nom.st_size;
    put(cola,archivo);
  }
  else if (S_ISDIR(st_nom.st_mode)) {
    // Es un directorio
    DIR *dir = opendir(nom);
    if (dir == NULL) {
      perror(nom);
      exit(0);
    }
    for (struct dirent *entry = readdir(dir);
         entry != NULL;
         entry = readdir(dir)) {
      if (strcmp(entry->d_name, ".")==0 || strcmp(entry->d_name, "..")==0) {
        continue;
      }
      char *nom_arch= malloc(strlen(nom)+strlen(entry->d_name)+2);
      strcpy(nom_arch, nom);
      strcat(nom_arch, "/");
      strcat(nom_arch, entry->d_name);
      listDir(nom_arch);
      free(nom_arch);
    }
    closedir(dir);
  }
  else {
    // Podria ser un dispositivo, un link simbolico, etc.
    fprintf(stderr, "Archivo %s es de tipo desconocido\n", nom);
    exit(0);
  }
}

int comparar(void *ptr, int i, int j){
  guardado **a= ptr;
  if (a[j]->tam == a[i]->tam){
    return strcmp(a[i]->nombre,a[j]->nombre);
  }
  return a[j]->tam - a[i]->tam;
}

int main(int argc, char *argv[]) {
  // ... complete ...
  if (argc!=3) {
    fprintf(stderr, "uso: %s <arch|dir>\n", argv[0]);
    exit(0);
  }
  int numero = atoi(argv[2]);
  cola = makeQueue();
  listDir(argv[1]);
  int largo_cola = queueLength(cola);
  guardado *list[largo_cola];

  for(int i=0;i<largo_cola;i++){
    list[i] = (guardado*)get(cola);
  }

  sortPtrArray(list, 0,largo_cola-1,comparar);

  for(int i=0;i<numero;i++){
    printf("%s %d\n",list[i]->nombre,list[i]->tam);

  }
  for(int i =0;i<largo_cola;i++){
    free(list[i]->nombre);
    free(list[i]);
  }
  return 0;
}

