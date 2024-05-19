==========================================================
Esta es la documentación para compilar y ejecutar su tarea
==========================================================

Se está ejecutando el comando: less README.txt

***************************
*** Para salir: tecla q ***
***************************

Para avanzar a una nueva página: tecla <page down>
Para retroceder a la página anterior: tecla <page up>
Para avanzar una sola línea: tecla <enter>
Para buscar un texto: tecla / seguido del texto (/...texto...)
         por ejemplo: /ddd

-----------------------------------------------

Ud. debe programar su solución en el archivo definir.c.

Debe probar su tarea bajo Debian 11 de 64 bits nativo o virtualizado.
Queda excluido WSL 1 para hacer las pruebas.  Sí puede usar WSL 2.
Estos son los requerimientos para aprobar su tarea:

+ make run-san debe felicitarlo y no debe reportar ningún problema como
  por ejemplo memory leaks.
+ make run-g debe felicitarlo.
+ make run debe felicitarlo.

Invoque el comando make zip para ejecutar todos los tests y generar un
archivo definir.zip que contiene definir.c, con su solución,
y resultados.txt, con la salida de make run, make run-g y make run-san.

Para ejecutar la solución de referencia, ejecute estos comandos:

ARCH=$(arch)
bash test-definir.sh prof.ref-$ARCH

Para depurar use: make ddd

Recuerde que para ejecutar el programa debe ingresar run en el panel de
comandos de ddd, especificando los parámetros.  Por ejemplo:

run dicc.txt bolsillo "bolsa pequena"

Video con ejemplos de uso de ddd: https://youtu.be/FtHZy7UkTT4
Archivos con los ejemplos: https://www.u-cursos.cl/ingenieria/2020/2/CC3301/1/novedades/r/demo-ddd.zip

-----------------------------------------------

Nuevo!

Si prefiere codeblocs, láncelo y abra el archivo definir.cbp.
Hay 3 perfiles de ejecución para probar su tarea:
+ Debug (equivale a make run-g)
+ Sanitize (make run-san)
+ Release (make run)

Solo prueba para el caso ./definir.bin dicc.txt bolsillo "bolsa pequena"
Para probar con otros parámetros, en codeblock vaya al menú Project -> Set
programs' arguments... En la ventana que se abre escribir los parámetros
en el panel Program arguments.  Por ejemplo: dicc.txt bolsillo "bolsa pequena"

Además cada ejecución de definir modifica el diccionario, por lo que
necesitará restaurarlo con este comando: cp dicc-ini.txt dicc.txt

Recuerde probar igualmente su tarea con make run-san, make run-g y make run.

-----------------------------------------------

Entrega de la tarea

Ejecute: make zip

Entregue por U-cursos el archivo definir.zip

A continuación es muy importante que descargue de U-cursos el mismo
archivo que subió, luego descargue nuevamente los archivos adjuntos y
vuelva a probar la tarea tal cual como la entregó.  Esto es para
evitar que Ud. reciba un 1.0 en su tarea porque entregó los archivos
equivocados.  Créame, sucede a menudo por ahorrarse esta verificación.

-----------------------------------------------

Limpieza de archivos

make clean

Hace limpieza borrando todos los archivos que se pueden volver
a reconstruir a partir de los fuentes: *.o binarios etc.

-----------------------------------------------

Acerca del comando make

El comando make sirve para automatizar el proceso de compilación asegurando
recompilar el archivo binario ejecutable cuando cambió uno de los archivos
fuentes de los cuales depende.

A veces es útil usar make con la opción -n para que solo muestre
exactamente qué comandos va a ejecutar, sin ejecutarlos de verdad.
Por ejemplo:

   make -n ddd

También es útil usar make con la opción -B para forzar la recompilación
de los fuentes a pesar de que no han cambiado desde la última compilación.
Por ejemplo:

   make -B run
