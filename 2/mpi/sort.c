#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>

void merge(int*, size_t, size_t);
void sort(int*, size_t);

int main(int argc, char** argv) {
    /*
     * Array size is based on the machine word size.
     */
    size_t n = argc > 1 ? 1 << (atoi(argv[1]))
                        : 1 << (sizeof(size_t) == 8 ? 20 : 14),
        limit = argc > 2 ? 1 << (atoi(argv[2]))
                         : 1 << 8;
    int* array = (int*) malloc(n * sizeof(int));
    struct timeval start, end;

    time_t current_time = time(NULL);
    srand(current_time);
    for (size_t i = 0; i < n; ++i) {
        array[i] = (rand() % (2 * limit)) - limit;
    }

    int rank, tasks_num;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &tasks_num);

    /*
     * Start time measurements.
     */
    gettimeofday(&start, NULL);
    size_t chunk_size = n / tasks_num;
    int* chunk = (int*) malloc(chunk_size * sizeof(int));

    MPI_Scatter(array, chunk_size, MPI_INT, chunk, chunk_size, MPI_INT, 0, MPI_COMM_WORLD);

    sort(chunk, chunk_size);

    int* sorted = NULL;

    if (rank == 0) {
        sorted = (int*) malloc(n * sizeof(int));

        if (n <= 20) {
            printf("Original array:\x20");
            for (size_t i = 0; i < n; ++i) {
                printf("%i\x20", array[i]);
            }
            printf("\n");
        }
    }

    MPI_Gather(chunk, chunk_size, MPI_INT, sorted, chunk_size, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        sort(sorted, n);

        if (n <= 20) {
            printf("Sorted array:\t");
            for (size_t i = 0; i < n; ++i) {
                printf("%i\x20", sorted[i]);
            }
            printf("\n\n");
        }

        free(sorted);
    }

    free(array);
    free(chunk);

    gettimeofday(&end, NULL);

    if (rank == 0) {
        double time = (end.tv_sec - start.tv_sec) * 1e6;
        time = (time + (end.tv_usec - start.tv_usec)) * 1e-6;

        FILE* results = fopen("results.txt", "a");

        fprintf(results, "%s", ctime(&current_time));
        fprintf(results, "\tSize:\x20%i\n", n);
        fprintf(results, "\tTime:\x20%lf\n", time);

        fclose(results);
    }

    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Finalize();

    return 0;
}

void sort(int* a, size_t n) {
    if (n < 2) { return; }

    size_t m = n / 2;
    sort(a, m);
    sort(a + m, n - m);
    merge(a, n, m);
}

void merge(int* a, size_t n, size_t m) {
    int* b = (int*) malloc(n * sizeof(int));

    for (size_t i = 0, j = m, k = 0; k < n; ++k) {
        b[k] = j == n ? a[i++]
                      : i == m ? a[j++]
                               : a[j] < a[i] ? a[j++]
                                             : a[i++];
    }
    for (size_t i = 0; i < n; ++i) {
        a[i] = b[i];
    }

    free(b);
}