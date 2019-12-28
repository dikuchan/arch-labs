#include <omp.h>

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

    ull threads = 16;

    matrix* vA = alloc(1, rows),
        * A = alloc(rows, cols),
        * vB = alloc(1, cols);

    fill(vA);
    fill(A);

    /**
     * Data split by rows.
     */

    clock_t start = clock();

    {
        ull i, j;
#pragma omp parallel private(i, j) shared(vA, A, vB) num_threads(threads)
        {
#pragma omp for
            iterate(, i, A->rows) {
                iterate(, j, A->cols) { vB(i) += vA(j) * A(i, j); }
            };
        }
    }

    printf("%f\n", ELAPSED);

    reset(vB);

    /**
     * Data split by columns.
     */

    start = clock();

    {
        ull i, j;
#pragma omp parallel private(i, j) shared(vA, A, vB) num_threads(threads)
        {
            T dot = 0;
            iterate(, i, A->rows) {
#pragma omp for
                iterate(, j, A->cols) { dot += vA(j) * A(i, j); }
#pragma omp critical
                {
                    vB(i) += dot;
                    dot = 0;
                }
            }
        }
    }

    printf("%f\n", ELAPSED);

    reset(vB);

    /**
     * Data split by blocks.
     */

    start = clock();

#pragma omp parallel shared(vA, A, vB) num_threads(threads)
    {
        ull t = omp_get_num_threads(),
            q = t,
            height = A->rows / t,
            width = A->cols / q;

        ull blocks;
#pragma omp for
        iterate(, blocks, t * q) {
            ull i = blocks / q,
                j = blocks % q,
                k = i * height;
            iterate(, k, (i + 1) * height) {
                ull l = j * width;
                iterate(, l, (j + 1) * width) {
                    vB(k) += A(k, l) * vA(l);
                }
            }
        }
    }

    printf("%f\n", ELAPSED);

#if defined(WRITE)
    write(vB, "result.txt");
#endif

    dealloc(vA);
    dealloc(A);
    dealloc(vB);

    return 0;
}
