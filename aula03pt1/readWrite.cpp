#include <iostream>
#include <fstream>
#include <sstream>
#include <map>

int main() {
    std::ifstream arquivoEntrada("texto.txt");
    std::ofstream arquivoSaida("estatisticas.txt");

    if (!arquivoEntrada) {
        std::cerr << "Erro ao abrir o arquivo de entrada!" << std::endl;
        return 1;
    }

    std::string linha, palavra;
    int totalLinhas = 0, totalPalavras = 0;
    std::map<std::string, int> frequenciaPalavras;

    while (std::getline(arquivoEntrada, linha)) {
        totalLinhas++;
        std::istringstream stream(linha);
        while (stream >> palavra) {
            totalPalavras++;
            frequenciaPalavras[palavra]++;
        }
    }

    std::string palavraMaisFrequente;
    int maiorFrequencia = 0;
    for (const auto& par : frequenciaPalavras) {
        if (par.second > maiorFrequencia) {
            maiorFrequencia = par.second;
            palavraMaisFrequente = par.first;
        }
    }

    arquivoSaida << "Número total de linhas: " << totalLinhas << std::endl;
    arquivoSaida << "Número total de palavras: " << totalPalavras << std::endl;
    arquivoSaida << "Número médio de palavras por linha: " << (totalPalavras / totalLinhas) << std::endl;
    arquivoSaida << "Palavra mais frequente: " << palavraMaisFrequente << " (frequência: " << maiorFrequencia << ")" << std::endl;

    arquivoEntrada.close();
    arquivoSaida.close();

    std::cout << "Estatísticas geradas em 'estatisticas.txt'" << std::endl;

    return 0;
}
