#include <iostream>
#include <vector>
#include <mpi.h>
#include <omp.h>

std::vector<int> parallel_search(const std::vector<int>& chunk, int target) {
    std::vector<int> found_positions;
    #pragma omp parallel for
    for (int i = 0; i < chunk.size(); i++) {
        if (chunk[i] == target) {
            #pragma omp critical
            found_positions.push_back(i);
        }
    }
    return found_positions;
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int N = 1000;
    int target = 500;
    std::vector<int> array;
    if (rank == 0) {
        array.resize(N);
        for (int i = 0; i < N; i++) {
            array[i] = (i % 100) + 1;  // Valores entre 1 e 100
        }
    }

    int chunk_size = N / size;
    std::vector<int> local_chunk(chunk_size);

    MPI_Scatter(array.data(), chunk_size, MPI_INT, local_chunk.data(), chunk_size, MPI_INT, 0, MPI_COMM_WORLD);

    std::vector<int> local_positions = parallel_search(local_chunk, target);

    int local_count = local_positions.size();
    std::vector<int> all_counts(size);
    MPI_Gather(&local_count, 1, MPI_INT, all_counts.data(), 1, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        std::cout << "Busca completa." << std::endl;
    }

    MPI_Finalize();
    return 0;
}
