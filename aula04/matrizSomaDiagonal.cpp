#include <iostream>

int main() {
    int matriz[3][3], soma = 0;
    std::cout << "Digite os elementos da matriz 3x3:" << std::endl;
    for(int i = 0; i < 3; i++) {
        for(int j = 0; j < 3; j++) {
            std::cin >> matriz[i][j];
            if(i == j) {
                soma += matriz[i][j];
            }
        }
    }

    std::cout << "A soma dos elementos na diagonal principal é: " << soma << std::endl;

    return 0;
}
