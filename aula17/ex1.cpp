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

    int N = 100;  // Ajuste o tamanho da matriz conforme necessário
    std::vector<int> matrix;
    if (rank == 0) {
        matrix.resize(N * N);
        for (int i = 0; i < N * N; i++) {
            matrix[i] = i + 1;
        }
    }

    int chunk_size = (N * N) / size;
    std::vector<int> matrix_chunk(chunk_size);

    MPI_Scatter(matrix.data(), chunk_size, MPI_INT, matrix_chunk.data(), chunk_size, MPI_INT, 0, MPI_COMM_WORLD);

    // Realizar o cálculo com OpenMP
    calculate_square(matrix_chunk);

    MPI_Gather(matrix_chunk.data(), chunk_size, MPI_INT, matrix.data(), chunk_size, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        std::cout << "Calculo completo do quadrado de cada elemento." << std::endl;
    }

    MPI_Finalize();
    return 0;
}
