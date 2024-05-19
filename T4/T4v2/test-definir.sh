#!/usr/bin/bash

trap "rm -rf /tmp/dicc.txt" EXIT
ARCH=$(arch)
DEFINIR=./$1
chmod +x prof.ref-$ARCH

clean() {
  rm -rf /tmp/dicc.txt
}

test() {
  DICC=$1
  LLAVE=$2
  VAL=$3
  EXTRA=$4
  if [ -f $DICC ] && [ -w $DICC ] ; then
    cp $DICC dicc-ref.txt
  fi
  echo "Ejecutando: $DEFINIR $DICC $LLAVE" '"'$VAL'"' $EXTRA
  ( $DEFINIR $DICC $LLAVE "$VAL" $EXTRA > raw-std.txt ) >& err.txt
  RC=$?
  cat raw-std.txt | xargs > std.txt
  if [ -f $DICC ] && [ -w $DICC ] ; then
    mv $DICC dicc.bak
    mv dicc-ref.txt $DICC
  fi
  ( ./prof.ref-$ARCH $DICC $LLAVE "$VAL" $EXTRA > raw-std-ref.txt ) >& err-ref.txt
  REFRC=$?
  cat raw-std-ref.txt | xargs > std-ref.txt
  if [ -f $DICC ] && [ -w $DICC ] ; then
    mv $DICC dicc-ref.txt
    mv dicc.bak $DICC
  fi

  cmp err.txt err-ref.txt
  if [ $? -ne 0 ]
  then
    echo "*** Salida estandar de errores incorrecta ***"
    echo "Su solucion entrega en la salida estandar de errores:"
    echo "============================================================"
    cat err.txt
    echo "============================================================"
    echo "Debio ser:"
    echo "============================================================"
    cat err-ref.txt
    echo "============================================================"
    echo "Si la salida parece ser igual, instale xxdiff con:"
    echo "    sudo apt-get install xxdiff"
    echo "y luego compare los caracteres invisibles con:"
    echo "xxdiff err.txt err-ref.txt"
#   clean
    exit 1
  fi
  cat err.txt

  cmp std.txt std-ref.txt
  if [ $? -ne 0 ]
  then
    echo "*** Salida estandar incorrecta ***"
    echo "Su solucion entrega en la salida estandar:"
    echo "============================================================"
    cat std.txt
    echo "============================================================"
    echo "Debio ser:"
    echo "============================================================"
    cat std-ref.txt
    echo "============================================================"
    echo "Si la salida parece ser igual, instale xxdiff con:"
    echo "    sudo apt-get install xxdiff"
    echo "y luego compare los caracteres invisibles con:"
    echo "xxdiff std.txt std-ref.txt"
#   clean
    exit 1
  fi
  cat raw-std.txt

  if [ "$RC" == 0 ]
  then
    if [ "$REFRC" != 0 ]
    then
      echo "El codigo de retorno es incorrectamente 0.  Debio ser $REFRC"
#     clean
      exit 1
    fi
  fi
  if [ "$RC" != 0 ]
  then
    if [ "$REFRC" == 0 ]
    then
      echo "El codigo de retorno es incorrectamente $RC.  Debio ser 0"
#     clean
      exit 1
    fi
  fi
  if [ "$RC" != 0 ]
  then
    echo "Bien.  Se diagnostico correctamente el error."
  fi

  if [ -f $DICC ] && [ -w $DICC ] ; then
    diff $DICC dicc-ref.txt
    if [ $? -ne 0 ]
    then
      echo "*** El diccionario resultante es incorrecto ***"
      echo "Se uso el comando diff para mostrar las direrencias con el"
      echo "diccionario de referencia"
      echo "Para visualizar mejor la diferencias, instale xxdiff con:"
      echo "    sudo apt-get install xxdiff"
      echo "y luego compare los archivos con"
      echo "xxdiff $DICC dicc-ref.txt"
#     clean
      exit 1
    fi
  fi
}

echo "-----------------------------------------------------------"
echo "Tests unitarios"
echo "Restaurando dicc.txt (una copia se encuentra en dicc-ini.txt)"
bash mk-dicc.sh
cp dicc.txt dicc-ini.txt
test dicc.txt embarcacion "todo tipo de artilugio capaz de navegar en el agua"
test dicc.txt casa "edificacion construida para ser habitada"
test dicc.txt lluvia "precipitacion de particulas liquidas de agua"
test dicc.txt alimento "sustancia ingerida por un ser vivo"
test dicc.txt celular "aparato portatil de un sistema de telefonia celular"


echo "-----------------------------------------------------------"
echo "Test con llave existente"
test dicc.txt alimento "sustancia ingerida por un ser vivo"

echo "-----------------------------------------------------------"
echo "Test con un diccionario inexistente"
test nodicc.txt bolsillo "bla bla"
echo "-----------------------------------------------------------"
echo "Test con diccionario sin permiso de escritura"
cp dicc.txt /tmp/
chmod -w /tmp/dicc.txt
test /tmp/dicc.txt evangelio "bla bla"
rm -f /tmp/dicc.txt

echo "-----------------------------------------------------------"
echo "Test de uso incorrecto de parametros"
test dicc.txt bolsillo "bolsa pequena" error
test bolsillo

echo "-----------------------------------------------------------"
echo "Test de definir con un diccionario completamente lleno"
test dicc-full.txt consejo "bla bla"

echo
echo "Felicitaciones: aprobo todos los tests"
