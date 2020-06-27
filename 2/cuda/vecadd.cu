#include <cuda.h>
#include <stdio.h>
#include <math.h>

#define N 1<<20
#define BLOCK_SIZE 1<<7

__global__
void add(int* a, int* b, int* c) 
{
    size_t index = blockIdx.x * blockDim.x + threadIdx.x;
    size_t stride = blockDim.x * gridDim.x;

    for (size_t i = index; i < N; i += stride) {
        c[i] = a[i] + b[i];
    }
}

int main(void)
{
    int *a, *b, *c;

    cudaMallocManaged(&a, sizeof(int) * N);
    cudaMallocManaged(&b, sizeof(int) * N);
    cudaMallocManaged(&c, sizeof(int) * N);

    for (size_t i = 0; i < N; ++i) {
        a[i] = -i;
        b[i] = i * i;
    }

    size_t threads_num = BLOCK_SIZE;
    // May overflow
    size_t blocks_num = (N + threads_num - 1) / threads_num;

    add<<<blocks_num, threads_num>>>(a, b, c);

    cudaDeviceSynchronize();

    for (size_t i = 0; i < N; ++i) {
        printf("%d + %d = %d\n", a[i], b[i], c[i]);
    }

    cudaFree(a);
    cudaFree(b);
    cudaFree(c);

    return 0;
}
