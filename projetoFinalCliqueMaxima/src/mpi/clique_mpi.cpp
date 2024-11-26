// clique_mpi_corrigido.cpp
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <set>
#include <algorithm>
#include <chrono>
#include <mpi.h>

const int MAX_DEPTH = 1000;  // Limite de profundidade para evitar recursão infinita

std::vector<std::vector<int>> LerGrafoListaAdj(const std::string& nomeArquivo, int& numVertices, int& numArestas, int rank) {
    std::vector<std::vector<int>> grafo;

    if (rank == 0) {
        std::ifstream arquivo(nomeArquivo);
        if (!arquivo.is_open()) {
            std::cerr << "Erro ao abrir o arquivo " << nomeArquivo << std::endl;
            MPI_Abort(MPI_COMM_WORLD, 1);
        }

        arquivo >> numVertices >> numArestas;

        std::cout << "Número de vértices: " << numVertices << std::endl;
        std::cout << "Número de arestas: " << numArestas << std::endl;

        grafo.resize(numVertices);

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
    }

    // Broadcast do número de vértices e arestas
    MPI_Bcast(&numVertices, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&numArestas, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // Broadcast do grafo
    if (rank != 0) {
        grafo.resize(numVertices);
    }

    // Enviar cada lista de adjacência
    for (int i = 0; i < numVertices; ++i) {
        int tamanho_lista = 0;
        if (rank == 0) {
            tamanho_lista = grafo[i].size();
        }
        MPI_Bcast(&tamanho_lista, 1, MPI_INT, 0, MPI_COMM_WORLD);

        if (rank != 0) {
            grafo[i].resize(tamanho_lista);
        }

        if (tamanho_lista > 0) {
            MPI_Bcast(grafo[i].data(), tamanho_lista, MPI_INT, 0, MPI_COMM_WORLD);
        }
    }

    return grafo;
}

void BronKerboschPivotMPI(std::vector<std::vector<int>>& grafo,
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

        BronKerboschPivotMPI(grafo, R_v, P_intersec, X_intersec, cliqueMaxima, depth + 1);

        P.erase(v);
        X.insert(v);
    }
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);  // Inicializa o MPI
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);  // Rank do processo atual
    MPI_Comm_size(MPI_COMM_WORLD, &size);  // Número total de processos

    std::string nomeArquivo = "../data/grafo.txt";
    int numVertices, numArestas;
    std::vector<std::vector<int>> grafo = LerGrafoListaAdj(nomeArquivo, numVertices, numArestas, rank);

    std::vector<int> cliqueMaximaLocal;

    // Todos os processos iniciam com P contendo todos os vértices
    std::set<int> P;
    for (int i = 0; i < numVertices; ++i) {
        P.insert(i);
    }
    std::set<int> R;
    std::set<int> X;

    // Dividir os vértices iniciais entre os processos
    std::vector<int> vertices_iniciais;
    for (int v = 0; v < numVertices; ++v) {
        if (v % size == rank) {
            vertices_iniciais.push_back(v);
        }
    }

    auto inicio = std::chrono::high_resolution_clock::now();

    // Cada processo executa Bron-Kerbosch para seus vértices iniciais
    for (int v : vertices_iniciais) {
        std::set<int> R_v = R;
        R_v.insert(v);

        std::set<int> P_v;
        for (int u : grafo[v]) {
            if (P.find(u) != P.end()) {
                P_v.insert(u);
            }
        }

        std::set<int> X_v;
        // X está vazio neste ponto

        std::vector<int> cliqueMaximaParcial;
        BronKerboschPivotMPI(grafo, R_v, P_v, X_v, cliqueMaximaParcial);

        if (cliqueMaximaParcial.size() > cliqueMaximaLocal.size()) {
            cliqueMaximaLocal = cliqueMaximaParcial;
        }

        P.erase(v);
        X.insert(v);
    }

    auto fim = std::chrono::high_resolution_clock::now();

    // Encontrar o tamanho máximo da clique e o rank do processo que o possui
    int tamanhoCliqueLocal = cliqueMaximaLocal.size();

    struct {
        int tamanho;
        int rank;
    } localData, globalData;

    localData.tamanho = tamanhoCliqueLocal;
    localData.rank = rank;

    // Usar MPI_Allreduce com MPI_MAXLOC para encontrar o tamanho máximo e o rank correspondente
    MPI_Allreduce(&localData, &globalData, 1, MPI_2INT, MPI_MAXLOC, MPI_COMM_WORLD);

    // O processo que possui a clique máxima define cliqueMaximaGlobal
    std::vector<int> cliqueMaximaGlobal(globalData.tamanho);
    if (rank == globalData.rank) {
        cliqueMaximaGlobal = cliqueMaximaLocal;
    }

    // Broadcast da clique máxima a partir do processo root (globalData.rank)
    MPI_Bcast(cliqueMaximaGlobal.data(), globalData.tamanho, MPI_INT, globalData.rank, MPI_COMM_WORLD);

    // Ordenar a clique máxima para exibição consistente
    std::sort(cliqueMaximaGlobal.begin(), cliqueMaximaGlobal.end());

    if (rank == 0) {
        std::chrono::duration<double> duracao = fim - inicio;
        std::cout << "\nClique máxima encontrada:" << std::endl;
        std::cout << "Tamanho: " << globalData.tamanho << std::endl;
        std::cout << "Vértices: ";
        for (int v : cliqueMaximaGlobal) {
            std::cout << v + 1 << " ";  // Ajuste para índices começando em 1
        }
        std::cout << std::endl;
        std::cout << "Tempo de execução: " << duracao.count() << " segundos." << std::endl;
    }

    MPI_Finalize();  // Finaliza o MPI
    return 0;
}
