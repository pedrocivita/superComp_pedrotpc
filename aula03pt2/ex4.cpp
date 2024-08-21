#include <iostream>
#include <stdexcept>

class Vector {
public:
    // Construtor que inicializa o vetor com uma capacidade inicial
    Vector(int tamanho);
    
    // Destrutor para liberar a memória alocada dinamicamente
    ~Vector();
    
    // Inicializa todos os elementos do vetor com um valor específico
    void inicializa(int valor);
    
    // Retorna o valor na posição especificada (leitura)
    int get(int index) const;
    
    // Modifica o valor na posição especificada (escrita)
    void set(int index, int valor);
    
    // Insere um novo valor na posição especificada, redimensionando se necessário
    void inserir(int valor);
    
    // Remove o elemento na posição especificada
    void remover(int index);
    
    // Imprime todos os elementos do vetor
    void imprime() const;

private:
    int* dados;          // Ponteiro para o array dinâmico que armazena os elementos
    int tam;             // Tamanho atual do vetor (número de elementos)
    int capacidade;      // Capacidade atual do vetor (espaço alocado)

    // Redimensiona o vetor para uma nova capacidade
    void redimensiona(int novaCapacidade);
};

// Implementação das funções da classe Vector

Vector::Vector(int tamanho) : tam(0), capacidade(tamanho) {
    dados = new int[capacidade];
}

Vector::~Vector() {
    delete[] dados;
}

void Vector::inicializa(int valor) {
    for (int i = 0; i < capacidade; ++i) {
        dados[i] = valor;
    }
    tam = capacidade;
}

int Vector::get(int index) const {
    if (index < 0 || index >= tam) {
        throw std::out_of_range("Índice fora do intervalo");
    }
    return dados[index];
}

void Vector::set(int index, int valor) {
    if (index < 0 || index >= tam) {
        throw std::out_of_range("Índice fora do intervalo");
    }
    dados[index] = valor;
}

void Vector::inserir(int valor) {
    if (tam == capacidade) {
        redimensiona(capacidade * 2);
    }
    dados[tam++] = valor;
}

void Vector::remover(int index) {
    if (index < 0 || index >= tam) {
        throw std::out_of_range("Índice fora do intervalo");
    }
    for (int i = index; i < tam - 1; ++i) {
        dados[i] = dados[i + 1];
    }
    --tam;
}

void Vector::imprime() const {
    for (int i = 0; i < tam; ++i) {
        std::cout << dados[i] << " ";
    }
    std::cout << std::endl;
}

void Vector::redimensiona(int novaCapacidade) {
    int* novoArray = new int[novaCapacidade];
    for (int i = 0; i < tam; ++i) {
        novoArray[i] = dados[i];
    }
    delete[] dados;
    dados = novoArray;
    capacidade = novaCapacidade;
}

int main() {
    Vector vec(5);
    vec.inicializa(0);
    vec.imprime();

    vec.set(2, 5);
    vec.imprime();

    vec.inserir(10);
    vec.imprime();

    vec.remover(1);
    vec.imprime();

    return 0;
}
