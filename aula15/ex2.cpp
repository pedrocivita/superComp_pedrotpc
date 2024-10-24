#include <mpi.h>
#include <iostream>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        char mensagem[] = "Olá";
        MPI_Send(mensagem, sizeof(mensagem), MPI_CHAR, 1, 0, MPI_COMM_WORLD);
        std::cout << "Processo 0 enviou: " << mensagem << std::endl;
    } else if (rank == 1) {
        char mensagem[10];
        MPI_Recv(mensagem, 10, MPI_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        std::cout << "Processo 1 recebeu: " << mensagem << std::endl;

        char resposta[] = "Oi";
        MPI_Send(resposta, sizeof(resposta), MPI_CHAR, 0, 0, MPI_COMM_WORLD);
        std::cout << "Processo 1 enviou: " << resposta << std::endl;
    } else if (rank == 0) {
        char resposta[10];
        MPI_Recv(resposta, 10, MPI_CHAR, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        std::cout << "Processo 0 recebeu: " << resposta << std::endl;
    }

    MPI_Finalize();
    return 0;
}
