#include <iostream>
#include <vector>
#include <numeric>
#include <mpi.h>
#include <omp.h>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int N = 1000;  // Tamanho do vetor
    std::vector<int> array;
    if (rank == 0) {
        array.resize(N);
        for (int i = 0; i < N; i++) {
            array[i] = i + 1;
        }
    }

    int chunk_size = N / size;
    std::vector<int> local_chunk(chunk_size);

    MPI_Scatter(array.data(), chunk_size, MPI_INT, local_chunk.data(), chunk_size, MPI_INT, 0, MPI_COMM_WORLD);

    int local_sum = 0;
    #pragma omp parallel for reduction(+:local_sum)
    for (int i = 0; i < local_chunk.size(); i++) {
        local_sum += local_chunk[i];
    }

    int global_sum = 0;
    MPI_Reduce(&local_sum, &global_sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        float average = static_cast<float>(global_sum) / N;
        std::cout << "Média calculada: " << average << std::endl;
    }

    MPI_Finalize();
    return 0;
}
