#ifndef VECTOR_H
#define VECTOR_H

#include <omp.h>
#include <sys/time.h>

#include "matrix.h"

ull vrows(ull r, // Matrix rows
          ull c, // Matrix cols
          ul threads)
{
    timeval start, end;
    matrix* vA = alloc(1, r),
          * A  = alloc(r, c),
          * vB = alloc(1, c); // Result

    fill(vA);
    fill(A);

    /**
     * Data split by rows
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

#ifdef WRITE
    printf("\tResult matrix is written to `vector.txt`\n");
    write(vB, "vector.txt");
#endif

    dealloc(vA);
    dealloc(A);
    dealloc(vB);

    return ELAPSED;
}

ull vcols(ull r, ull c, ul threads)
{
    timeval start, end;
    matrix* vA = alloc(1, r),
          * A  = alloc(r, c),
          * vB = alloc(1, c); // Result

    fill(vA);
    fill(A);

    /**
     * Data split by columns
     */

    gettimeofday(&start, NULL);

    {
        ull i = 0, j = 0;
#pragma omp parallel private(i, j) shared(vB) num_threads(threads)
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

#ifdef WRITE
    write(vB, "vector.txt");
#endif

    dealloc(vA);
    dealloc(A);
    dealloc(vB);

    return ELAPSED;
}

ull vblocks(ull r, ull c, ul threads)
{
    timeval start, end;
    matrix* vA = alloc(1, r),
          * A  = alloc(r, c),
          * vB = alloc(1, c); // Result

    fill(vA);
    fill(A);

    /**
     * Data split by blocks
     */

    gettimeofday(&start, NULL);

    {
        ull b = 0, // Block size
            i = 0, j = 0, k = 0, l = 0;
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

#ifdef WRITE
    write(vB, "vector.txt");
#endif

    dealloc(vA);
    dealloc(A);
    dealloc(vB);

    return ELAPSED;
}

#endif // VECTOR_H
