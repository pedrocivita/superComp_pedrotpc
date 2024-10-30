#include <mpi.h>
#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    const int array_size = 100;
    std::vector<int> array;
    std::vector<int> local_array(array_size / size);
    
    if (rank == 0) {
        array.resize(array_size);
        std::generate(array.begin(), array.end(), []() { return rand() % 100; });
    }
    
    MPI_Scatter(array.data(), array_size / size, MPI_INT, local_array.data(), array_size / size, MPI_INT, 0, MPI_COMM_WORLD);
    
    double local_sum = std::accumulate(local_array.begin(), local_array.end(), 0);
    double local_avg = local_sum / local_array.size();
    
    double global_sum;
    MPI_Reduce(&local_avg, &global_sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    
    if (rank == 0) {
        double global_avg = global_sum / size;
        std::cout << "Média global: " << global_avg << std::endl;
    }

    MPI_Finalize();
    return 0;
}
