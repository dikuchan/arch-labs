EXECS=vecadd matadd matprod
NVCC=nvcc

all: ${EXECS}

vecadd: vecadd.cu
	${NVCC} -o 1 vecadd.cu

matadd: matadd.cu
	${NVCC} -o 2 matadd.cu

matprod: matprod.cu
	${NVCC} -o 3 matprod.cu

clean:
	seq -s ' ' 3 | xargs rm
