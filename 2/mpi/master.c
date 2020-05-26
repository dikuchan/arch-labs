#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char** argv)
{
    int task_num, rank;

    srand(time(NULL));

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &task_num);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int slave_num = task_num - 1, results[slave_num];
    MPI_Request requests[slave_num];
    MPI_Status master_status, statuses[slave_num];

    if (rank == 0) {
        for (int s = 0; s < slave_num; s += 1) {
            int number = rand() % 1024;
            MPI_Isend(&number, 1, MPI_INT, s + 1, s, MPI_COMM_WORLD, &requests[s]);
            printf("Message %i sent from master to slave %i\n", 
                    number, 
                    s);
        }

        MPI_Waitall(slave_num, requests, statuses);

        for (int s = 0; s < slave_num; s += 1) {
            MPI_Irecv(&results[s], 1, MPI_INT, s + 1, MPI_ANY_TAG, MPI_COMM_WORLD, &requests[s]);
        }

        MPI_Waitall(slave_num, requests, statuses);
        
        for (int s = 0; s < slave_num; s += 1) {
            printf("Received number from slave %i is %s\n",
                    statuses[s].MPI_TAG,
                    results[s] ? "odd" : "even");
        }
    } 
    else {
        int buffer;
        MPI_Recv(&buffer, 1, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &master_status);
        buffer &= 1;
        MPI_Send(&buffer, 1, MPI_INT, 0, rank - 1, MPI_COMM_WORLD);
    }

    MPI_Finalize();
}
