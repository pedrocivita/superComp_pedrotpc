#include <iostream>
#include <string>

int main() {
    std::string str;
    std::cout << "Digite uma string: ";
    std::getline(std::cin, str);

    std::cout << "A string tem " << str.length() << " caracteres." << std::endl;

    return 0;
}