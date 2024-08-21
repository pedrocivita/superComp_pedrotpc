#include <iostream>

int main() {
    double num1, num2;
    char op;
    std::cout << "Digite dois números: ";
    std::cin >> num1 >> num2;
    std::cout << "Digite a operação (+, -, *, /): ";
    std::cin >> op;

    switch(op) {
        case '+': std::cout << "Resultado: " << num1 + num2 << std::endl; break;
        case '-': std::cout << "Resultado: " << num1 - num2 << std::endl; break;
        case '*': std::cout << "Resultado: " << num1 * num2 << std::endl; break;
        case '/': 
            if(num2 != 0)
                std::cout << "Resultado: " << num1 / num2 << std::endl; 
            else 
                std::cout << "Erro: Divisão por zero!" << std::endl;
            break;
        default: std::cout << "Operação inválida!" << std::endl;
    }

    return 0;
}
