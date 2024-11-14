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
    // Prolina
    {"CCA", 2}, {"CCG", 2}, {"CCU", 2}, {"CCC", 2},
    // Serina
    {"UCU", 3}, {"UCA", 3}, {"UCG", 3}, {"UCC", 3},
    // Glutamina
    {"CAG", 4}, {"CAA", 4},
    // Treonina
    {"ACA", 5}, {"ACC", 5}, {"ACU", 5}, {"ACG", 5},
    // Cisteína
    {"UGC", 6}, {"UGU", 6},
    // Valina
    {"GUG", 7}, {"GUA", 7}, {"GUC", 7}, {"GUU", 7},
    // Códon STOP
    {"UGA", 0}
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
        std::string filename = "../cromossomos/chr" + std::to_string(crom) + "_rna.fa";
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Processo " << rank << ": erro ao abrir o arquivo " << filename << std::endl;
            continue;
        }

        std::string line;
        std::string sequence;

        while (std::getline(file, line)) {
            if (line.empty() || line[0] == '>') continue; // Ignora linhas de descrição
            for (char c : line) {
                sequence += to_upper(c);
            }
        }
        file.close();

        size_t seq_size = sequence.size();
        std::vector<int> proteina;

        // Variáveis de controle
        bool inicio = false;
        bool stop_codon_found = false;
        size_t seq_end = seq_size - seq_size % 3; // Evitar ultrapassar o tamanho da sequência

        // Tradução utilizando OpenMP
        #pragma omp parallel
        {
            std::vector<int> proteina_thread;
            bool local_inicio = false;

            #pragma omp for nowait
            for (size_t i = 0; i <= seq_end - 3; i += 3) {
                if (stop_codon_found) continue;

                std::string codon = sequence.substr(i, 3);

                if (!inicio && !local_inicio) {
                    if (codon == "AUG") {
                        local_inicio = true;
                        #pragma omp critical
                        {
                            inicio = true;
                            proteina.push_back(codon_table[codon]);
                        }
                    }
                    continue;
                }

                if (local_inicio) {
                    if (codon == "UGA") {
                        #pragma omp atomic write
                        stop_codon_found = true;
                        continue;
                    }

                    if (codon_table.count(codon)) {
                        int aminoacido = codon_table[codon];
                        proteina_thread.push_back(aminoacido);
                    }
                }
            }

            // Combinar as proteínas traduzidas pelas threads
            if (!proteina_thread.empty()) {
                #pragma omp critical
                {
                    proteina.insert(proteina.end(), proteina_thread.begin(), proteina_thread.end());
                }
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
