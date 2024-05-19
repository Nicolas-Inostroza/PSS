# Si desea agregar un programa con archivos test-ejemplo.c, ejemplo.c y
# ejemplo.h, agregue un par de reglas que digan:
# test-ejemplo.o ejemplo.o: ejemplo.h
# test-ejemplo: test-ejemplo.o ejemplo.o

PROB=wc

RISCV = /opt/riscv
TARGET = riscv64-unknown-elf
CC = ${RISCV}/bin/${TARGET}-gcc
CXX = ${RISCV}/bin/${TARGET}-g++
AS = ${RISCV}/bin/${TARGET}-as
LD = ${RISCV}/bin/${TARGET}-ld
STRIP = ${RISCV}/bin/${TARGET}-strip
GDB = ${RISCV}/bin/${TARGET}-gdb
PK = ${RISCV}/${TARGET}/bin/pk
DUMP= ${RISCV}/${TARGET}/bin/objdump
QEMU32=qemu-riscv32

SHELL=bash -o pipefail
MAK=make --no-print-directory

COMFLAGS= -Wall -pedantic -std=c2x -march=rv32im -mabi=ilp32

CFLAGS= -g ${COMFLAGS}
LDFLAGS= -g
LDLIBS=

readme:
	@less README.txt

sort-rv-$(PROB): test-sort.o sort-rv-$(PROB).o

sort-c-$(PROB): test-sort.o sort-c-$(PROB).o

sort-rv: test-sort.o sort-rv.o

sort-c: test-sort.o sort-c.o

zip:
	@if grep -P '\t' sort-c-$(PROB).c ; then echo "Su archivo sort-c-$(PROB).c contiene tabs.  Use el comando expand para reemplazarlos por espacios en blanco!" ; false ; else true; fi
	@rm -f resultados.txt $(PROB).zip
	@echo "Sistema operativo utilizado" > resultados.txt
	@uname -a >> resultados.txt
	@cat resultados.txt
	@echo ==== make sort-c-$(PROB).run ==== | tee -a resultados.txt
	@$(MAK) -B sort-c-$(PROB).run | tee -a resultados.txt
	@echo ==== make sort-rv-$(PROB).run ==== | tee -a resultados.txt
	@$(MAK) -B sort-rv-$(PROB).run | tee -a resultados.txt
	@echo ==== zip ====
	zip -r $(PROB).zip resultados.txt sort-c-$(PROB).c sort-rv-$(PROB).s
	@echo "Entregue por u-cursos el archivo $(PROB).zip"
	@echo "Descargue de u-cursos lo que entrego, descargue nuevamente los"
	@echo "archivos adjuntos y vuelva a probar la tarea tal cual como"
	@echo "la entrego.  Esto es para evitar que Ud. reciba un 1.0 en su"
	@echo "tarea porque entrego los archivos equivocados.  Creame, sucede"
	@echo "a menudo por ahorrarse esta verificacion."


clean:
	rm -f *.o $(PROB) sort-rv sort-rv-$(PROB) sort-c sort-c-$(PROB) core \
                  resultados.txt $(PROB).zip

%.o: %.c
	${CC} -g -c ${COMFLAGS} $< -o $@

%.s: %.c
	${CC} -O -S ${COMFLAGS} $<

%.ddd: %
	qemu-riscv32 -g 1234 $(*F) &
	ddd --debugger ${GDB} --command target.gdb $(*F)

%.o: %.s
	${CC} -c ${COMFLAGS} $< -o $@

%: %.o
	${CC} ${COMFLAGS} ${LDFLAGS} $^ ${LDLIBS} -o $@

%.run: %
	qemu-riscv32 $(*F)

%.dump: %
	${DUMP} -S $(*F) | less
