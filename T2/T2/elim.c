#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "elim.h"

void eliminar(char *str, char *pat) {
  //-----------------------Punteros--------------------//
  char *Puntinicio = str;//Puntero al inicio del string
  char *PuntStr =str;//Puntero que se movera por el string para realizar las comparaciones
  char *PuntPat =pat;//Puntero a la secuencia buscada
  char *PuntElim = PuntStr;//Puntero que guardara la posicion a la que apunta PuntStr antes de realizar la comparacion
  //--------------------------------------------------//

  int contador = 0;//Contador que guardara la canditad de caracteres iguales
  //-------------------------------------------------//

  while (*PuntStr){//El loop sigue mientras *PuntStr !='\0'

    contador = 0;//Reinicio el contador
    PuntPat = pat; //Reinicio el Puntero de la secuencia
    PuntElim = PuntStr;//Coloco la posicion de PuntElim a la posicion de PuntStr

    if (*PuntStr == *PuntPat){//Si el valor de Puntstr es igual al valor de PuntPat entro a el loop

      while (*PuntStr == *PuntPat && *PuntPat){ //Si los caracteres son iguales y todavia me queda caracteres en la secuencia a buscar paso al siguiente.
        
        contador++;//Cuento cuantos caracteres iguales se han encontrado
        PuntPat++;
        PuntStr++;

      }

      if(*PuntPat){//Si la secuencia no se encontro por completo.

        while(contador != 0){//Debo agregar al string todos los caracteres que si hayan sido iguales al momento de comparar.
          *str = *PuntElim;
          str++;
          PuntElim++;
          contador--;
        }
      }
    }
    else{//Si el valor de PuntStr != PuntPat lo agrego a str y muevo los punteros.

      *str = *PuntStr;
      str++;
      PuntStr++;
    }
    
  }
  *str = 0;//Agrego el '\0' para indicar el final del string
  str = Puntinicio;//Reinicio la posicion del puntero *str al inicio del string.
}








char *eliminados(char *str, char *pat) {
  //-----------------------Punteros--------------------//
  char *Puntinicio = str;//Puntero al inicio del string
  char *PuntStr =str;//Puntero que se movera por el string para realizar las comparaciones
  char *PuntPat =pat;//Puntero a la secuencia buscada
  //--------------------------------------------------//

  int contador = 0;//Contador que guardara la canditad de veces que se repite la secuencia buscada
  int contadorlargo = 0;//Contador que vera el tamaño del patron pat
  //--------------------Tamaño Strings----------------//
  int Largo_str = 0;
  int Largo_pat = 0;
  //--------------------------------------------------//


  while (*PuntStr){//El loop sigue mientras *PuntStr !='\0'
    Largo_str ++;
    PuntPat = pat;//Reinicio el puntero de la secuencia
    contadorlargo=0;

    if (*PuntStr == *PuntPat){
      while (*PuntStr == *PuntPat && *PuntPat){ //Si los caracteres son iguales y todavia me queda carecter en la secuencia a buscar paso al siguiente.
        contadorlargo++;
        PuntPat++;
        PuntStr++;
        Largo_str++;
      }
      if (!*PuntPat){//Si encontre la secuencia entera agrego uno al contador
        contador ++;
        Largo_pat=contadorlargo;
      }
    }
    else{
      PuntStr++;
    }
  }

  //-----------------------Punteros--------------------//
  char *Result = malloc((Largo_str - (contador * Largo_pat)) + 1); //Creo un string con un espacio igual a el largo del string menos el largo de la secuencia por la cantidad de veces que aparece mas 1 para el 0.
  char *InicioResult = Result;//Puntero que guarda la posicion inicial de Resultado
  char *PuntElim = str;//Puntero que guardara la posicion a la que apunta PuntStr antes de realizar la comparacion
  //--------------------------------------------------//


  contador = 0;//Contador ahora guardarara la canditad de caracteres iguales que se encuentren.

  PuntPat = pat;//Reinicio la posicion del puntero de la secuencia
  PuntStr = Puntinicio;//Reinicio el Puntero del string al inicio
  //----------------------------------------------------------------------------------------//
  
  //Literalmente el mismo codigo que la funcion eliminar pero cambiando str por Result.
  while (*PuntStr){//El loop sigue mientras *PuntStr !='\0'

    contador = 0;//Reinicio el contador
    PuntPat = pat;//Reinicio el puntero al inicio de la secuencia
    PuntElim = PuntStr;//Coloco la posicion de PuntElim a la posicion de PuntStr

    if (*PuntStr == *PuntPat){

        while (*PuntStr == *PuntPat && *PuntPat){ //Si los caracteres son iguales y todavia me quedan caracteres en la secuencia a buscar paso al siguiente.
          
          contador++;//Cuento cuantos caracteres iguales se han encontrado
          PuntPat++;
          PuntStr++;

        }
        
        if(*PuntPat){//Si la secuencia no se encontro por completo.

          while(contador != 0){//Debo agregar al string todos los caracteres que si hayan sido iguales al momento de comparar.
            *Result = *PuntElim;
            Result++;
            PuntElim++;
            contador--;
          }

        }
      }
      else{//Si los valores de PuntStr != PuntPat lo agrego a Result y muevo los punteros.

        *Result = *PuntStr;
        Result++;
        PuntStr++;
      }
      
    }
  *Result = 0;//Agrego el '\0' para indicar el final del string
  return InicioResult;//Devuelvo el string Resultante.
}