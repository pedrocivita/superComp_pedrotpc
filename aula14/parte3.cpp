#include <iostream>
#include <random>
#include <chrono>
#include <omp.h>

int main() {
    const int N = 10000000;
    int dentro_circulo = 0;

    auto start = std::chrono::high_resolution_clock::now();

    #pragma omp parallel reduction(+:dentro_circulo)
    {
        // Inicializa o gerador de números aleatórios para cada thread
        std::mt19937 gen(omp_get_thread_num());
        std::uniform_real_distribution<double> dist(0.0, 1.0);

        #pragma omp for
        for (int i = 0; i < N; i++) {
            double x = dist(gen);
            double y = dist(gen);

            if (x * x + y * y <= 1.0) {
                dentro_circulo++;
            }
        }
    }

    double pi = 4.0 * dentro_circulo / N;

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;

    std::cout << "Valor estimado de pi (otimizado): " << pi << std::endl;
    std::cout << "Tempo de execução (otimizado): " << duration.count() << " segundos" << std::endl;

    return 0;
}
