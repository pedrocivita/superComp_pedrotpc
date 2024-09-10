#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <chrono> // Biblioteca para medição de tempo

// Estrutura para armazenar os itens da mochila
struct Item {
    int peso;
    int valor;
};

// Função para ler os dados do arquivo
void lerDados(const std::string& nomeArquivo, int& capacidade, std::vector<Item>& itens) {
    std::ifstream arquivo(nomeArquivo);
    if (!arquivo) {
        std::cerr << "Erro ao abrir o arquivo " << nomeArquivo << std::endl;
        return;
    }

    int numeroItens;
    arquivo >> numeroItens >> capacidade;

    itens.resize(numeroItens);
    for (int i = 0; i < numeroItens; ++i) {
        arquivo >> itens[i].peso >> itens[i].valor;
    }
}

// Função para a heurística "Mais Caro Primeiro"
int maisCaroPrimeiro(int capacidade, std::vector<Item>& itens, int& pesoTotal) {
    // Ordena os itens por valor, decrescente
    std::sort(itens.begin(), itens.end(), [](const Item& a, const Item& b) {
        return a.valor > b.valor;
    });

    pesoTotal = 0;
    int valorTotal = 0;
    for (const auto& item : itens) {
        if (pesoTotal + item.peso <= capacidade) {
            pesoTotal += item.peso;
            valorTotal += item.valor;
        } else {
            break;
        }
    }
    return valorTotal;
}

// Função para a heurística "Mais Leve Primeiro"
int maisLevePrimeiro(int capacidade, std::vector<Item>& itens, int& pesoTotal) {
    // Ordena os itens por peso, crescente
    std::sort(itens.begin(), itens.end(), [](const Item& a, const Item& b) {
        return a.peso < b.peso;
    });

    pesoTotal = 0;
    int valorTotal = 0;
    for (const auto& item : itens) {
        if (pesoTotal + item.peso <= capacidade) {
            pesoTotal += item.peso;
            valorTotal += item.valor;
        } else {
            break;
        }
    }
    return valorTotal;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Uso: ./mochila <arquivo_de_entrada.txt>" << std::endl;
        return 1;
    }

    std::string nomeArquivo = argv[1];
    int capacidade;
    std::vector<Item> itens;

    lerDados(nomeArquivo, capacidade, itens);

    // Medir tempo de execução para heurística "Mais Caro Primeiro"
    int pesoTotalCaro = 0;
    auto inicioCaro = std::chrono::high_resolution_clock::now();
    int valorCaro = maisCaroPrimeiro(capacidade, itens, pesoTotalCaro);
    auto fimCaro = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duracaoCaro = fimCaro - inicioCaro;
    std::cout << "Heurística Mais Caro Primeiro: Valor = " << valorCaro << ", Peso Total = " << pesoTotalCaro 
              << ", Tempo = " << duracaoCaro.count() << " segundos" << std::endl;

    // Medir tempo de execução para heurística "Mais Leve Primeiro"
    int pesoTotalLeve = 0;
    auto inicioLeve = std::chrono::high_resolution_clock::now();
    int valorLeve = maisLevePrimeiro(capacidade, itens, pesoTotalLeve);
    auto fimLeve = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duracaoLeve = fimLeve - inicioLeve;
    std::cout << "Heurística Mais Leve Primeiro: Valor = " << valorLeve << ", Peso Total = " << pesoTotalLeve 
              << ", Tempo = " << duracaoLeve.count() << " segundos" << std::endl;

    return 0;
}
