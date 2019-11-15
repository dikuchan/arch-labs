#include "matrix.h"

int main()
{
#if defined(INLINE)
    matrix *A = alloc(300, 200),
           *B = alloc(200, 300),
           *C = alloc(300, 300);
#else
    char input[20], *pointer;
    ull rowsA, colsA,
        rowsB, colsB;

    readnum(rowsA);
    readnum(colsA);
    matrix* A = alloc(rowsA, colsA);

    readnum(rowsB);
    readnum(colsB);
    matrix* B = alloc(rowsB, colsB);
    matrix* C = alloc(rowsA, colsB);
#endif

    fill(A);
    fill(B);

    clock_t start = clock();

    {
        ull i, j, k;
#pragma omp parallel for private(i, j, k) shared(A, B, C)
        iterate(, i, A->rows) {
            iterate(, j, B->cols) {
                T dot = 0;
                iterate(, k, A->cols)
                    { dot += A(i, k) * B(k, j); }
                C(i, j) = dot;
            }
        }
    }

    clock_t parallel = clock();

    {
        iterate(ull, i, A->rows) {
            iterate(ull, j, B->cols) {
                T dot = 0;
                iterate(ull, k, A->cols)
                    { dot += A(i, k) * B(k, j); }
                C(i, j) = dot;
            }
        }
    }

    clock_t end = clock();

    printf("%f\n", (double)(parallel - start) / CLOCKS_PER_SEC);
    printf("%f\n",   (double)(end - parallel) / CLOCKS_PER_SEC);

#if defined(WRITE)
    write(C, "result.txt");
#endif

    dealloc(A);
    dealloc(B);
    dealloc(C);
}
