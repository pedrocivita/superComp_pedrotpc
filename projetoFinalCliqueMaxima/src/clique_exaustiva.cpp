// clique_exaustiva.cpp
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <set>
#include <algorithm>
#include <chrono>

const int MAX_DEPTH = 1000;  // Limite de profundidade para evitar recursão infinita

std::vector<std::vector<int>> LerGrafoListaAdj(const std::string& nomeArquivo, int& numVertices) {
    std::ifstream arquivo(nomeArquivo);
    if (!arquivo.is_open()) {
        std::cerr << "Erro ao abrir o arquivo " << nomeArquivo << std::endl;
        exit(1);
    }

    int numArestas;
    arquivo >> numVertices >> numArestas;

    std::cout << "Número de vértices: " << numVertices << std::endl;
    std::cout << "Número de arestas: " << numArestas << std::endl;

    std::vector<std::vector<int>> grafo(numVertices);

    int u, v;
    while (arquivo >> u >> v) {
        if (u < 1 || v < 1 || u > numVertices || v > numVertices) {
            std::cerr << "Aresta inválida: " << u << " - " << v << std::endl;
            continue;
        }
        grafo[u - 1].push_back(v - 1);
        grafo[v - 1].push_back(u - 1);  // Grafo não direcionado
    }

    arquivo.close();
    return grafo;
}

void BronKerboschPivot(std::vector<std::vector<int>>& grafo,
                       std::set<int> R,
                       std::set<int> P,
                       std::set<int> X,
                       std::vector<int>& cliqueMaxima,
                       int depth = 0) {
    if (depth > MAX_DEPTH) {
        return;
    }

    if (P.empty() && X.empty()) {
        if (R.size() > cliqueMaxima.size()) {
            cliqueMaxima.assign(R.begin(), R.end());
        }
        return;
    }

    // Escolher pivô u a partir de P ∪ X
    int u = -1;
    int maxGrau = -1;
    std::set<int> unionPX;
    unionPX.insert(P.begin(), P.end());
    unionPX.insert(X.begin(), X.end());

    if (!unionPX.empty()) {
        for (int v : unionPX) {
            if (v >= 0 && v < grafo.size()) {
                int grau = grafo[v].size();
                if (grau > maxGrau) {
                    maxGrau = grau;
                    u = v;
                }
            }
        }
    } else {
        u = 0;  // Se P ∪ X estiver vazio, escolhemos um pivô arbitrário
    }

    if (u == -1 || u >= grafo.size()) {
        return;
    }

    // P \ N(u)
    std::set<int> P_sem_Nu;
    for (int v : P) {
        if (std::find(grafo[u].begin(), grafo[u].end(), v) == grafo[u].end()) {
            P_sem_Nu.insert(v);
        }
    }

    for (int v : P_sem_Nu) {
        std::set<int> R_v = R;
        R_v.insert(v);

        // Intersecção de P e N(v)
        std::set<int> P_intersec;
        for (int u2 : grafo[v]) {
            if (P.find(u2) != P.end()) {
                P_intersec.insert(u2);
            }
        }

        // Intersecção de X e N(v)
        std::set<int> X_intersec;
        for (int u2 : grafo[v]) {
            if (X.find(u2) != X.end()) {
                X_intersec.insert(u2);
            }
        }

        BronKerboschPivot(grafo, R_v, P_intersec, X_intersec, cliqueMaxima, depth + 1);

        P.erase(v);
        X.insert(v);
    }
}

int main() {
    std::string nomeArquivo = "../data/grafo.txt";
    int numVertices;
    std::vector<std::vector<int>> grafo = LerGrafoListaAdj(nomeArquivo, numVertices);

    std::set<int> R;
    std::set<int> P;
    std::set<int> X;
    std::vector<int> cliqueMaxima;

    // Inicializar P com todos os vértices
    for (int i = 0; i < numVertices; ++i) {
        P.insert(i);
    }

    auto inicio = std::chrono::high_resolution_clock::now();
    BronKerboschPivot(grafo, R, P, X, cliqueMaxima);
    auto fim = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> duracao = fim - inicio;

    // Ordenar a clique máxima para exibição consistente
    std::sort(cliqueMaxima.begin(), cliqueMaxima.end());

    std::cout << "\nClique máxima encontrada:" << std::endl;
    std::cout << "Tamanho: " << cliqueMaxima.size() << std::endl;
    std::cout << "Vértices: ";
    for (int v : cliqueMaxima) {
        std::cout << v + 1 << " ";  // Ajuste para índices começando em 1
    }
    std::cout << std::endl;
    std::cout << "Tempo de execução: " << duracao.count() << " segundos." << std::endl;

    return 0;
}
