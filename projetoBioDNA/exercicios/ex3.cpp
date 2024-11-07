#include <mpi.h>
#include <omp.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

// Função para converter caracteres para maiúsculas
char to_upper(char c) {
    if (c >= 'a' && c <= 'z')
        return c - 32;
    return c;
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    double start_time = MPI_Wtime();  // Início da medição do tempo

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Lista de cromossomos (1 a 22)
    std::vector<int> cromossomos;
    for (int i = 1; i <= 22; ++i) {
        cromossomos.push_back(i);
    }

    // Distribuição dos cromossomos entre os processos
    std::vector<int> cromossomos_local;
    for (size_t i = rank; i < cromossomos.size(); i += size) {
        cromossomos_local.push_back(cromossomos[i]);
    }

    long long contagem_local = 0;

    // Processamento dos arquivos RNA atribuídos ao processo
    for (int crom : cromossomos_local) {
        std::string filename = "chr" + std::to_string(crom) + "_rna.fa";
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Processo " << rank << ": erro ao abrir o arquivo " << filename << std::endl;
            continue;
        }

        std::string line;
        std::string sequence;

        while (std::getline(file, line)) {
            if (line.empty() || line[0] == '>') continue; // Ignora linhas de descrição
            sequence += line;
        }
        file.close();

        // Busca por 'AUG' utilizando OpenMP
        size_t seq_size = sequence.size();
        #pragma omp parallel for reduction(+:contagem_local)
        for (size_t i = 0; i <= seq_size - 3; ++i) {
            char c1 = to_upper(sequence[i]);
            char c2 = to_upper(sequence[i + 1]);
            char c3 = to_upper(sequence[i + 2]);
            if (c1 == 'A' && c2 == 'U' && c3 == 'G') {
                contagem_local++;
            }
        }
    }

    // Redução das contagens dos processos
    long long contagem_global = 0;
    MPI_Reduce(&contagem_local, &contagem_global, 1, MPI_LONG_LONG, MPI_SUM, 0, MPI_COMM_WORLD);

    double end_time = MPI_Wtime();  // Fim da medição do tempo
    double total_time = end_time - start_time;

    if (rank == 0) {
        std::cout << "Total de proteínas inicializadas (AUG): " << contagem_global << std::endl;
        std::cout << "Tempo total de execução: " << total_time << " segundos." << std::endl;
    }

    MPI_Finalize();
    return 0;
}
