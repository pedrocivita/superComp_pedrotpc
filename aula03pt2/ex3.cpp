// #include <iostream>
// #include <chrono>

// int main() {
//     int N = 1000;  // Define o tamanho da matriz como N x N

//     auto inicio = std::chrono::high_resolution_clock::now();

//     // Alocação dinâmica de memória para uma matriz N x N
//     int** matriz = new int*[N];
//     for (int i = 0; i < N; ++i) {
//         matriz[i] = new int[N];
//     }

//     // Inicializa e realiza a soma de todos os elementos da matriz
//     int soma = 0;
//     for (int i = 0; i < N; ++i) {
//         for (int j = 0; j < N; ++j) {
//             matriz[i][j] = i + j;
//             soma += matriz[i][j];
//         }
//     }

//     auto fim = std::chrono::high_resolution_clock::now();
//     std::chrono::duration<double> duracao = fim - inicio;

//     // Liberação da memória alocada dinamicamente
//     for (int i = 0; i < N; ++i) {
//         delete[] matriz[i];
//     }
//     delete[] matriz;

//     std::cout << "Soma: " << soma << std::endl;
//     std::cout << "Tempo de execução (alocação manual): " << duracao.count() << " segundos" << std::endl;

//     return 0;
// }

#include <iostream>
#include <vector>
#include <chrono>

int main() {
    int N = 1000;  // Define o tamanho da matriz como N x N

    auto inicio = std::chrono::high_resolution_clock::now();

    // Utiliza std::vector para alocar a matriz N x N
    std::vector<std::vector<int>> matriz(N, std::vector<int>(N));

    // Inicializa e realiza a soma de todos os elementos da matriz
    int soma = 0;
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            matriz[i][j] = i + j;
            soma += matriz[i][j];
        }
    }

    auto fim = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duracao = fim - inicio;

    std::cout << "Soma: " << soma << std::endl;
    std::cout << "Tempo de execução (std::vector): " << duracao.count() << " segundos" << std::endl;

    return 0;
}
