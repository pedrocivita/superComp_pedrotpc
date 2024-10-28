#include <iostream>
#include <omp.h>

int main() {
    const int num_threads = 4;
    int mensagem = 0;

    #pragma omp parallel num_threads(num_threads) shared(mensagem)
    {
        int tid = omp_get_thread_num();
        if (tid == 0) {
            mensagem = 1;
            std::cout << "Thread " << tid << " iniciou a mensagem com valor: " << mensagem << std::endl;
        }
        #pragma omp barrier
        if (tid > 0) {
            mensagem += tid;
            std::cout << "Thread " << tid << " recebeu mensagem: " << mensagem << std::endl;
        }
    }
    return 0;
}
