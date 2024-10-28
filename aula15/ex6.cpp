#include <iostream>
#include <omp.h>

int main() {
    const int num_threads = 4;

    #pragma omp parallel num_threads(num_threads)
    {
        int tid = omp_get_thread_num();
        if (tid == 0) {
            for (int i = 1; i < num_threads; i++) {
                std::cout << "Thread 0 enviando mensagem para Thread " << i << std::endl;
                std::cout << "Thread " << i << " respondeu à Thread 0" << std::endl;
            }
        }
    }
    return 0;
}
