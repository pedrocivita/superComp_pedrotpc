// clique_mpi.cpp
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <algorithm>
#include <chrono>
#include <mpi.h>

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

void BronKerboschMPI(std::vector<std::vector<int>>& grafo,
                     std::vector<int> R,
                     std::vector<int> P,
                     std::vector<int> X,
                     std::vector<int>& cliqueMaxima) {
    if (P.empty() && X.empty()) {
        if (R.size() > cliqueMaxima.size()) {
            cliqueMaxima = R;
        }
        return;
    }

    int u = -1;
    if (!P.empty()) {
        u = P[0];
    } else if (!X.empty()) {
        u = X[0];
    }

    std::vector<int> P_sem_Nu;
    for (int v : P) {
        if (u == -1 || !grafo[u][v]) {
            P_sem_Nu.push_back(v);
        }
    }

    for (size_t i = 0; i < P_sem_Nu.size(); ++i) {
        int v = P_sem_Nu[i];
        std::vector<int> R_v = R;
        R_v.push_back(v);

        // Intersecção de P e N(v)
        std::vector<int> P_intersec;
        for (int u2 : P) {
            if (grafo[v][u2]) {
                P_intersec.push_back(u2);
            }
        }

        // Intersecção de X e N(v)
        std::vector<int> X_intersec;
        for (int u2 : X) {
            if (grafo[v][u2]) {
                X_intersec.push_back(u2);
            }
        }

        BronKerboschMPI(grafo, R_v, P_intersec, X_intersec, cliqueMaxima);

        // Remover v de P e adicionar em X
        P.erase(std::remove(P.begin(), P.end(), v), P.end());
        X.push_back(v);
    }
}

int main(int argc, char** argv) {
    int rank, size;
    MPI_Init(&argc, &argv);                 // Inicializa o MPI
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);   // Rank do processo atual
    MPI_Comm_size(MPI_COMM_WORLD, &size);   // Número total de processos

    std::string nomeArquivo = "data/grafo.txt";
    int numVertices;
    std::vector<std::vector<int>> grafo;

    if (rank == 0) {
        grafo = LerGrafo(nomeArquivo, numVertices);
    }

    // Broadcast do número de vértices
    MPI_Bcast(&numVertices, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // Broadcast do grafo
    if (rank != 0) {
        grafo.resize(numVertices, std::vector<int>(numVertices, 0));
    }

    for (int i = 0; i < numVertices; ++i) {
        MPI_Bcast(grafo[i].data(), numVertices, MPI_INT, 0, MPI_COMM_WORLD);
    }

    // Divisão dos vértices entre os processos
    std::vector<int> R;
    std::vector<int> P;
    std::vector<int> X;
    std::vector<int> cliqueMaximaLocal;

    for (int i = rank; i < numVertices; i += size) {
        P.push_back(i);
    }

    auto inicio = std::chrono::high_resolution_clock::now();
    BronKerboschMPI(grafo, R, P, X, cliqueMaximaLocal);
    auto fim = std::chrono::high_resolution_clock::now();

    // Encontrar a maior clique entre todos os processos
    int tamanhoCliqueLocal = cliqueMaximaLocal.size();
    int tamanhoCliqueGlobal;

    MPI_Allreduce(&tamanhoCliqueLocal, &tamanhoCliqueGlobal, 1, MPI_INT, MPI_MAX, MPI_COMM_WORLD);

    // Processo que possui a clique máxima envia para os demais
    std::vector<int> cliqueMaximaGlobal(tamanhoCliqueGlobal);
    if (tamanhoCliqueLocal == tamanhoCliqueGlobal) {
        cliqueMaximaGlobal = cliqueMaximaLocal;
    }

    // Broadcast da clique máxima
    MPI_Bcast(cliqueMaximaGlobal.data(), tamanhoCliqueGlobal, MPI_INT, tamanhoCliqueLocal == tamanhoCliqueGlobal ? rank : MPI_PROC_NULL, MPI_COMM_WORLD);

    if (rank == 0) {
        std::chrono::duration<double> duracao = fim - inicio;
        std::cout << "Tamanho da clique máxima: " << tamanhoCliqueGlobal << std::endl;
        std::cout << "Vértices da clique máxima: ";
        for (int v : cliqueMaximaGlobal) {
            std::cout << v + 1 << " ";  // Ajuste para índices começando em 1
        }
        std::cout << std::endl;
        std::cout << "Tempo de execução: " << duracao.count() << " segundos." << std::endl;
    }

    MPI_Finalize();  // Finaliza o MPI
    return 0;
}
