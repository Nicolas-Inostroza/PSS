#include <stdio.h>

#include "suma.h"

Bcd sumaBcd(Bcd x, Bcd y) {
  //Definimos las variables.

  Bcd SobreDiez = 0;//Variable que tiene valores 0, si la suma de digitos es inferior a 10, y 1 en caso contrario.

  Bcd Resultado = 0;//Variable donde se guardara el resultado final

  Bcd Mascara = 0xf;//Mascara 1111
  int CantidadShifts=0;//Contador para la cantidad de shifts
  Bcd digitoX;
  Bcd digitoY;


  for (int i=0; i<16;i++){ //Ponemos como restriccion i<16 para que tome solo 16 digitos.

    // Obtengo el primer digito de los numeros x e y
    digitoX = x & Mascara;
    digitoY = y & Mascara;

    Bcd SumDigitos = digitoY + digitoX + SobreDiez;//Suma de los digitos de cada numero y del valor acarreado "SobreDiez"

    //Tengo que ver si es mayor o igual que 10, si es el caso debo sumar 1 al siguiente digito y restar 10 al actual.
    if (SumDigitos >= 10){
      SobreDiez = 1;
      SumDigitos -= 10;
    }
    else{ //Si no es 10 o mayor no debo sumar nada al siguiente digito
      SobreDiez = 0;
    }
    //Shifteo los numeros x e y 4 veces hacia la derecha para dejar el siguiente digito en la primera posiciion.
    x = (x >> 4);
    y = (y >> 4);

    //Con un "or" coloco el resultado de la suma en la variable "Resultado", la variable CantidadShift lo coloca en al posicion correspondiente.
    Resultado = Resultado | (SumDigitos << CantidadShifts);
    CantidadShifts +=4; //Sumo 4 a la cantidad de shifts que debo realizar para pasar a la posicion del siguiente digito.
  }

  if (SobreDiez){ //Si SobreDiez vale 1 entonces el resultado seria de 17 digitos por lo que retorna 0xff...f
    return 0xffffffffffffffff;
  }
  return Resultado;//Si el Resultado tiene 16 digitos o menos devolvera el resultado.

}
