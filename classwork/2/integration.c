#include <stdio.h>
#include <omp.h>

#define N 10000

double f(double x) { return 4. / (1. + x * x); }

int main() {
    double a = 0., b = 1.,
           h = (b - a) / N, sum = 0.;

#pragma omp parallel for reduction(+:sum)
    for (int i = 1; i < N; ++i) { sum += f(a + i * h); }

    printf("%f\n", (sum + 3.) * h);

    return 0;
}
