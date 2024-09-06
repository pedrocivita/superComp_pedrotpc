#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <ctime>

struct Item {
    int peso;
    int valor;
};

// Função para ler dados do arquivo
void lerDados(const std::string& nomeArquivo, int& capacidade, std::vector<Item>& itens) {
    std::ifstream arquivo(nomeArquivo);
    if (!arquivo) {
        std::cerr << "Erro ao abrir o arquivo " << nomeArquivo << std::endl;
        exit(1);
    }

    int numItens;
    arquivo >> numItens >> capacidade;
    itens.resize(numItens);

    for (int i = 0; i < numItens; ++i) {
        arquivo >> itens[i].peso >> itens[i].valor;
    }
}

// Heurística: Embaralhar e preencher
int heuristicaEmbaralharPreencher(int capacidade, std::vector<Item>& itens, int& pesoTotal) {
    std::random_shuffle(itens.begin(), itens.end());

    pesoTotal = 0;
    int valorTotal = 0;
    for (const auto& item : itens) {
        if (pesoTotal + item.peso <= capacidade) {
            pesoTotal += item.peso;
            valorTotal += item.valor;
        }
    }
    return valorTotal;
}

// Função para realizar a mochila cheia (tentando incluir o máximo de itens)
int mochilaCheia(int capacidade, std::vector<Item>& itens, int& pesoTotal) {
    pesoTotal = 0;
    int valorTotal = 0;
    for (const auto& item : itens) {
        if (pesoTotal + item.peso <= capacidade) {
            pesoTotal += item.peso;
            valorTotal += item.valor;
        }
    }
    return valorTotal;
}

// Função para Substituição de Objeto
int substituicaoDeObjeto(int capacidade, std::vector<Item>& itens, int& pesoTotal) {
    int valorTotal = mochilaCheia(capacidade, itens, pesoTotal);

    for (size_t i = 0; i < itens.size(); ++i) {
        for (size_t j = 0; j < itens.size(); ++j) {
            if (pesoTotal - itens[i].peso + itens[j].peso <= capacidade && itens[j].valor > itens[i].valor) {
                pesoTotal = pesoTotal - itens[i].peso + itens[j].peso;
                valorTotal = valorTotal - itens[i].valor + itens[j].valor;
            }
        }
    }
    return valorTotal;
}

// Hill Climbing para o problema da mochila
int hillClimbing(int capacidade, std::vector<Item>& itens, int& pesoTotal) {
    int melhorValor = mochilaCheia(capacidade, itens, pesoTotal);

    bool melhorou = true;
    while (melhorou) {
        melhorou = false;
        for (size_t i = 0; i < itens.size(); ++i) {
            int novoPeso = pesoTotal + (itens[i].peso * (pesoTotal < capacidade ? 1 : -1));
            int novoValor = melhorValor + (itens[i].valor * (pesoTotal < capacidade ? 1 : -1));

            if (novoPeso <= capacidade && novoValor > melhorValor) {
                pesoTotal = novoPeso;
                melhorValor = novoValor;
                melhorou = true;
            }
        }
    }
    return melhorValor;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Uso: " << argv[0] << " <arquivo de entrada>" << std::endl;
        return 1;
    }

    std::string nomeArquivo = argv[1];
    int capacidade;
    std::vector<Item> itens;

    lerDados(nomeArquivo, capacidade, itens);

    // Execução da Heurística Aleatória (Embaralhar e Preencher)
    for (int i = 0; i < 5; ++i) {
        int pesoTotal = 0;
        clock_t inicio = clock();
        int valor = heuristicaEmbaralharPreencher(capacidade, itens, pesoTotal);
        clock_t fim = clock();
        std::cout << "Heurística Aleatória Execução " << i + 1 << ": Valor = " << valor
                  << ", Peso = " << pesoTotal << ", Tempo = " << double(fim - inicio) / CLOCKS_PER_SEC << "s" << std::endl;
    }

    // Execução da Mochila Cheia
    for (int i = 0; i < 10; ++i) {
        int pesoTotal = 0;
        clock_t inicio = clock();
        int valor = mochilaCheia(capacidade, itens, pesoTotal);
        clock_t fim = clock();
        std::cout << "Mochila Cheia Execução " << i + 1 << ": Valor = " << valor
                  << ", Peso = " << pesoTotal << ", Tempo = " << double(fim - inicio) / CLOCKS_PER_SEC << "s" << std::endl;
    }

    // Execução da Substituição de Objeto
    for (int i = 0; i < 10; ++i) {
        int pesoTotal = 0;
        clock_t inicio = clock();
        int valor = substituicaoDeObjeto(capacidade, itens, pesoTotal);
        clock_t fim = clock();
        std::cout << "Substituição de Objeto Execução " << i + 1 << ": Valor = " << valor
                  << ", Peso = " << pesoTotal << ", Tempo = " << double(fim - inicio) / CLOCKS_PER_SEC << "s" << std::endl;
    }

    // Execução do Hill Climbing
    for (int i = 0; i < 10; ++i) {
        int pesoTotal = 0;
        clock_t inicio = clock();
        int valor = hillClimbing(capacidade, itens, pesoTotal);
        clock_t fim = clock();
        std::cout << "Hill Climbing Execução " << i + 1 << ": Valor = " << valor
                  << ", Peso = " << pesoTotal << ", Tempo = " << double(fim - inicio) / CLOCKS_PER_SEC << "s" << std::endl;
    }

    return 0;
}
