#include "matrix.h"

int main()
{
    ull rowsA, colsA,
        rowsB, colsB;
#if defined(INLINE)
    rowsA = colsB = 300;
    colsA = rowsB = 200;
#else
    rowsA = readnum();
    colsA = readnum();
    rowsB = readnum();
    colsB = readnum();
#endif

    matrix *A = alloc(rowsA, colsA),
           *B = alloc(rowsB, colsB),
           *C = alloc(rowsA, colsB);

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

    printf("%f\n", ELAPSED);
    start = clock();

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

    printf("%f\n", ELAPSED);

#if defined(WRITE)
    write(C, "result.txt");
#endif

    dealloc(A);
    dealloc(B);
    dealloc(C);
}
