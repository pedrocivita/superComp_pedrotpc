#include <mpi.h>
#include <iostream>
#include <vector>
#include <numeric>
#include <cmath>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    const int array_size = 100;
    std::vector<double> array;
    std::vector<double> local_array(array_size / size);

    double global_mean;

    if (rank == 0) {
        array.resize(array_size);
        std::generate(array.begin(), array.end(), []() { return static_cast<double>(rand()) / RAND_MAX; });
        global_mean = std::accumulate(array.begin(), array.end(), 0.0) / array.size();
    }

    MPI_Bcast(&global_mean, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Scatter(array.data(), array_size / size, MPI_DOUBLE, local_array.data(), array_size / size, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    
    double local_sum_square_diff = 0.0;
    for (const auto& val : local_array) {
        local_sum_square_diff += (val - global_mean) * (val - global_mean);
    }

    double global_sum_square_diff;
    MPI_Reduce(&local_sum_square_diff, &global_sum_square_diff, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    
    if (rank == 0) {
        double stddev = std::sqrt(global_sum_square_diff / array_size);
        std::cout << "Desvio padrão global: " << stddev << std::endl;
    }

    MPI_Finalize();
    return 0;
}
