// clique_exaustiva.cpp
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <algorithm>
#include <chrono>

std::vector<std::vector<int>> LerGrafo(const std::string& nomeArquivo, int& numVertices) {
    std::ifstream arquivo(nomeArquivo);
    int numArestas;
    arquivo >> numVertices >> numArestas;

    std::vector<std::vector<int>> grafo(numVertices, std::vector<int>(numVertices, 0));

    for (int i = 0; i < numArestas; ++i) {
        int u, v;
        arquivo >> u >> v;
        grafo[u - 1][v - 1] = 1;
        grafo[v - 1][u - 1] = 1;  // O grafo é não direcionado
    }

    arquivo.close();
    return grafo;
}

void BronKerbosch(std::vector<std::vector<int>>& grafo,
                  std::vector<int>& R,
                  std::vector<int>& P,
                  std::vector<int>& X,
                  std::vector<int>& cliqueMaxima) {
    if (P.empty() && X.empty()) {
        if (R.size() > cliqueMaxima.size()) {
            cliqueMaxima = R;
        }
        return;
    }

    std::vector<int> P_copy = P;  // Copia de P para iteração
    for (int v : P_copy) {
        std::vector<int> R_v = R;
        R_v.push_back(v);

        // Intersecção de P e N(v)
        std::vector<int> P_intersec;
        for (int u : P) {
            if (grafo[v][u]) {
                P_intersec.push_back(u);
            }
        }

        // Intersecção de X e N(v)
        std::vector<int> X_intersec;
        for (int u : X) {
            if (grafo[v][u]) {
                X_intersec.push_back(u);
            }
        }

        BronKerbosch(grafo, R_v, P_intersec, X_intersec, cliqueMaxima);

        // Remover v de P e adicionar em X
        P.erase(std::remove(P.begin(), P.end(), v), P.end());
        X.push_back(v);
    }
}

int main() {
    std::string nomeArquivo = "data/grafo.txt";
    int numVertices;
    std::vector<std::vector<int>> grafo = LerGrafo(nomeArquivo, numVertices);

    std::vector<int> R;
    std::vector<int> P(numVertices);
    std::vector<int> X;
    std::vector<int> cliqueMaxima;

    // Inicializar P com todos os vértices
    for (int i = 0; i < numVertices; ++i) {
        P[i] = i;
    }

    auto inicio = std::chrono::high_resolution_clock::now();
    BronKerbosch(grafo, R, P, X, cliqueMaxima);
    auto fim = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> duracao = fim - inicio;

    std::cout << "Tamanho da clique máxima: " << cliqueMaxima.size() << std::endl;
    std::cout << "Vértices da clique máxima: ";
    for (int v : cliqueMaxima) {
        std::cout << v + 1 << " ";  // Ajuste para índices começando em 1
    }
    std::cout << std::endl;
    std::cout << "Tempo de execução: " << duracao.count() << " segundos." << std::endl;

    return 0;
}
