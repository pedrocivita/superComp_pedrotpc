#include <iostream>
#include <vector>
#include <chrono> // Biblioteca para medir o tempo de execução

// Função de Fibonacci usando Recursão Pura
long long fib(int n) {
    if (n <= 1) {
        return n;
    } else {
        return fib(n - 1) + fib(n - 2);
    }
}

// Função de Fibonacci usando Memoization
long long fib_memo(int n, std::vector<long long>& memo) {
    if (n <= 1) {
        return n;
    }
    if (memo[n] != -1) {
        return memo[n];
    }
    memo[n] = fib_memo(n - 1, memo) + fib_memo(n - 2, memo);
    return memo[n];
}

// Função para calcular Fibonacci e medir os tempos de execução
void calcular_fibonacci(int n) {
    // Medir tempo para Recursão Pura
    auto start_pura = std::chrono::high_resolution_clock::now();
    long long result_pura = fib(n);
    auto end_pura = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration_pura = end_pura - start_pura;

    std::cout << "Fibonacci de " << n << " = " << result_pura << " (Recursao Pura)" << std::endl;
    std::cout << "Tempo de execucao (Recursao Pura): " << duration_pura.count() << " segundos" << std::endl;

    // Medir tempo para Memoization
    std::vector<long long> memo(n + 1, -1);
    auto start_memo = std::chrono::high_resolution_clock::now();
    long long result_memo = fib_memo(n, memo);
    auto end_memo = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration_memo = end_memo - start_memo;

    std::cout << "Fibonacci de " << n << " = " << result_memo << " (Memoization)" << std::endl;
    std::cout << "Tempo de execucao (Memoization): " << duration_memo.count() << " segundos" << std::endl;
    std::cout << "---------------------------------------------" << std::endl;
}

int main() {
    // Lista de valores fixos, começando em 6 e aumentando de 6 em 6 até 48
    std::vector<int> valores = {6, 12, 18, 24, 30, 36, 42, 48};

    // Para cada valor na lista, calcular o Fibonacci usando ambos os métodos
    for (int n : valores) {
        calcular_fibonacci(n);
    }

    return 0;
}
