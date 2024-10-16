#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <ctime>

// Função que aplica a heurística para o Problema do Subconjunto de Soma
bool encontrarSubconjunto(const std::vector<int>& conjunto, int alvo) {
    // Faz uma cópia do conjunto original para ordenação
    std::vector<int> copiaConjunto = conjunto;
    
    // Ordenar em ordem decrescente
    std::sort(copiaConjunto.rbegin(), copiaConjunto.rend());

    int somaAtual = 0;

    // Itera pelos elementos do conjunto ordenado
    for (const auto& elem : copiaConjunto) {
        // Soma o elemento ao subconjunto se não ultrapassar o alvo
        if (somaAtual + elem <= alvo) {
            somaAtual += elem;
        }
        // Se a soma for igual ao alvo, retorna verdadeiro
        if (somaAtual == alvo) {
            return true;
        }
    }
    
    // Se não encontrar o subconjunto que atinge o alvo, retorna falso
    return false;
}

// Função para gerar um vetor de números aleatórios
std::vector<int> gerarConjuntoAleatorio(int tamanho, int valorMaximo) {
    std::vector<int> conjunto(tamanho);
    for (int& num : conjunto) {
        num = rand() % valorMaximo + 1;  // Gera números de 1 até valorMaximo
    }
    return conjunto;
}

int main() {
    srand(static_cast<unsigned>(time(0))); // Inicializa o gerador de números aleatórios com base no tempo atual

    // Entrada de dados
    int tamanhoConjunto, valorAlvo;
    
    std::cout << "Informe o tamanho do conjunto: ";
    std::cin >> tamanhoConjunto;
    
    std::cout << "Informe o valor alvo: ";
    std::cin >> valorAlvo;

    // Gera o conjunto de números aleatórios
    std::vector<int> conjunto = gerarConjuntoAleatorio(tamanhoConjunto, 100);

    // Mede o tempo de execução da heurística
    auto inicio = std::chrono::high_resolution_clock::now();
    bool encontrado = encontrarSubconjunto(conjunto, valorAlvo);
    auto fim = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duracao = fim - inicio;

    // Exibe os resultados
    std::cout << "Conjunto (tamanho: " << conjunto.size() << "), Alvo: " << valorAlvo << ": "
              << (encontrado ? "Encontrado" : "Nao encontrado") << std::endl;
    std::cout << "Tempo de execucao: " << duracao.count() << " segundos" << std::endl;

    return 0;
}
