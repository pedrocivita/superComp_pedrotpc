#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>  // Biblioteca para medir o tempo de execução
#include <cstdlib> // Biblioteca para gerar números aleatórios
#include <ctime>   // Biblioteca para usar o tempo atual como semente

// Função que implementa a solução ótima usando recursão
bool subsetSumOptimal(const std::vector<int>& set, int n, int target) {
    // Caso base: se o alvo for 0, encontramos uma solução
    if (target == 0) {
        return true;
    }
    // Caso base: se não restam elementos e o alvo ainda não foi atingido
    if (n == 0 && target != 0) {
        return false;
    }
    // Se o último elemento é maior que o alvo, não o incluímos no subconjunto
    if (set[n - 1] > target) {
        return subsetSumOptimal(set, n - 1, target);
    }
    // Verifica se a soma pode ser encontrada incluindo ou excluindo o último elemento
    return subsetSumOptimal(set, n - 1, target) || subsetSumOptimal(set, n - 1, target - set[n - 1]);
}

// Função auxiliar para gerar um vetor aleatório de números inteiros grandes
std::vector<int> generateRandomSet(int size, int maxValue) {
    std::vector<int> set(size);
    for (int i = 0; i < size; i++) {
        set[i] = rand() % maxValue + 1;  // Gera números entre 1 e maxValue
    }
    return set;
}

int main() {
    srand(time(0)); // Inicializa a semente para geração de números aleatórios

    // Pequenas entradas (1ª ordem de grandeza)
    std::vector<int> smallSet1 = generateRandomSet(50, 10000);
    std::vector<int> smallSet2 = generateRandomSet(75, 10000);

    // Entradas médias (2ª ordem de grandeza)
    std::vector<int> mediumSet1 = generateRandomSet(100, 50000);
    std::vector<int> mediumSet2 = generateRandomSet(150, 50000);

    // Grandes entradas (3ª ordem de grandeza)
    std::vector<int> largeSet1 = generateRandomSet(200, 100000); // Reduzido de 400 para 200
    std::vector<int> largeSet2 = generateRandomSet(250, 100000); // Reduzido de 450 para 250

    // Entradas ainda maiores (4ª ordem de grandeza)
    std::vector<int> extraLargeSet1 = generateRandomSet(300, 500000); // Reduzido de 700 para 300
    std::vector<int> extraLargeSet2 = generateRandomSet(350, 500000); // Reduzido de 800 para 350

    // Entradas ainda maiores (5ª ordem de grandeza) - reduzido levemente
    std::vector<int> extraExtraLargeSet1 = generateRandomSet(400, 1000000); // Reduzido de 700 para 400
    std::vector<int> extraExtraLargeSet2 = generateRandomSet(450, 1000000); // Reduzido de 750 para 450

    // Alvos para cada conjunto
    std::vector<int> targets = {10000, 15000, 25000, 40000, 75000, 100000, 200000, 300000, 450000, 550000};

    // Vetores de teste para entradas pequenas, médias, grandes e muito grandes
    std::vector<std::vector<int>> testSets = {
        smallSet1, smallSet2,
        mediumSet1, mediumSet2,
        largeSet1, largeSet2,
        extraLargeSet1, extraLargeSet2,
        extraExtraLargeSet1, extraExtraLargeSet2
    };

    // Executar o algoritmo para as entradas geradas
    for (int i = 0; i < testSets.size(); i++) {
        auto start = std::chrono::high_resolution_clock::now();
        bool result = subsetSumOptimal(testSets[i], testSets[i].size(), targets[i]);
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration = end - start;

        std::cout << "Conjunto " << i + 1 << " (tamanho: " << testSets[i].size() << "), Alvo " << targets[i]
                  << ": " << (result ? "Sim" : "Nao") << std::endl;
        std::cout << "Tempo de execucao: " << duration.count() << " segundos" << std::endl;
    }

    return 0;
}
