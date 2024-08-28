#include <iostream>
#include <vector>
#include <fstream>
#include <chrono>

struct Item {
    int peso;
    int valor;
};

// Função recursiva para resolver o problema da mochila
int mochila(int capacidade, const std::vector<Item>& itens, int n, int& pesoOcupado) {
    // Caso base: sem itens restantes ou capacidade da mochila esgotada
    if (n == 0 || capacidade == 0) {
        return 0;
    }

    // Se o peso do n-ésimo item é maior que a capacidade, ele não pode ser incluído
    if (itens[n - 1].peso > capacidade) {
        return mochila(capacidade, itens, n - 1, pesoOcupado);
    } else {
        int pesoComInclusao = pesoOcupado + itens[n - 1].peso;
        int pesoSemInclusao = pesoOcupado;

        // Incluindo o item
        int inclui = itens[n - 1].valor + mochila(capacidade - itens[n - 1].peso, itens, n - 1, pesoComInclusao);
        
        // Não incluindo o item
        int nao_inclui = mochila(capacidade, itens, n - 1, pesoSemInclusao);

        if (inclui > nao_inclui) {
            pesoOcupado = pesoComInclusao;
            return inclui;
        } else {
            pesoOcupado = pesoSemInclusao;
            return nao_inclui;
        }
    }
}

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

int main() {
    std::vector<Item> itens;
    int capacidade;

    // Altere o nome do arquivo para testar os diferentes casos de entrada
    std::string nomeArquivo = "in4.txt";  // Pode ser "in1.txt", "in2.txt", "in3.txt", ou "in4.txt"
    
    lerEntrada(nomeArquivo, itens, capacidade);

    int pesoOcupado = 0;

    auto inicio = std::chrono::high_resolution_clock::now();

    int valorMaximo = mochila(capacidade, itens, itens.size(), pesoOcupado);

    auto fim = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duracao = fim - inicio;

    std::cout << "Arquivo: " << nomeArquivo << std::endl;
    std::cout << "Valor máximo alcançado: " << valorMaximo << " dinheiros" << std::endl;
    std::cout << "Peso ocupado: " << pesoOcupado << " Kg" << std::endl;
    std::cout << "Tempo de execução: " << duracao.count() << " segundos" << std::endl;

    return 0;
}
