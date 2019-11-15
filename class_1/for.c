#include <stdlib.h>
#include <stdio.h>
#include <omp.h>

int main()
{
    size_t size = 10;
    int x;

    int* first = (int*)malloc(sizeof(int) * size);
    int* second = (int*)malloc(sizeof(int) * size);

    for (size_t i = 0; i < size; ++i) {
        first[i] = i * 3;
        second[i] = i * 2;
    }

#pragma omp parallel for lastprivate(x)
    for (size_t i = 0; i < size; ++i) {
        x = first[i] + second[i];
        second[i] = x;

        printf("Mass: %u\n", x);
    }

    printf("Masses after for:\n");
    for (size_t i = 0; i < size; ++i) {
        printf("\t%i\n", second[i]);
    }

    free(first);
    free(second);

    return 0;
}
