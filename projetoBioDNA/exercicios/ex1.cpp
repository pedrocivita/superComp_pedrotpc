#include <mpi.h>
#include <omp.h>
#include <iostream>
#include <fstream>
#include <string>
#include <map>
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

    std::map<char, long long> contagem_local;
    contagem_local['A'] = 0;
    contagem_local['T'] = 0;
    contagem_local['C'] = 0;
    contagem_local['G'] = 0;

    // Processamento dos arquivos atribuídos ao processo
    for (int crom : cromossomos_local) {
        std::string filename = "chr" + std::to_string(crom) + ".subst.fa";
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Processo " << rank << ": erro ao abrir o arquivo " << filename << std::endl;
            continue;
        }

        std::string line;
        std::vector<std::string> sequences;

        while (std::getline(file, line)) {
            if (line.empty() || line[0] == '>') continue; // Ignora linhas de descrição
            sequences.push_back(line);
        }
        file.close();

        // Contagem das bases utilizando OpenMP
        #pragma omp parallel
        {
            std::map<char, long long> contagem_thread;
            contagem_thread['A'] = 0;
            contagem_thread['T'] = 0;
            contagem_thread['C'] = 0;
            contagem_thread['G'] = 0;

            #pragma omp for nowait
            for (size_t i = 0; i < sequences.size(); ++i) {
                const std::string& seq = sequences[i];
                for (char c : seq) {
                    char base = to_upper(c);
                    if (contagem_thread.count(base)) {
                        contagem_thread[base]++;
                    }
                }
            }

            // Redução das contagens das threads
            #pragma omp critical
            {
                for (auto& pair : contagem_thread) {
                    contagem_local[pair.first] += pair.second;
                }
            }
        }
    }

    // Redução das contagens dos processos
    long long contagem_global[4]; // Ordem: A, T, C, G
    long long contagem_proc[4] = {
        contagem_local['A'],
        contagem_local['T'],
        contagem_local['C'],
        contagem_local['G']
    };

    MPI_Reduce(contagem_proc, contagem_global, 4, MPI_LONG_LONG, MPI_SUM, 0, MPI_COMM_WORLD);

    double end_time = MPI_Wtime();  // Fim da medição do tempo
    double total_time = end_time - start_time;

    if (rank == 0) {
        std::cout << "Contagem total de bases:" << std::endl;
        std::cout << "A: " << contagem_global[0] << std::endl;
        std::cout << "T: " << contagem_global[1] << std::endl;
        std::cout << "C: " << contagem_global[2] << std::endl;
        std::cout << "G: " << contagem_global[3] << std::endl;
        std::cout << "Tempo total de execução: " << total_time << " segundos." << std::endl;
    }

    MPI_Finalize();
    return 0;
}
