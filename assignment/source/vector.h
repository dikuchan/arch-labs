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
        * A = alloc(r, c),
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
        * A = alloc(r, c),
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
        * A = alloc(r, c),
        * vB = alloc(1, c); // Result

    fill(vA);
    fill(A);

    /**
     * Data split by blocks
     */

    gettimeofday(&start, NULL);

#pragma omp parallel shared(vB) num_threads(threads)
    {
        ull lt = omp_get_num_threads(),
            bv = lt, // Vertical blocks
            bh = lt; // Horizontal blocks
#pragma omp for collapse(2)
        iterate(ull, iv, lt) {
            iterate(ull, ih, lt) {
                for (ull i = iv * A->rows / bv; i < (iv + 1) * A->rows / bv; ++i) {
                    for (ull j = ih * A->cols / bh; j < (ih + 1) * A->cols / bh; ++j) {
#pragma omp atomic
                        vB(i) += A(i, j) * vA(i);
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
