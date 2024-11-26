#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <random>
#include <chrono>

// Definição de constantes
const int NUM_ITEMS = 40;                // Número de itens disponíveis
const int KNAPSACK_CAPACITY = 100;       // Capacidade máxima da mochila
const int POPULATION_SIZE = 16;          // Tamanho da população
const int NUM_GENERATIONS = 1000;        // Número de gerações
const double CROSSOVER_RATE = 0.8;       // Taxa de crossover
const double MUTATION_RATE = 0.15;       // Taxa de mutação
const int NUM_PARENTS = POPULATION_SIZE / 2;    // Número de pais selecionados
const int NUM_OFFSPRINGS = POPULATION_SIZE - NUM_PARENTS;  // Número de descendentes gerados

// Inicialização dos geradores de números aleatórios
std::random_device rd;
std::mt19937 gen(rd());

// Função para gerar inteiros aleatórios em um intervalo
int randInt(int min, int max) {
    std::uniform_int_distribution<> dis(min, max);
    return dis(gen);
}

// Função para gerar números de ponto flutuante aleatórios em um intervalo
double randDouble(double min, double max) {
    std::uniform_real_distribution<> dis(min, max);
    return dis(gen);
}

// Função para imprimir os itens disponíveis
void printItems(const std::vector<int>& weights, const std::vector<int>& values) {
    std::cout << "Possíveis itens:\n";
    std::cout << "Item\tPeso\tValor\n";
    for (int i = 0; i < NUM_ITEMS; ++i) {
        std::cout << i + 1 << "\t" << weights[i] << "\t" << values[i] << "\n";
    }
}

// Função de fitness
std::vector<int> fitness(const std::vector<std::vector<int>>& population,
                         const std::vector<int>& weights,
                         const std::vector<int>& values,
                         int capacity) {
    std::vector<int> fitness_values(population.size());
    for (size_t i = 0; i < population.size(); ++i) {
        int total_value = 0;
        int total_weight = 0;
        for (size_t j = 0; j < population[i].size(); ++j) {
            total_value += population[i][j] * values[j];
            total_weight += population[i][j] * weights[j];
        }
        if (total_weight <= capacity) {
            fitness_values[i] = total_value;
        } else {
            fitness_values[i] = 0;  // Penaliza soluções que excedem a capacidade
        }
    }
    return fitness_values;
}

// Função de seleção
std::vector<std::vector<int>> selection(const std::vector<int>& fitness_values,
                                        const std::vector<std::vector<int>>& population,
                                        int num_parents) {
    std::vector<std::vector<int>> parents(num_parents);
    std::vector<int> fitness_copy = fitness_values;
    for (int i = 0; i < num_parents; ++i) {
        auto max_it = std::max_element(fitness_copy.begin(), fitness_copy.end());
        int max_idx = std::distance(fitness_copy.begin(), max_it);
        parents[i] = population[max_idx];
        fitness_copy[max_idx] = -9999;  // Exclui este indivíduo de futuras seleções
    }
    return parents;
}

// Função de crossover
std::vector<std::vector<int>> crossover(const std::vector<std::vector<int>>& parents,
                                        int num_offsprings) {
    std::vector<std::vector<int>> offsprings;
    int crossover_point = NUM_ITEMS / 2;
    int i = 0;
    int cnt_offsprings = 0;

    while (cnt_offsprings < num_offsprings) {
        int parent1_index = i % parents.size();
        int parent2_index = (i + 1) % parents.size();
        double x = randDouble(0.0, 1.0);
        if (x > CROSSOVER_RATE) {
            // Não realiza crossover
        } else {
            std::vector<int> offspring(NUM_ITEMS);
            // Primeira metade do pai 1
            std::copy(parents[parent1_index].begin(),
                      parents[parent1_index].begin() + crossover_point,
                      offspring.begin());
            // Segunda metade do pai 2
            std::copy(parents[parent2_index].begin() + crossover_point,
                      parents[parent2_index].end(),
                      offspring.begin() + crossover_point);
            offsprings.push_back(offspring);
            cnt_offsprings++;
        }
        i++;
    }
    return offsprings;
}

