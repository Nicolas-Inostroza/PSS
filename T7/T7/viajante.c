#define _XOPEN_SOURCE 500

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "viajante.h"

int leer(int fd, void *vbuf, int n) {
  char *buf= vbuf;
  do {
    int rc= read(fd, buf, n);
    if (rc<=0)
      return 1; /* fracaso: error o fin del archivo/pipe/socket */
    n-= rc; /* descontamos los bytes leídos */
    buf+= rc; /* avanzamos el buffer para no reescribir lo leido previamente */
  } while (n>0); /* mientras no leamos todo lo que esperamos */
  return 0; /* exito */
}

double viajante_par(int z[], int n, double **m, int nperm, int p) {
  if (p<=1){
    return viajante(z[],n,m,nperm/p);
  }
  else{
    int fds[2];
    pipe(fds);
    pid_t pid= fork()
    srandom(getUSecsOfDay()*getpid());
    if(pid==0){
      close(fds[0]);
      int nperm_hijos = viajante_par(z[],n,m,nperm,p-1); 
      write(fds[1],&nperm_hijos,sizeof(double));
      exit(0);
    }
    else{
      close(fds[1]);
      int min;
      leer(fds[0],&min,sizeof(int));
      close(fds[0]);
      waitpid(pid,NULL,0);
      if ()

    }

  }

  // Complete esta funcion
  // Puede invocar a su version secuencial: viajante
  // Use la funcion leer para que el padre reciba la ruta del hijo.
  // Use fork() para crear p nuevos procesos.  El padre solo obtiene los
  // resultados de los procesos hijos a traves de pipes, entierra a los
  // los hijos y calcula el resultado final.
  // Despues de invocar fork() agregue: srandom(getUSecsOfDay()*getpid());
  // Esto es para que cada proceso genere secuencias de numeros aleatorios
  // diferentes.
}
