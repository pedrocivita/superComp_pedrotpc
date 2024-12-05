#include <mpi.h>
#include <iostream>
#include <vector>
#include <omp.h>
#include <chrono>
#include <cstdlib>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    const int N = 1000;
    int num_lines = N / size;
    int remainder = N % size;

    if (rank < remainder) {
        num_lines += 1;
    }

    std::vector<double> matrix;
    if (rank == 0) {
        matrix.resize(N * N);
        srand(42);
        for (int i = 0; i < N * N; ++i) {
            matrix[i] = static_cast<double>(rand()) / RAND_MAX;
        }
    }

    std::vector<int> sendcounts(size);
    std::vector<int> displs(size);
    int offset = 0;
    for (int i = 0; i < size; ++i) {
        int lines = N / size + (i < remainder ? 1 : 0);
        sendcounts[i] = lines * N;
        displs[i] = offset;
        offset += sendcounts[i];
    }

    std::vector<double> local_matrix(num_lines * N);

    MPI_Scatterv(rank == 0 ? matrix.data() : nullptr, sendcounts.data(), displs.data(), MPI_DOUBLE,
                 local_matrix.data(), num_lines * N, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    auto local_start = std::chrono::high_resolution_clock::now();

    std::vector<double> local_means(num_lines);

    #pragma omp parallel for
    for (int i = 0; i < num_lines; ++i) {
        double sum = 0.0;
        for (int j = 0; j < N; ++j) {
            sum += local_matrix[i * N + j];
        }
        local_means[i] = sum / N;
    }

    auto local_end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> local_elapsed = local_end - local_start;
    std::cout << "Rank " << rank << " - Tempo de cálculo local: " << local_elapsed.count() << " segundos." << std::endl;

    std::vector<int> recvcounts(size);
    std::vector<int> recvdispls(size);
    int offset_means = 0;
    for (int i = 0; i < size; ++i) {
        int lines = N / size + (i < remainder ? 1 : 0);
        recvcounts[i] = lines;
        recvdispls[i] = offset_means;
        offset_means += lines;
    }

    std::vector<double> all_means;
    if (rank == 0) {
        all_means.resize(N);
    }

    MPI_Gatherv(local_means.data(), num_lines, MPI_DOUBLE,
                rank == 0 ? all_means.data() : nullptr, recvcounts.data(), recvdispls.data(), MPI_DOUBLE,
                0, MPI_COMM_WORLD);


    if (rank == 0) {

        auto total_start = std::chrono::high_resolution_clock::now();

        double total_sum = 0.0;

        #pragma omp parallel for reduction(+:total_sum)
        for (int i = 0; i < N; ++i) {
            total_sum += all_means[i];
        }

        double global_mean = total_sum / N;

        auto total_end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> total_elapsed = total_end - total_start;

        std::cout << "Média total das linhas: " << global_mean << std::endl;
        std::cout << "Tempo de cálculo total no processo raiz: " << total_elapsed.count() << " segundos." << std::endl;
    }

    MPI_Finalize();
    return 0;
}
