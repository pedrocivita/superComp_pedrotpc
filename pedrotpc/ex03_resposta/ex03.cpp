#include <iostream>
#include <vector>
#include <omp.h>
#include <algorithm>
#include <chrono>

double calculateMean(const std::vector<double>& data) {
    double sum = 0.0;
    size_t n = data.size();

    auto start = std::chrono::high_resolution_clock::now();

    #pragma omp parallel for reduction(+:sum)
    for (size_t i = 0; i < n; ++i) {
        sum += data[i];
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "Tempo para calcular a média: " << elapsed.count() << " segundos." << std::endl;

    return sum / n;
}

double findMax(const std::vector<double>& data) {
    double maxVal = data[0];

    auto start = std::chrono::high_resolution_clock::now();

    #pragma omp parallel for reduction(max:maxVal)
    for (size_t i = 0; i < data.size(); ++i) {
        if (data[i] > maxVal) {
            maxVal = data[i];
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "Tempo para encontrar o máximo: " << elapsed.count() << " segundos." << std::endl;

    return maxVal;
}

double calculateProduct(const std::vector<double>& data) {
    double product = 1.0;

    auto start = std::chrono::high_resolution_clock::now();

    #pragma omp parallel for reduction(*:product)
    for (size_t i = 0; i < data.size(); ++i) {
        product *= data[i];
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "Tempo para calcular o produto: " << elapsed.count() << " segundos." << std::endl;

    return product;
}

int main() {
    std::vector<double> data(1000000, 1.000001);

    double mean = 0.0, maxVal = 0.0, product = 0.0;

    auto total_start = std::chrono::high_resolution_clock::now();

    #pragma omp parallel
    {
        #pragma omp sections
        {
            #pragma omp section
            {
                mean = calculateMean(data);
            }
            #pragma omp section
            {
                maxVal = findMax(data);
            }
            #pragma omp section
            {
                product = calculateProduct(data);
            }
        }
    }

    // Fim da medição de tempo total
    auto total_end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> total_elapsed = total_end - total_start;

    // Exibe os resultados
    std::cout << "Média: " << mean << std::endl;
    std::cout << "Máximo: " << maxVal << std::endl;
    std::cout << "Produto: " << product << std::endl;

    std::cout << "Tempo total de execução: " << total_elapsed.count() << " segundos." << std::endl;

    return 0;
}
