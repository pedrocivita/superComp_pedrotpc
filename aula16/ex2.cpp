#include <mpi.h>
#include <iostream>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    
    int num_iterations = 1000;
    
    if (rank == 0) {
        num_iterations = 5000;
    }
    
    MPI_Bcast(&num_iterations, 1, MPI_INT, 0, MPI_COMM_WORLD);
    
    int result = rank * num_iterations;
    std::cout << "Processo " << rank << " realizou " << num_iterations << " iterações com resultado " << result << std::endl;

    MPI_Finalize();
    return 0;
}
