#include <omp.h>
#include <sys/time.h>

#include "matrix.h"

int main()
{
    ull rowsA, colsA,
        rowsB, colsB;
#if defined(INLINE)
    rowsA = colsB = 700;
    colsA = rowsB = 700;
#else
    rowsA = readnum(stdin, "First matrix rows: ");
    colsA = readnum(stdin, "First matrix cols: ");
    rowsB = readnum(stdin, "Second matrix rows: ");
    colsB = readnum(stdin, "Second matrix cols: ");
#endif

    struct timeval start;
    struct timeval end;

    matrix* A = alloc(rowsA, colsA),
        * B = alloc(rowsB, colsB),
        * C = alloc(rowsA, colsB);

    fill(A);
    fill(B);

    gettimeofday(&start, NULL);

    {
        ull i = 0, j = 0, k = 0;
#pragma omp parallel for private(i, j, k) shared(C)
            iterate(, i, A->rows) {
                iterate(, j, B->cols) {
                    T dot = 0;
                    iterate(, k, A->cols) {
                        dot += A(i, k) * B(k, j);
                    }
                    C(i, j) = dot;
                }
            }
    }

    gettimeofday(&end, NULL);
    printf("%llu μs\n", ELAPSED);
    gettimeofday(&start, NULL);

    {
        iterate(ull, i, A->rows) {
            iterate(ull, j, B->cols) {
                T dot = 0;
                iterate(ull, k, A->cols) {
                    dot += A(i, k) * B(k, j);
                }
                C(i, j) = dot;
            }
        }
    }

    gettimeofday(&end, NULL);
    printf("%llu μs\n", ELAPSED);

#if defined(WRITE)
    write(C, "result.txt");
#endif

    dealloc(A);
    dealloc(B);
    dealloc(C);
}
