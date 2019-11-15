#include <stdio.h>
#include <omp.h>

int main()
{
    int i;
#pragma omp parallel private(i)
    {
#pragma omp for schedule (static)
// #pragma omp for schedule (static, 1)
// #pragma omp for schedule (static, 5)
// #pragma omp for schedule (dynamic)
// #pragma omp for schedule (dynamic, 1)
// #pragma omp for schedule (dynamic, 3)
// #pragma omp for schedule (guided, 2)
// #pragma omp for schedule (auto)
// #pragma omp for schedule (runtime)
        for (i = 0; i < 10; ++i) {
            printf("Thread %d runs i = %i \n", omp_get_thread_num(), i);
        }
    }
    return 0;
}
