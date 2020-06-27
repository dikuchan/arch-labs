#include <cuda.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define M 1<<6
#define N 1<<8
#define BLOCK_SIZE 1<<7

__global__
void add(int* A, int* B, int* C) 
{
    size_t column = blockDim.x * blockIdx.x + threadIdx.x;
    size_t row = blockDim.y * blockIdx.y + threadIdx.y;

    if (row < M && column < N) {
        size_t tid = row * N + column;
        C[tid] = A[tid] + B[tid];
    }
}

int main(void)
{
    srand(time(NULL));

    size_t bytes = sizeof(int) * M * N;

    int* A = (int*)malloc(bytes);
    int* B = (int*)malloc(bytes);
    int* C = (int*)malloc(bytes);

    int* dA, *dB, *dC;

    cudaMalloc(&dA, bytes);
    cudaMalloc(&dB, bytes);
    cudaMalloc(&dC, bytes);

    for (size_t i = 0; i < M; ++i) {
        for (size_t j = 0; j < N; ++j) {
            A[i * N + j] = rand() % 1<<10;
            B[i * N + j] = rand() % 1<<10;
        }
    }

    cudaMemcpy(dA, A, bytes, cudaMemcpyHostToDevice);
    cudaMemcpy(dB, B, bytes, cudaMemcpyHostToDevice);

    // Number of CUDA threads per grid block
    dim3 threads_num(BLOCK_SIZE, BLOCK_SIZE, 1);
    // Number of blocks in grid
    dim3 blocks_num(ceil(float(N) / BLOCK_SIZE), ceil(float(M) / BLOCK_SIZE), 1);

    // Apply addition
    add<<<blocks_num, threads_num>>>(dA, dB, dC);

    cudaError_t error_sync = cudaGetLastError();
    cudaError_t error_async = cudaDeviceSynchronize();

    if (error_sync != cudaSuccess) {
        printf(stderr, "Error in configuration: %s", cudaGetErrorString(error_sync));
        goto clean;
    }

    if (error_async != cudaSuccess) {
        printf(stderr, "Error on GPU: %s", cudaGetErrorString(error_async));
        goto clean;
    }

    cudaMemcpy(C, dC, bytes, cudaMemcpyDeviceToHost);

    for (size_t i = 0; i < M; ++i) {
        for (size_t j = 0; j < N; ++j) {
            printf("%i\t", C[i * N + j]);
        }
        printf("\n");
    }

clean:
    free(A);
    free(B);
    free(C);

    cudaFree(dA);
    cudaFree(dB);
    cudaFree(dC);

    return 0;
}
