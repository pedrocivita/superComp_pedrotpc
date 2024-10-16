#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <chrono>
#include <omp.h>

int main() {
    const int N = 10000000;
    int dentro_circulo = 0;

    std::srand(static_cast<unsigned>(std::time(0)));

    auto start = std::chrono::high_resolution_clock::now();

    #pragma omp parallel for reduction(+:dentro_circulo)
    for (int i = 0; i < N; i++) {
        double x = static_cast<double>(std::rand()) / RAND_MAX;
        double y = static_cast<double>(std::rand()) / RAND_MAX;

        if (x * x + y * y <= 1.0) {
            dentro_circulo++;
        }
    }

    double pi = 4.0 * dentro_circulo / N;

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;

    std::cout << "Valor estimado de pi (paralelo): " << pi << std::endl;
    std::cout << "Tempo de execução (paralelo): " << duration.count() << " segundos" << std::endl;

    return 0;
}
