#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv)
{
    int process_num, number, name_size;
    char name[MPI_MAX_PROCESSOR_NAME];

    MPI_Init(&argc, &argv);
    
    MPI_Comm_size(MPI_COMM_WORLD, &process_num);
    MPI_Comm_rank(MPI_COMM_WORLD, &number);
    MPI_Get_processor_name(name, &name_size);

    printf("Device %i (%s) out of %i\n", 
            number + 1,
            name,
            process_num);

    MPI_Finalize();
}
