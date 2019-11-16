#include "matrix.h"

int main()
{
    ull rows, cols;
#if defined(INLINE)
    rows = cols = 5000;
#else
    rows = readnum();
    cols = readnum();
#endif

    matrix *vA = alloc(1, rows),
            *A = alloc(rows, cols),
            *B = alloc(1, cols);

    fill(vA);
    fill(A);

    /**
     * Data split by rows.
     */

    clock_t start = clock();

    {
#pragma omp parallel shared(vA, A, B)
        {
            ull i, j;
#pragma omp for
            iterate(, i, A->rows) {
                iterate(, j, A->cols)
                { B(0, i) += vA(j) * A(i, j); }
            };
        }
    }

    printf("%f\n", ELAPSED);

    reset(B);

    /**
     * Data split by columns.
     */

    start = clock();

    {
#pragma omp parallel shared(vA, A, B)
        {
            T dot = 0;
            ull i, j;
            iterate(, i, A->rows) {
#pragma omp for
                iterate(, j, A->cols)
                { dot += vA(j) * A(i, j); }
#pragma omp critical
                {
                    B(0, i) += dot;
                    dot = 0;
                }
            }
        }
    }

    printf("%f\n", ELAPSED);

    /**
     * // TODO
     * Data split by blocks.
     */

#if defined(WRITE)
    write(B, "result.txt");
#endif

    dealloc(vA);
    dealloc(A);
    dealloc(B);

    return 0;
}
