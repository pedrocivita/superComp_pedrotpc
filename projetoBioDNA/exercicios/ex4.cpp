#include <mpi.h>
#include <omp.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>

// Mapeamento de códons para aminoácidos (representados por números)
std::map<std::string, int> codon_table = {
    {"AUG", 1}, // Metionina (Início)
    // Adicione todos os códons e seus respectivos aminoácidos
    {"UUU", 2}, {"UUC", 2}, // Fenilalanina
    {"UUA", 3}, {"UUG", 3}, // Leucina
    // ...
    {"UAA", 0}, {"UAG", 0}, {"UGA", 0} // Códores de parada
};

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

        size_t seq_size = sequence.size();
        std::vector<int> proteina;

        // Tradução utilizando OpenMP
        bool stop_codon_found = false;
        #pragma omp parallel
        {
            std::vector<int> proteina_thread;

            #pragma omp for nowait
            for (size_t i = 0; i <= seq_size - 3; i += 3) {
                if (stop_codon_found) continue;

                std::string codon = "";
                codon += to_upper(sequence[i]);
                codon += to_upper(sequence[i + 1]);
                codon += to_upper(sequence[i + 2]);

                int aminoacido = 0;
                if (codon_table.count(codon)) {
                    aminoacido = codon_table[codon];
                }

                if (aminoacido == 0) { // Códon de parada
                    stop_codon_found = true;
                } else {
                    proteina_thread.push_back(aminoacido);
                }
            }

            // Combinar as proteínas traduzidas pelas threads
            #pragma omp critical
            {
                proteina.insert(proteina.end(), proteina_thread.begin(), proteina_thread.end());
            }
        }

        // Exibir a sequência de aminoácidos
        std::cout << "Processo " << rank << ", Cromossomo " << crom << ", Proteína traduzida: ";
        for (int aminoacido : proteina) {
            std::cout << aminoacido << "-";
        }
        std::cout << "Fim" << std::endl;
    }

    double end_time = MPI_Wtime();  // Fim da medição do tempo
    double total_time = end_time - start_time;

    // Exibição do tempo de execução por cada processo
    std::cout << "Processo " << rank << ": tempo de execução = " << total_time << " segundos." << std::endl;

    MPI_Finalize();
    return 0;
}
