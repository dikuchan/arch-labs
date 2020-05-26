#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char** argv)
{
    int task_num, rank;
    MPI_Status status;

    srand(time(NULL));

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &task_num);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int number = rand() % 1024, results[task_num];

    MPI_Bcast(&number, 1, MPI_INT, 0, MPI_COMM_WORLD);

    printf("Sent %i from device %i\n", 
            number, 
            rank);

    MPI_Allgather(&number, 1, MPI_INT, &results, 1, MPI_INT, MPI_COMM_WORLD);
    
    for (int d = 0; d < task_num; d += 1) {
        printf("Device %i received message %i from %i\n", 
                rank, 
                results[d], 
                d);
    }

    MPI_Finalize();
}
