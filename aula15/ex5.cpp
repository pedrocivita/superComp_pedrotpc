#include <iostream>
#include <omp.h>

int main() {
    const int num_threads = 4;
    int soma_total = 0;

    #pragma omp parallel num_threads(num_threads) reduction(+:soma_total)
    {
        int tid = omp_get_thread_num();
        int valor = tid + 1;  // Cada thread possui um valor único para somar
        soma_total += valor;
        std::cout << "Thread " << tid << " somou " << valor << ", soma parcial: " << soma_total << std::endl;
    }

    std::cout << "Soma total: " << soma_total << std::endl;
    return 0;
}
