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

#ifdef WRITE
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

    /**
     * Block method
     */

#pragma omp parallel shared(C) num_threads(threads)
    {
        ull lt = threads,
            iv = 0, ih = 0;
#pragma omp for schedule(static) collapse(2)
        iterate(, iv, lt) { // Vertical block index
            iterate(, ih, lt) { // Horizontal block index
                for (ull i = iv * A->rows / lt; i < (iv + 1) * A->rows / lt; ++i) {
                    for (ull j = ih * A->cols / lt; j < (ih + 1) * A->cols / lt; ++j) {
                        iterate(ull, k, A->cols) {
                            C(i, j) += A(i, k) * B(k, j);
                        }
                    }
                }
            }
        }
    }

    gettimeofday(&end, NULL);

#ifdef WRITE
    write(C, "product.txt");
#endif

    dealloc(A);
    dealloc(B);
    dealloc(C);

    return ELAPSED;
}

ull mcheckerboard(ull ar, ull ac, ull br, ull bc, ul threads)
{
    timeval start, end;
    matrix* A = alloc(ar, ac),
          * B = alloc(br, bc),
          * C = alloc(ar, bc);

    fill(A);
    fill(B);

    gettimeofday(&start, NULL);

    /**
     * Checkerboard method
     */

#pragma omp parallel shared(C)
    {
        ull bv = threads, // Vertical blocks
            bh = threads, // Horizontal blocks
            bk = threads, // K blocks
            iv = 0, ih = 0, ik = 0; // Indices
#pragma omp for collapse(2)
        iterate(, iv, bv) {
            iterate(, ih, bh) {
                iterate(, ik, bk) {
                    for (ull i = iv * A->rows / bv; i < (iv + 1) * A->rows / bv; ++i) {
                        for (ull j = ih * B->cols / bh; j < (ih + 1) * B->cols / bh; ++j) {
                            for (ull k = ik * A->cols / bk; k < (ik + 1) * A->cols / bk; ++k) {
#pragma omp atomic
                                C(i, j) += A(i, k) * B(k, j);
                            }
                        }
                    }
                }
            }
        }
    }

    gettimeofday(&end, NULL);

#ifdef WRITE
    write(C, "product.txt");
#endif

    dealloc(A);
    dealloc(B);
    dealloc(C);

    return ELAPSED;
}


#endif // MULTIPLICATION_H
