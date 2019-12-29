#include <omp.h>
#include <sys/time.h>
#include <string.h>

#include "utils.h"
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

    struct timeval start;
    struct timeval end;

    matrix* vA = alloc(1, rows),
        * A = alloc(rows, cols),
        * vB = alloc(1, cols);

    fill(vA);
    fill(A);

    /**
     * Data split by rows.
     */

    gettimeofday(&start, NULL);

    {
        ull i = 0, j = 0;
#pragma omp parallel private(i, j) shared(vB) num_threads(threads)
        {
#pragma omp for
            iterate(, i, A->rows) {
                iterate(, j, A->cols) { vB(i) += vA(j) * A(i, j); }
            }
        }
    }

    gettimeofday(&end, NULL);
    printf("%llu μs\n", ELAPSED);

    reset(vB);

    /**
     * Data split by columns.
     */

    gettimeofday(&start, NULL);

    {
        ull i = 0, j = 0;
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

    gettimeofday(&end, NULL);
    printf("%llu μs\n", ELAPSED);

    reset(vB);

    /**
     * Data split by blocks.
     */

    gettimeofday(&start, NULL);

    {
        ull b = 0, i = 0, j = 0, k = 0, l = 0;
#pragma omp parallel shared(vB) num_threads(threads)
        {
            ull lt = omp_get_num_threads(),
                height = A->rows / lt,
                width = A->cols / lt;
#pragma omp for private(b, i, j, k, l)
            iterate(, b, lt * lt) {
                i = b / lt;
                j = b % lt;
                for (k = i * width; k != (i + 1) * height; ++k) {
                    for (l = j * width; l != (j + 1) * width; ++l) {
                        vB(k) += A(k, l) * vA(l);
                    }
                }
            }
        }
    }

    gettimeofday(&end, NULL);
    printf("%llu μs\n", ELAPSED);

#if defined(WRITE)
    write(vB, "result.txt");
#endif

    dealloc(vA);
    dealloc(A);
    dealloc(vB);

    return 0;
}
