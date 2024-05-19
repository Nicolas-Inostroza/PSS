# Para revisar las opciones de compilacion y ejecucion,
# ingrese en el terminal el comando: make
#
#

PROB=suma

SRCS= test-$(PROB).c $(PROB).c
HDRS= $(PROB).h

SHELL=bash -o pipefail

INCLUDE=
CFLAGS=-Wall -Werror -pedantic -std=c18 -Wno-unused-function $(INCLUDE)
LDLIBS=-lm
ARCH=$(shell arch)

MAK=make --no-print-directory

readme:
	@less README.txt

$(PROB).bin $(PROB).bin-g $(PROB).bin-san: $(SRCS) $(HDRS)

run-san: $(PROB).bin-san
	@if grep -P '\t' $(PROB).c ; then echo "Su archivo $(PROB).c contiene tabs.  Reemplacelos por espacios en blanco!" ; false ; else true; fi
	./$(PROB).bin-san

run-g: $(PROB).bin-g
	./$(PROB).bin-g

prof.ref: $(PROB).bin
	cp $(PROB).bin prof.ref-$(ARCH)
	strip prof.ref-$(ARCH)

run: $(PROB).bin
	@echo "Midiendo cuanto demora la solucion del profesor"
	rm -f tiempo-ref.txt
	chmod +x prof.ref-$(ARCH)
	./prof.ref-$(ARCH) ref > /dev/null
	@echo "Tiempos de referencia = " `cat tiempo-ref.txt`
	@echo "Verificando y comparando con su solucion"
	@echo ./$(PROB).bin `cat tiempo-ref.txt`
	@./$(PROB).bin `cat tiempo-ref.txt`

ddd: $(PROB).ddd

ddd-san: $(PROB).ddd-san

zip:
	@if grep -P '\t' $(PROB).c ; then echo "Su archivo $(PROB).c contiene tabs.  Reemplacelos por espacios en blanco!" ; false ; else true; fi
	@rm -f resultados.txt $(PROB).zip
	@echo "Sistema operativo utilizado" > resultados.txt
	@uname -a >> resultados.txt
	@cat resultados.txt
	@echo ==== run-san ==== | tee -a resultados.txt
	@$(MAK) -B run-san | tee -a resultados.txt
	@echo ==== run-g ==== | tee -a resultados.txt
	@$(MAK) -B run-g | tee -a resultados.txt
	@echo ==== run ==== | tee -a resultados.txt
	@$(MAK) -B run | tee -a resultados.txt
	@echo ==== zip ====
	zip -r $(PROB).zip resultados.txt $(PROB).c
	@echo "Entregue por u-cursos el archivo $(PROB).zip"
	@echo "Descargue de u-cursos lo que entrego, descomprimalo y revise"
	@echo "que esta su ultima version de $(PROB).c"

%.bin: %.c
	gcc -O -DOPT=1 $(CFLAGS) $(SRCS) $(LDLIBS) -o $@

%.bin-g: %.c
	gcc -g $(CFLAGS) $(SRCS) $(LDLIBS) -o $@

%.bin-san: %.c
	gcc -g -DSAN=1 -fsanitize=address -fsanitize=undefined -DSANITIZE $(CFLAGS) $(SRCS) $(LDLIBS) -o $@

%.ddd: %.bin-g
	ddd $(*F).bin-g &

%.ddd-san: %.bin-san
	ddd $(*F).bin-san &

clean:
	rm -f *.o *.log *.bin* core resultados.txt $(PROB).zip
	rm -rf bin obj
