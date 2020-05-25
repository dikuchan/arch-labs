EXECS=main
DIRS=-Isource -Iinclude
LIBS=-fopenmp
DEFS=-DWRITE -DINLINE

all: ${EXECS}

main: 1.c
	gcc main.c -o 1 ${DEFS} ${DIRS} ${LIBS}

clean:
	rm {EXECS}
