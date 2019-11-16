#include <limits.h>

#include "matrix.h"

int main()
{
    ull rows, cols;
#if defined(INLINE)
    rows = cols = 100;
#else
    rows = readnum();
    cols = readnum();
#endif

    matrix* A = alloc(rows, cols);
    fill(A);

    T minimum = MAXIMUM, maximum = MINIMUM;

    clock_t start = clock();

    {
        ull i, j;
#pragma omp parallel for private(i, j) shared(A)
        iterate(, i, A->rows) {
            iterate(, j, A->cols) {
                if (A(i, j) < minimum)
                { minimum = A(i, j); }
            }
            if (minimum > maximum)
            { maximum = minimum; };
            minimum = MAXIMUM;
        }
    }

    printf("%f\n", ELAPSED);

#if defined(WRITE)
    write(A, "result.txt");
#endif
    printf(TOKEN, maximum);

    dealloc(A);

    return 0;
}