// Função de mutação
std::vector<std::vector<int>> mutation(std::vector<std::vector<int>>& offsprings) {
    for (size_t i = 0; i < offsprings.size(); ++i) {
        double random_value = randDouble(0.0, 1.0);
        if (random_value > MUTATION_RATE) {
            continue;
        }
        int mutation_point = randInt(0, NUM_ITEMS - 1);
        offsprings[i][mutation_point] = 1 - offsprings[i][mutation_point];  // Inverte o gene
    }
    return offsprings;
}

// Função de otimização
std::pair<std::vector<int>, std::vector<std::vector<int>>> optimize(
    std::vector<std::vector<int>>& population,
    const std::vector<int>& weights,
    const std::vector<int>& values,
    int capacity) {
    std::vector<std::vector<int>> fitness_history;
    std::vector<int> parameters;

    for (int generation = 0; generation < NUM_GENERATIONS; ++generation) {
        // Calcula o fitness
        std::vector<int> fitness_values = fitness(population, weights, values, capacity);
        fitness_history.push_back(fitness_values);

        // Seleção
        std::vector<std::vector<int>> parents = selection(fitness_values, population, NUM_PARENTS);

        // Crossover
        std::vector<std::vector<int>> offsprings = crossover(parents, NUM_OFFSPRINGS);

        // Mutação
        offsprings = mutation(offsprings);

        // Cria nova população
        population.clear();
        population.insert(population.end(), parents.begin(), parents.end());
        population.insert(population.end(), offsprings.begin(), offsprings.end());
    }

    // Cálculo final de fitness
    std::vector<int> final_fitness = fitness(population, weights, values, capacity);
    auto max_it = std::max_element(final_fitness.begin(), final_fitness.end());
    int max_idx = std::distance(final_fitness.begin(), max_it);
    parameters = population[max_idx];

    return std::make_pair(parameters, fitness_history);
}

int main() {
    // Sementeia o gerador de números aleatórios com o tempo atual
    gen.seed(std::chrono::system_clock::now().time_since_epoch().count());

    // Gera pesos e valores aleatórios para os itens
    std::vector<int> weights(NUM_ITEMS);
    std::vector<int> values(NUM_ITEMS);
    for (int i = 0; i < NUM_ITEMS; ++i) {
        weights[i] = randInt(1, 15);  // Pesos entre 1 e 15
        values[i] = randInt(1, 100);  // Valores entre 1 e 100
    }

    // Imprime os itens disponíveis
    printItems(weights, values);

    // Inicializa a população
    std::vector<std::vector<int>> population(POPULATION_SIZE, std::vector<int>(NUM_ITEMS));
    for (auto& individual : population) {
        for (int& gene : individual) {
            gene = randInt(0, 1);
        }
    }

    // Início da medição do tempo
    auto start_time = std::chrono::high_resolution_clock::now();

    // Executa a otimização
    std::pair<std::vector<int>, std::vector<std::vector<int>>> result = optimize(population, weights, values, KNAPSACK_CAPACITY);
    std::vector<int> best_solution = result.first;
    std::vector<std::vector<int>> fitness_history = result.second;

    // Fim da medição do tempo
    auto end_time = std::chrono::high_resolution_clock::now();

    // Calcula a duração
    std::chrono::duration<double> duration = end_time - start_time;

    // Exibe os resultados
    std::cout << "\nMelhor solução encontrada:\n";
    int total_weight = 0;
    int total_value = 0;
    for (size_t i = 0; i < best_solution.size(); ++i) {
        if (best_solution[i] == 1) {
            std::cout << "Item " << i + 1 << ": Peso = " << weights[i]
                      << ", Valor = " << values[i] << "\n";
            total_weight += weights[i];
            total_value += values[i];
        }
    }

    // Verifica se a solução é válida (peso total não excede a capacidade)
    if (total_weight <= KNAPSACK_CAPACITY) {
        std::cout << "Peso total: " << total_weight << "\n";
        std::cout << "Valor total: " << total_value << "\n";
    } else {
        std::cout << "Solução inválida: o peso total excede a capacidade da mochila.\n";
        std::cout << "Peso total: " << total_weight << " (Capacidade máxima: " << KNAPSACK_CAPACITY << ")\n";
        std::cout << "Valor total: " << total_value << "\n";
    }

    // Exibe o tempo de execução
    std::cout << "Tempo de execução: " << duration.count() << " segundos\n";

    return 0;
}
