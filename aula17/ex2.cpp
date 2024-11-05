#include <iostream>
#include <vector>
#include <mpi.h>
#include <omp.h>

void calculate_square(std::vector<int>& matrix_chunk) {
    #pragma omp parallel for
    for (size_t i = 0; i < matrix_chunk.size(); i++) {
        matrix_chunk[i] *= matrix_chunk[i];
    }
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int N = 103;  // Exemplo de valor que não é múltiplo do número de processos
    int chunk_size = (N * N) / size;
    int remainder = (N * N) % size;

    std::vector<int> matrix;
    if (rank == 0) {
        matrix.resize(N * N);
        for (int i = 0; i < N * N; i++) {
            matrix[i] = i + 1;
        }
    }

    std::vector<int> matrix_chunk(chunk_size + (rank < remainder ? 1 : 0));

    std::vector<int> send_counts(size, chunk_size);
    std::vector<int> displacements(size, 0);
    for (int i = 0; i < remainder; i++) {
        send_counts[i]++;
    }
    for (int i = 1; i < size; i++) {
        displacements[i] = displacements[i - 1] + send_counts[i - 1];
    }

    MPI_Scatterv(matrix.data(), send_counts.data(), displacements.data(), MPI_INT, matrix_chunk.data(), matrix_chunk.size(), MPI_INT, 0, MPI_COMM_WORLD);

    calculate_square(matrix_chunk);

    MPI_Gatherv(matrix_chunk.data(), matrix_chunk.size(), MPI_INT, matrix.data(), send_counts.data(), displacements.data(), MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        std::cout << "Cálculo completo com MPI_Scatterv." << std::endl;
    }

    MPI_Finalize();
    return 0;
}
