#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char** argv)
{
    int task_num, rank, pred, next, buffers[2], tag_1 = 1, tag_2 = 2;
    MPI_Request requests[4];
    MPI_Status statuses[4];

    srand(time(NULL));

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &task_num);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    pred = rank - 1;
    next = rank + 1;
    if (rank == 0) { pred = task_num - 1; }
    if (rank == (task_num - 1)) { next = 0; }

    /*
    * Ring topology.
    * Each device sends and receives messages in both directions without blocking.
    */

    MPI_Irecv(&buffers[0], 1, MPI_INT, pred, tag_1, MPI_COMM_WORLD, &requests[0]);
    MPI_Irecv(&buffers[1], 1, MPI_INT, next, tag_2, MPI_COMM_WORLD, &requests[1]);
    
    int numbers[2] = { rand() % 1024, -(rand() % 1024) };

    MPI_Isend(&numbers[0], 1, MPI_INT, pred, tag_2, MPI_COMM_WORLD, &requests[2]);
    MPI_Isend(&numbers[1], 1, MPI_INT, next, tag_1, MPI_COMM_WORLD, &requests[3]);

    MPI_Waitall(4, requests, statuses);
    
    printf("Device %i received messages %i, %i\n", 
            rank, 
            buffers[0], 
            buffers[1]);
    printf("Device %i sent messages %i, %i\n", 
            rank, 
            numbers[0], 
            numbers[1]);

    MPI_Finalize();
}
