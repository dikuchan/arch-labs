#include "matrix.h"

int main()
{
    ull rows, cols;
#if defined(INLINE)
    rows = cols = 1000;
#else
    rows = readnum();
    cols = readnum();
#endif

    matrix *vA = alloc(1, rows),
            *A = alloc(rows, cols),
            *B = alloc(1, cols);

    fill(vA);
    fill(A);

    clock_t start = clock();

    {
#pragma omp parallel
        {
            ull i, j;
            matrix* vB = alloc(1, A->rows);
            iterate(, i, A->rows) {
#pragma omp for
                iterate(, j, A->cols)
                { vB(i) += vA(j) * A(i, j); }
            }
#pragma omp critical
            {
                iterate(, i, A->cols)
                { B(0, i) = vB(i); }
            };
            dealloc(vB);
        };
    }

    printf("%f\n", ELAPSED);

#if defined(WRITE)
    write(B, "result.txt");
#endif

    dealloc(vA);
    dealloc(A);
    dealloc(B);

    return 0;
}
