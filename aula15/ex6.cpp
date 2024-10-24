#include <mpi.h>
#include <iostream>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) {
        for (int i = 1; i < size; ++i) {
            int mensagem = i * 10;
            MPI_Send(&mensagem, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
            MPI_Recv(&mensagem, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            std::cout << "Processo 0 recebeu resposta de " << i << ": " << mensagem << std::endl;
        }
    } else {
        int mensagem;
        MPI_Recv(&mensagem, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        mensagem *= 2;  // Dobrar o valor para simular uma resposta
        MPI_Send(&mensagem, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
}
