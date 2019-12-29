#ifndef MINMAX_H
#define MINMAX_H

#include <limits.h>
#include <sys/time.h>

#include "matrix.h"

ull minmax(ull r, // Matrix rows
           ull c, // Matrix cols
           ul threads)
{
    timeval start, end;
    matrix* A = alloc(r, c);
    T maximum = MINIMUM;

    fill(A);

    gettimeofday(&start, NULL);

    /**
     * Find maximum value of all rows' minimums
     */

    {
        ull i = 0, j = 0;
#pragma omp parallel for reduction(max: maximum) private(i, j) shared(A) num_threads(threads)
        iterate(, i, A->rows) {
            T minimum = A(i, 0);
#pragma omp parallel for reduction(min: minimum) private(j) shared(A) num_threads(threads)
            iterate(, j, A->cols) {
                if (A(i, j) < minimum) { minimum = A(i, j); }
            }
            if (minimum > maximum) { maximum = minimum; }
        }
    }

    gettimeofday(&end, NULL);

    printf("\tResult: ");
    printf(TOKEN, maximum);

#if defined(WRITE)
    write(A, "minmax.txt");
    printf("\tMatrix is written to `minmax.txt`\n");
#endif

    dealloc(A);

    return ELAPSED;
}

#endif // MINMAX_H
