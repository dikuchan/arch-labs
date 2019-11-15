#include <stdio.h>
#include <omp.h>

int main()
{
    int n;
#pragma omp parallel private(n)
    {
        n = omp_get_thread_num();
        printf("Before critical %i \n", n);
#pragma omp critical
        {
            printf("Thread %i is working \n", n);
        };
        printf("After critical %i \n", n);
    }
    return 0;
}