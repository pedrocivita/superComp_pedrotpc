#include <iostream>

int main() {
    int num;
    std::cout << "Digite um número: ";
    std::cin >> num;

    if(num % 2 == 0)
        std::cout << "O número é par." << std::endl;
    else
        std::cout << "O número é ímpar." << std::endl;

    return 0;
}
