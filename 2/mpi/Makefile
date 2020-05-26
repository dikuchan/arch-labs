EXECS=hello baton shift master each
MPICC=mpicc

all: ${EXECS}

hello: hello.c
	${MPICC} -o 1 hello.c

baton: baton.c
	${MPICC} -o 2 baton.c

shift: shift.c
	${MPICC} -o 3 shift.c

master: master.c
	${MPICC} -o 4 master.c

each: each.c
	${MPICC} -o 5 each.c

clean:
	seq -s ' ' 5 | xargs rm
