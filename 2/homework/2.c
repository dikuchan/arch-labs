#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv)
{
    int processes, number, nlength;
    char name[MPI_MAX_PROCESSOR_NAME];

    MPI_Init(&argc, &argv);
    
    MPI_Comm_size(MPI_COMM_WORLD, &processes);
    MPI_Comm_rank(MPI_COMM_WORLD, &number);
    MPI_Get_processor_name(name, &nlength);

    printf("Processor %i (%s) out of %i\n", 
            number, 
            name, 
            processes);

    MPI_Finalize();
}
