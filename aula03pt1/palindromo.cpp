#include <iostream>
#include <string>

int main() {
    std::string str, rev_str;
    std::cout << "Digite uma string: ";
    std::cin >> str;

    rev_str = std::string(str.rbegin(), str.rend());

    if(str == rev_str)
        std::cout << "A string é um palíndromo." << std::endl;
    else
        std::cout << "A string não é um palíndromo." << std::endl;

    return 0;
}
