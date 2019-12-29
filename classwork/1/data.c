#include <stdio.h>
#include <omp.h>

int main()
{
    int var = 1;
    printf("Variable before parallel region: %i\n", var);

#pragma omp parallel private(var)
    {
        var = 0;
        printf("Variable at first: %i\n", var);
        var = omp_get_thread_num();
        printf("Variable after `get_thread_num()`: %i\n", var);
    }

    printf("Variable after parallel region: %i\n", var);

    return 0;
}
