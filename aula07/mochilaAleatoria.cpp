#include <iostream>
#include <vector>
#include <fstream>
#include <chrono>
#include <algorithm>
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

// Heurística: Embaralhar e Preencher a Mochila
int heuristicaAleatoria(std::vector<Item>& itens, int capacidade) {
    // Embaralhar o vetor de itens
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(itens.begin(), itens.end(), g);

    int pesoOcupado = 0;
    int valorTotal = 0;

    // Percorrer os itens e adicionar à mochila enquanto houver capacidade
    for (const auto& item : itens) {
        if (pesoOcupado + item.peso <= capacidade) {
            pesoOcupado += item.peso;
            valorTotal += item.valor;
        } else {
            break;
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

    // Executando a heurística aleatória
    auto inicio = std::chrono::high_resolution_clock::now();
    int valorHeuristica = heuristicaAleatoria(itens, capacidade);
    auto fim = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duracao = fim - inicio;

    std::cout << "Arquivo: " << nomeArquivo << std::endl;
    std::cout << "Valor alcançado pela heurística: " << valorHeuristica << " dinheiros" << std::endl;
    std::cout << "Tempo de execução da heurística: " << duracao.count() << " segundos" << std::endl;

    return 0;
}
