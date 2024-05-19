#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "pss.h"

//... define variables globales o funciones adicionales ...
void colocarString(char *nombre_archivo, char *llave, char *definicion){

  FILE *archivo=fopen(nombre_archivo, "r+");
  
  if(archivo == NULL){
    perror(nombre_archivo);
    exit(-1);
  }

  //--Obtengo e largo del archivo y defino un char del tamaño de la linea que buscamos--//
  int LargoLinea = 80;
  //-------------------------------------------------------------------------------------//
  fseek(archivo, 0, SEEK_END);
  int LargoArchivo = ftell(archivo);
  int Numero_Lineas = LargoArchivo / LargoLinea;
  int Lineacorres = hash_string(llave) % Numero_Lineas;
  //---------------------------------------------------------------------------------//

  //--Nos desplazamos hacia la linea correspondiente --------------------------------//
  fseek(archivo, Lineacorres*(LargoLinea+1), SEEK_SET);
  
  //-------------------------------------------------------------------------------------//
  int LineaEncontrada = Lineacorres + 1;
  int ultima_Linea = LargoArchivo-(LargoLinea+1);
  char *ini=llave;
  while(LineaEncontrada != Lineacorres){

    int posicionactual= ftell(archivo);
    char comp[LargoLinea+1];
    fread(comp, LargoLinea+1, 1,  archivo);
    fseek(archivo,posicionactual,SEEK_SET);
    llave = ini;
    char *comparador=comp;

    if(*comparador == *llave){ //Veo si la llave se encuentra en la linea
      while (*llave == *comparador && *llave!=0){    
        llave++;
        comparador++;
      }
      if(*llave ==0){//Si la llave ya existe
        fprintf(stderr,"La llave %s ya se encuentra en el diccionario\n",ini);
        exit(1);
      }
    }


    if(comp[0] == ' '){//Si la linea designada esta vacia
      fprintf(archivo,"%s:%s",llave,definicion);
      break;
    }

    fseek(archivo,LargoLinea+1,SEEK_CUR); //Voy a la siguiente linea
    LineaEncontrada +=1;

    if(posicionactual == ultima_Linea){//Si ya revise la ultima linea vuelvo al inicio del archivo
      fseek(archivo,0,SEEK_SET);
      LineaEncontrada = 0;
    }
  }
  if (LineaEncontrada==Lineacorres){ //Si realize una vuelta completa
    fprintf(stderr,"%s: el diccionario esta lleno\n",nombre_archivo);
    exit(1);
  }
  fclose(archivo);
  return;
}




int main(int argc, char *argv[]) {
  if (argc !=4){//Si se reciven una cantidad diferente a la necesaria al llamar el archivo
    fprintf(stderr,"Uso: ./definir <diccionario> <llave> <definicion>\n");
    exit(-1);
  }
  
  colocarString(argv[1],argv[2],argv[3]);

  return 0;

}
