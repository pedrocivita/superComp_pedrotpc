#include <iostream>   
#include <fstream>    
#include <vector>     

// Função que lê números de um arquivo e os armazena em um vetor.
void lerArquivo(const std::string& nomeArquivo, std::vector<int>& numeros) {
    std::ifstream arquivoEntrada(nomeArquivo);
    if (!arquivoEntrada) {
        std::cerr << "Erro ao abrir o arquivo " << nomeArquivo << std::endl;
        return;
    }

    int numero;
    while (arquivoEntrada >> numero) {
        numeros.push_back(numero);
    }

    arquivoEntrada.close();
}

// Função que dobra os valores de um vetor de números, passando o vetor original por referência.
void dobrarValoresReferencia(const std::vector<int>& numerosOriginais, std::vector<int>& numerosDobrados) {
    numerosDobrados.clear();
    for (int num : numerosOriginais) {
        numerosDobrados.push_back(num * 2);
    }
}

// Função que dobra os valores de um vetor de números, passando o vetor original por ponteiro.
void dobrarValoresPonteiro(const std::vector<int>* numerosOriginais, std::vector<int>* numerosDobrados) {
    numerosDobrados->clear();
    for (int num : *numerosOriginais) {
        numerosDobrados->push_back(num * 2);
    }
}

// Função que escreve os números de um vetor em um arquivo.
void escreverArquivo(const std::string& nomeArquivo, const std::vector<int>& numeros) {
    std::ofstream arquivoSaida(nomeArquivo);
    if (!arquivoSaida) {
        std::cerr << "Erro ao criar o arquivo " << nomeArquivo << std::endl;
        return;
    }

    for (int num : numeros) {
        arquivoSaida << num << std::endl;
    }

    arquivoSaida.close();
}

int main() {
    std::vector<int> numeros, numerosDobrados;

    // Lê os números do arquivo "entrada.txt"
    lerArquivo("entrada.txt", numeros);

    // Dobra os valores usando passagem por referência
    dobrarValoresReferencia(numeros, numerosDobrados);
    escreverArquivo("saida_referencia.txt", numerosDobrados);

    // Dobra os valores usando passagem por ponteiro
    dobrarValoresPonteiro(&numeros, &numerosDobrados);
    escreverArquivo("saida_ponteiro.txt", numerosDobrados);

    return 0;
}
