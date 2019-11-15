#include <stdio.h>
#include <omp.h>

int main()
{
    size_t id, amount = omp_get_num_threads();
    printf("Defined number of threads: %li\n", amount);
    size_t start = omp_get_wtime();

#pragma omp parallel num_threads(4)
    {
        amount = omp_get_num_threads();
        id = omp_get_thread_num();
        printf("Current number: %li out of %li\n", id, amount);
    }

    size_t end = omp_get_wtime();
    printf("Time elapsed: %zu\n", end - start);

    return 0;
}
