#include <iostream>

int main() {
    int vetor[10], maior;
    std::cout << "Digite 10 números: ";
    for(int i = 0; i < 10; i++) {
        std::cin >> vetor[i];
        if(i == 0 || vetor[i] > maior)
            maior = vetor[i];
    }

    std::cout << "O maior número é: " << maior << std::endl;

    return 0;
}
