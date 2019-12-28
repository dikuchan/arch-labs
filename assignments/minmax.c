#include <limits.h>
#include <sys/time.h>

#include "matrix.h"

int main()
{
    ull rows, cols;
#if defined(INLINE)
    rows = cols = 1000;
#else
    rows = readnum(stdin, "Number of rows: ");
    cols = readnum(stdin, "Number of cols: ");
#endif

    struct timeval start;
    struct timeval end;

    matrix* A = alloc(rows, cols);
    fill(A);

    gettimeofday(&start, NULL);

    {
        ull i = 0, j = 0;
        T maximum = MINIMUM;
#pragma omp parallel for reduction(max: maximum) private(i, j) shared(A)
        iterate(, i, A->rows) {
            T minimum = A(i, 0);
#pragma omp parallel for reduction(min: minimum) private(j) shared(A)
            iterate(, j, A->cols) {
                if (A(i, j) < minimum) { minimum = A(i, j); }
            }
            if (minimum > maximum) { maximum = minimum; }
        }

        printf(TOKEN, maximum);
    }

    gettimeofday(&end, NULL);
    printf("%llu μs\n", ELAPSED);

#if defined(WRITE)
    write(A, "result.txt");
#endif

    dealloc(A);

    return 0;
}
