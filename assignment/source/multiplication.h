#ifndef MULTIPLICATION_H
#define MULTIPLICATION_H

#include <omp.h>
#include <sys/time.h>

#include "matrix.h"

ull mstandard(ull ar, ull ac, // Matrix A rows and cols
            ull br, ull bc,   // Matrix B rows and cols
            ul threads)       // Number of threads
{
    timeval start, end;
    matrix* A = alloc(ar, ac),
          * B = alloc(br, bc),
          * C = alloc(ar, bc); // Result

    fill(A);
    fill(B);

    gettimeofday(&start, NULL);

    /**
     * Simple method
     */

#pragma omp parallel shared(C) num_threads(threads)
    {
        ull i = 0, j = 0, k = 0;
#pragma omp for private(i, j, k)
            iterate(, i, A->rows) {
                iterate(, j, B->cols) {
                    T dot = 0; // Store multiplication result
                    iterate(, k, A->cols) {
                        dot += A(i, k) * B(k, j);
                    }
                    C(i, j) = dot;
                }
            }
    }

    gettimeofday(&end, NULL);

#if defined(WRITE)
    write(C, "product.txt");
    printf("\tResult matrix is written to `product.txt`\n");
#endif

    dealloc(A);
    dealloc(B);
    dealloc(C);

    return ELAPSED;
}

ull mblocks(ull ar, ull ac, ull br, ull bc, ul threads)
{
    timeval start, end;
    matrix* A = alloc(ar, ac),
          * B = alloc(br, bc),
          * C = alloc(ar, bc);

    fill(A);
    fill(B);

    gettimeofday(&start, NULL);

#pragma omp parallel shared(C) num_threads(threads)
    {
        ull lt = omp_get_num_threads(),
            abh = A->rows / lt, // Height of A matrix block
            abw = A->cols / lt, // Width of A matrix block
         // bbh = B->rows / lt, // Height of B matrix block
            bbw = B->cols / lt; // Width of B matrix block
#pragma omp for
        iterate(ull, abc, lt * lt) { // Matrix A block counter
            ull ib = abc / lt, jb = abc % lt; // Block index
            iterate(ull, kb, lt) {
                for (ull i = ib * abh; i < (ib + 1) * abh; ++i) {
                    for (ull j = jb * abw; j < (jb + 1) * abw; ++j) {
                        for (ull k = kb * bbw; k < (kb + 1) * bbw; ++k) {
                            C(i, k) += A(i, j) * B(j, k);
                        }
                    }
                }
            }
        }
    }

    gettimeofday(&end, NULL);

#if defined(WRITE)
    write(C, "product.txt");
#endif

    dealloc(A);
    dealloc(B);
    dealloc(C);

    return ELAPSED;
}

#endif // MULTIPLICATION_H
