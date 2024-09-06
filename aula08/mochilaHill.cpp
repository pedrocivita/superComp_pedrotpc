#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <chrono>

struct Item {
    int peso;
    int valor;
};

// Função que calcula o peso de uma solução
int calcularPeso(const std::vector<Item>& itens, const std::vector<bool>& solucao) {
    int pesoTotal = 0;
    for (size_t i = 0; i < itens.size(); ++i) {
        if (solucao[i]) {
            pesoTotal += itens[i].peso;
        }
    }
    return pesoTotal;
}

// Função que calcula o valor de uma solução
int calcularValor(const std::vector<Item>& itens, const std::vector<bool>& solucao) {
    int valorTotal = 0;
    for (size_t i = 0; i < itens.size(); ++i) {
        if (solucao[i]) {
            valorTotal += itens[i].valor;
        }
    }
    return valorTotal;
}

// Função que ajusta a solução se o peso exceder o limite
void ajustarSolucao(std::vector<Item>& itens, std::vector<bool>& solucao, int& pesoAtual, int& valorAtual, int capacidadeMaxima) {
    for (size_t i = 0; i < solucao.size(); ++i) {
        if (pesoAtual > capacidadeMaxima && solucao[i]) {
            solucao[i] = false;
            pesoAtual -= itens[i].peso;
            valorAtual -= itens[i].valor;
        }
    }
}

// Função para fazer a Mochila Cheia
int mochilaCheia(std::vector<Item>& itens, std::vector<bool>& solucao, int capacidadeMaxima) {
    int pesoAtual = calcularPeso(itens, solucao);
    int valorAtual = calcularValor(itens, solucao);

    // Tenta adicionar itens à solução
    for (size_t i = 0; i < itens.size(); ++i) {
        if (!solucao[i] && pesoAtual + itens[i].peso <= capacidadeMaxima) {
            solucao[i] = true;
            pesoAtual += itens[i].peso;
            valorAtual += itens[i].valor;
        }
    }

    ajustarSolucao(itens, solucao, pesoAtual, valorAtual, capacidadeMaxima);
    return valorAtual;
}

// Função para realizar a Substituição de Objeto
int substituicaoDeObjeto(std::vector<Item>& itens, std::vector<bool>& solucao, int capacidadeMaxima) {
    int valorAtual = calcularValor(itens, solucao);
    int pesoAtual = calcularPeso(itens, solucao);

    bool trocou = true;
    while (trocou) {
        trocou = false;
        for (size_t i = 0; i < itens.size(); ++i) {
            if (!solucao[i]) { // Se o item não está na solução
                for (size_t j = 0; j < itens.size(); ++j) {
                    if (solucao[j] && itens[i].peso <= itens[j].peso && itens[i].valor > itens[j].valor) {
                        solucao[j] = false;
                        solucao[i] = true;
                        pesoAtual = pesoAtual - itens[j].peso + itens[i].peso;
                        valorAtual = valorAtual - itens[j].valor + itens[i].valor;
                        trocou = true;
                        ajustarSolucao(itens, solucao, pesoAtual, valorAtual, capacidadeMaxima);
                        break;
                    }
                }
            }
        }
    }
    return valorAtual;
}

// Função para Hill Climbing
int hillClimbing(std::vector<Item>& itens, std::vector<bool>& solucao, int capacidadeMaxima) {
    int valorAtual = calcularValor(itens, solucao);
    int pesoAtual = calcularPeso(itens, solucao);

    bool melhorou = true;
    while (melhorou) {
        melhorou = false;
        for (size_t i = 0; i < itens.size(); ++i) {
            std::vector<bool> vizinho = solucao;
            vizinho[i] = !vizinho[i]; // Inverte o bit

            int pesoVizinho = calcularPeso(itens, vizinho);
            int valorVizinho = calcularValor(itens, vizinho);

            if (pesoVizinho <= capacidadeMaxima && valorVizinho > valorAtual) {
                solucao = vizinho;
                valorAtual = valorVizinho;
                pesoAtual = pesoVizinho;
                melhorou = true;
            }
        }
    }

    ajustarSolucao(itens, solucao, pesoAtual, valorAtual, capacidadeMaxima);
    return valorAtual;
}

int main() {
    std::srand(std::time(0));

    // Inicia a medição do tempo
    auto inicio = std::chrono::high_resolution_clock::now();

    int N, W;
    std::cin >> N >> W;

    std::vector<Item> itens(N);
    for (int i = 0; i < N; ++i) {
        std::cin >> itens[i].peso >> itens[i].valor;
    }

    // Gera uma solução inicial aleatória
    std::vector<bool> solucao(N);
    for (int i = 0; i < N; ++i) {
        solucao[i] = std::rand() % 2;
    }

    // Ajusta a solução inicial se necessário
    int pesoInicial = calcularPeso(itens, solucao);
    int valorInicial = calcularValor(itens, solucao);
    ajustarSolucao(itens, solucao, pesoInicial, valorInicial, W);

    std::cout << "Peso e valor iniciais: " << pesoInicial << " " << valorInicial << std::endl;

    // Executa a fase de Mochila Cheia
    int valorAposMochilaCheia = mochilaCheia(itens, solucao, W);
    std::cout << "Valor após Mochila Cheia: " << valorAposMochilaCheia << std::endl;

    // Executa a fase de Substituição de Objeto
    int valorAposSubstituicao = substituicaoDeObjeto(itens, solucao, W);
    std::cout << "Valor após Substituição de Objeto: " << valorAposSubstituicao << std::endl;

    // Executa a fase de Hill Climbing
    int valorFinal = hillClimbing(itens, solucao, W);
    std::cout << "Valor final após Hill Climbing: " << valorFinal << std::endl;

    // Finaliza a medição do tempo
    auto fim = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duracao = fim - inicio;
    std::cout << "Tempo de execução: " << duracao.count() << " segundos" << std::endl;

    return 0;
}
