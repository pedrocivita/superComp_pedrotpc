#include <mpi.h>
#include <omp.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

// Função para transcrever DNA para RNA
char transcrever(char base) {
    switch (base) {
        case 'A': return 'U';
        case 'T': return 'A';
        case 'C': return 'G';
        case 'G': return 'C';
        default: return 'N'; // N para bases desconhecidas
    }
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

    // Processamento dos arquivos atribuídos ao processo
    for (int crom : cromossomos_local) {
        std::string filename_in = "chr" + std::to_string(crom) + ".subst.fa";
        std::string filename_out = "chr" + std::to_string(crom) + "_rna.fa";
        std::ifstream file_in(filename_in);
        std::ofstream file_out(filename_out);

        if (!file_in.is_open()) {
            std::cerr << "Processo " << rank << ": erro ao abrir o arquivo " << filename_in << std::endl;
            continue;
        }
        if (!file_out.is_open()) {
            std::cerr << "Processo " << rank << ": erro ao criar o arquivo " << filename_out << std::endl;
            continue;
        }

        std::string line;
        std::vector<std::string> sequences;
        std::vector<std::string> headers;

        while (std::getline(file_in, line)) {
            if (line.empty()) continue;
            if (line[0] == '>') {
                headers.push_back(line);
                sequences.push_back("");
            } else {
                sequences.back() += line;
            }
        }
        file_in.close();

        // Transcrição utilizando OpenMP
        #pragma omp parallel for
        for (size_t i = 0; i < sequences.size(); ++i) {
            std::string& seq = sequences[i];
            for (size_t j = 0; j < seq.size(); ++j) {
                char base = toupper(seq[j]);
                seq[j] = transcrever(base);
            }
        }

        // Escreve a sequência transcrita no arquivo de saída
        for (size_t i = 0; i < sequences.size(); ++i) {
            file_out << headers[i] << std::endl;
            file_out << sequences[i] << std::endl;
        }
        file_out.close();

        // Adiciona mensagem de status
        std::cout << "Processo " << rank << ": transcrição do cromossomo " << crom << " concluída." << std::endl;
    }

    double end_time = MPI_Wtime();  // Fim da medição do tempo
    double total_time = end_time - start_time;

    // Exibição do tempo de execução por cada processo
    std::cout << "Processo " << rank << ": tempo de execução = " << total_time << " segundos." << std::endl;

    MPI_Finalize();
    return 0;
}
