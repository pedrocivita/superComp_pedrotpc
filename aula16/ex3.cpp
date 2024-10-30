#include <mpi.h>
#include <iostream>
#include <vector>
#include <algorithm>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    const int array_size = 100;
    std::vector<float> array;
    std::vector<float> local_array(array_size / size);
    
    float max_value;

    if (rank == 0) {
        array.resize(array_size);
        std::generate(array.begin(), array.end(), []() { return static_cast<float>(rand()) / RAND_MAX; });
        max_value = *std::max_element(array.begin(), array.end());
    }
    
    MPI_Bcast(&max_value, 1, MPI_FLOAT, 0, MPI_COMM_WORLD);
    MPI_Scatter(array.data(), array_size / size, MPI_FLOAT, local_array.data(), array_size / size, MPI_FLOAT, 0, MPI_COMM_WORLD);
    
    for (auto& val : local_array) {
        val /= max_value;
    }
    
    MPI_Gather(local_array.data(), array_size / size, MPI_FLOAT, array.data(), array_size / size, MPI_FLOAT, 0, MPI_COMM_WORLD);
    
    if (rank == 0) {
        std::cout << "Array normalizado: ";
        for (const auto& val : array) {
            std::cout << val << " ";
        }
        std::cout << std::endl;
    }

    MPI_Finalize();
    return 0;
}
