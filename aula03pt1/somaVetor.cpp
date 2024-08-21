#include <iostream>

int main() {
    int vetor[5], soma = 0;
    std::cout << "Digite 5 números: ";
    for(int i = 0; i < 5; i++) {
        std::cin >> vetor[i];
        soma += vetor[i];
    }

    std::cout << "A soma dos números é: " << soma << std::endl;

    return 0;
}
