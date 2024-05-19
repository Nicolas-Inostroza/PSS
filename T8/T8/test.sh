probar() {
  BIN=$1
  DIR=$2
  N=$3
  REF=$4
  echo "Ejecutando: ./$BIN $DIR $N > salida.txt"
  (./$BIN $DIR $N > salida.txt) >& err.txt
  RC=$?
  echo "La salida estandar es:"
  echo "-------------------------------------------"
  cat salida.txt
  echo "-------------------------------------------"
  if [ $RC -ne 0 ]
  then
    echo
    echo "*** El codigo de retorno fue $RC. Debio ser 0 ***"
    echo "El codigo de retorno es el valor que retorna la funcion main"
    echo "o el argumento de la funcion exit"
    echo "La salida estandar de errores fue:"
    echo "-------------------------------------------"
    cat err.txt
    echo "-------------------------------------------"
    exit 1
  fi
  echo "Comparando salida estandar con salida esperada con:"
  echo "diff salida.txt $REF"
  diff salida.txt $REF
  if [ $? -ne 0 ]
  then
    echo
    echo "*** La salida estandar es incorrecta ***"
    echo "Debio ser:"
    echo "-------------------------------------------"
    cat $REF
    echo "-------------------------------------------"
    echo "Si la salida parece ser igual, instale xxdiff con:"
    echo "    sudo apt-get install xxdiff"
    echo "y luego compare los caracteres invisibles con:"
    echo "xxdiff salida.txt $REF"
    exit 1
  fi
  cmp err.txt empty.txt
  if [ $? -ne 0 ]
  then
    echo
    echo "*** La salida estandar de errores es incorrecta ***"
    echo "Debio estar vacia"
    echo "La salida estandar de errores fue:"
    echo "-------------------------------------------"
    cat err.txt
    echo "-------------------------------------------"
    exit 1
  fi
  echo Aprobado
}

probar $1 dir 2  dir-ref.txt

if [ ! -d /opt/riscv ] ; then
  echo Necesita instalar el toolchain de riscv en /opt/riscv
  exit 1
fi

probar $1 /opt/riscv 10 opt-riscv-ref-$(arch).txt
