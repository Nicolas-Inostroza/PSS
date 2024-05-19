#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "elim.h"

void eliminar(char *str, char *pat) {
  char *Puntinicio = str;
  char *PuntStr =str;
  char *PuntPat =pat;
  char *PuntElim = str;
  int contador = 0;


  while (*PuntStr != '\0'){
    PuntPat = pat;
    if (*PuntStr == *PuntPat){

      while (*PuntStr == *PuntPat && *PuntPat !='\0' ){ //Si los caracteres son iguales y todavia me queda carecter en la secuencia a buscar paso al siguiente.
        contador+=1;
        PuntPat++;
        PuntStr++;
      }
      if (*PuntPat == '\0'){
        contador = 0;
        PuntElim = PuntStr;
      }
      else{

        while(contador!=0){
          *str = *PuntElim;
          str++;
          PuntElim++;
          contador-=1;
        }
        *str = '\0';
      }
    }
    else{
      *str = *PuntStr;
      str++;
      PuntStr++;
    }
    
  }
  *str = '\0';
  str = Puntinicio;
}

void main(){
    char *hola[]="hola"
    eliminar(hola,"o");
    sprintf("%s",hola);
    
}




char *eliminados(char *str, char *pat) {
  char *Puntinicio = str;
  char *PuntStr =str;
  char *PuntPat =pat;
  int contador = 0;
  int Largo_str = strlen(str);
  int Largo_pat = strlen(pat);
  while (*str){
    PuntPat = pat;
    PuntStr = str;
    while (*PuntStr == *PuntPat && *PuntPat !='\0' ){ //Si los caracteres son iguales y todavia me queda carecter en la secuencia a buscar paso al siguiente.

      PuntPat++;
      PuntStr++;
    }
    if (*PuntPat == '\0'){
      contador += 1;
      }
    str++;
  }
  char *Result = malloc((Largo_str - (contador * Largo_pat)) + 1);
  char *InicioResult = Result;
  PuntPat = pat;
  PuntStr = Puntinicio;
  while(*PuntStr !='\0'){
    if(*PuntPat == '\0'){
      PuntPat = pat;
    }
    else if(*PuntStr != *PuntPat){
      *Result = *PuntStr;
      PuntStr++;
      Result++;
    }
    else{
      PuntStr++;
      PuntPat++;
    }
  }
  *Result = '\0';
  return InicioResult;
}
