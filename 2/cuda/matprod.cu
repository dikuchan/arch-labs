#include <cuda.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define N 1<<10
#define BLOCK_SIZE 1<<7

__global__
void multiply(int* A, int* B, int* C)
{
    int result = 0;

    size_t i, j;

    __shared__ int sA[BLOCK_SIZE][BLOCK_SIZE];
    __shared__ int sB[BLOCK_SIZE][BLOCK_SIZE];

    // Column of matrix A
    size_t column = blockIdx.x * blockDim.x + threadIdx.x;
    // Row of matrix B
    size_t row = blockIdx.y * blockDim.y + threadIdx.y;

    for (size_t tile = 0; tile < gridDim.x; ++tile) {
        // Column of matrix B
        i = tile * BLOCK_SIZE + threadIdx.y;
        // Row of matrix A
        j = tile * BLOCK_SIZE + threadIdx.x;

        sA[threadIdx.y][threadIdx.x] = A[row * N + j];
        sB[threadIdx.y][threadIdx.x] = B[i * N + column];

        __syncthreads();

        for (size_t k = 0; k < BLOCK_SIZE; ++k) {
            temp += sA[threadIdx.y][k] * sB[k][threadIdx.x];
        }

        __syncthreads();
    }

    C[row * N + column] = result;
}

int main(void)
{
    srand(time(NULL));

    size_t bytes;

    int* A = (int*)malloc(bytes);
    int* B = (int*)malloc(bytes);
    int* C = (int*)malloc(bytes);

    int* dA, *dB, *dC;

    cudaMalloc(&dA, bytes);
    cudaMalloc(&dB, bytes);
    cudaMalloc(&dC, bytes);

    for (size_t i = 0; i < N; ++i) {
        for (size_t j = 0; j < N; ++i) {
            A[i * N + j] = rand() % 1<<10;
            B[i * N + j] = rand() % 1<<10;
        }
    }

    cudaMemcpy(dA, A, bytes, cudaMemcpyHostToDevice);
    cudaMemcpy(dB, B, bytes, cudaMemcpyHostToDevice);

    dim3 threads_num(BLOCK_SIZE, BLOCK_SIZE);
    dim3 blocks_num(ceil(float(N) / BLOCK_SIZE), ceil(float(M) / BLOCK_SIZE), 1);

    multiply<<<blocks_num, threads_num>>>(dA, dB, dC);

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

    for (size_t i = 0; i < N; ++i) {
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
