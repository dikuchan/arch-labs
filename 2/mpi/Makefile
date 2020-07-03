EXECS=hello baton shift master each sort
MPICC=mpicc
START=mpirun

all: ${EXECS}

tests: sort
	for i in {1..20}; do ${START} 6 20 20; done
	gawk -f results.awk results.txt

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

sort: sort.c
	${MPICC} -o 6 sort.c

clean:
	seq -s ' ' 6 | xargs rm
	rm *.txt
