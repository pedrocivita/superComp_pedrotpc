#include <iostream>
#include <vector>
#include <fstream>
#include <chrono>
#include <random>

struct Item {
    int peso;
    int valor;
};

// Função para ler os dados do arquivo de entrada
void lerEntrada(const std::string& nomeArquivo, std::vector<Item>& itens, int& capacidade) {
    std::ifstream arquivo(nomeArquivo);
    if (!arquivo) {
        std::cerr << "Erro ao abrir o arquivo: " << nomeArquivo << std::endl;
        exit(1);
    }

    int n;
    arquivo >> n >> capacidade;
    itens.resize(n);

    for (int i = 0; i < n; ++i) {
        arquivo >> itens[i].peso >> itens[i].valor;
    }
}

// Heurística: Seleção Aleatória Baseada em Probabilidade
int heuristicaProbabilidade(std::vector<Item>& itens, int capacidade, double limiar) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);

    int pesoOcupado = 0;
    int valorTotal = 0;

    // Percorrer os itens sem alterar a ordem
    for (const auto& item : itens) {
        double probabilidade = dis(gen);

        // Se a probabilidade for maior que o limiar e o item couber na mochila, adiciona o item
        if (probabilidade > limiar && pesoOcupado + item.peso <= capacidade) {
            pesoOcupado += item.peso;
            valorTotal += item.valor;
        }
    }

    return valorTotal;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Uso: " << argv[0] << " <nome do arquivo de entrada>" << std::endl;
        return 1;
    }

    std::vector<Item> itens;
    int capacidade;

    std::string nomeArquivo = argv[1];
    
    lerEntrada(nomeArquivo, itens, capacidade);

    double limiar = 0.5;  // Limiar de probabilidade

    // Executando a heurística de seleção aleatória baseada em probabilidade
    auto inicio = std::chrono::high_resolution_clock::now();
    int valorHeuristica = heuristicaProbabilidade(itens, capacidade, limiar);
    auto fim = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duracao = fim - inicio;

    std::cout << "Arquivo: " << nomeArquivo << std::endl;
    std::cout << "Valor alcançado pela heurística: " << valorHeuristica << " dinheiros" << std::endl;
    std::cout << "Tempo de execução da heurística: " << duracao.count() << " segundos" << std::endl;

    return 0;
}
