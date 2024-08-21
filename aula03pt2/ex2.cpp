#include <iostream>
#include <vector>

// Classe Matriz para representar uma matriz quadrada
class Matriz {
public:
    // Construtor que inicializa uma matriz quadrada de dimensão N x N
    Matriz(int N) {
        data.resize(N, std::vector<int>(N, 0));  // Inicializa a matriz com zeros
    }

    // Método para acessar um elemento específico da matriz (leitura)
    // Use 'const' para garantir que este método não modifica a matriz
    int get(int i, int j) const {
        // Retorna o elemento na posição (i, j)
        return data[i][j];
    }

    // Método para modificar um elemento específico da matriz (escrita)
    void set(int i, int j, int valor) {
        // Modifica o elemento na posição (i, j)
        data[i][j] = valor;
    }

    // Método para imprimir a matriz
    void imprime() const {
        for (const auto& linha : data) {
            for (int valor : linha) {
                std::cout << valor << " ";
            }
            std::cout << std::endl;
        }
    }

    // Método para acessar o tamanho da matriz (número de linhas ou colunas)
    int tamanho() const {
        return data.size();
    }

private:
    std::vector<std::vector<int>> data;  // Armazena os elementos da matriz
};

// Função que multiplica duas matrizes A e B, armazenando o resultado em C
// Garanta que A e B são constantes para evitar modificações
void multiplicaMatriz(const Matriz& A, const Matriz& B, Matriz& C) {
    int N = A.tamanho();  // Supõe que as matrizes são quadradas e de mesma dimensão

    // Laço para realizar a multiplicação de matrizes
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            int soma = 0;
            for (int k = 0; k < N; k++) {
                // Multiplica os elementos correspondentes de A e B
                soma += A.get(i, k) * B.get(k, j);
            }
            // Armazena o resultado em C
            C.set(i, j, soma);
        }
    }
}

int main() {
    int N = 3;  // Define o tamanho da matriz (N x N)

    // Cria as matrizes: A, B, e C, todas de dimensão N x N
    Matriz A(N), B(N), C(N);

    // Exemplo de preenchimento das matrizes A e B
    A.set(0, 0, 1); A.set(0, 1, 2); A.set(0, 2, 3);
    A.set(1, 0, 4); A.set(1, 1, 5); A.set(1, 2, 6);
    A.set(2, 0, 7); A.set(2, 1, 8); A.set(2, 2, 9);

    B.set(0, 0, 9); B.set(0, 1, 8); B.set(0, 2, 7);
    B.set(1, 0, 6); B.set(1, 1, 5); B.set(1, 2, 4);
    B.set(2, 0, 3); B.set(2, 1, 2); B.set(2, 2, 1);

    // Multiplica as matrizes A e B, armazenando o resultado em C
    multiplicaMatriz(A, B, C);

    // Imprime a matriz resultante C
    C.imprime();

    return 0;  // Retorna 0, indicando que o programa terminou com sucesso
}
