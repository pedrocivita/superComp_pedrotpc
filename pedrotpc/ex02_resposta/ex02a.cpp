#include <iostream>
#include <omp.h>

long long factorial(int n) {
    long long result = 1;

    // Paraleliza o loop com redução multiplicativa
    #pragma omp parallel for reduction(*:result)
    for (int i = 1; i <= n; ++i) {
        result *= i;
    }

    return result;
}

int main() {
    int n = 10;
    std::cout << "Fatorial de " << n << " é " << factorial(n) << std::endl;
    return 0;
}
