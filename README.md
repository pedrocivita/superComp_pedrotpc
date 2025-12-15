# High-Performance Computing Projects

[![C++](https://img.shields.io/badge/C++-00599C?style=for-the-badge&logo=c%2B%2B&logoColor=white)](https://isocpp.org/)
[![OpenMP](https://img.shields.io/badge/OpenMP-0078D4?style=for-the-badge)](https://www.openmp.org/)
[![MPI](https://img.shields.io/badge/MPI-0078D4?style=for-the-badge)](https://www.mpi-forum.org/)
[![SLURM](https://img.shields.io/badge/SLURM-0A0A0A?style=for-the-badge)](https://slurm.schedmd.com/)
[![Linux](https://img.shields.io/badge/Linux-FCC624?style=for-the-badge&logo=linux&logoColor=black)](https://www.linux.org/)

A comprehensive collection of high-performance computing (HPC) projects and exercises developed during the Supercomputing course at Insper - Instituto de Ensino e Pesquisa. This repository demonstrates practical applications of parallel computing techniques, optimization algorithms, and distributed systems.

## About

This repository contains coursework and projects focusing on parallel and distributed computing using modern HPC technologies. The projects explore various optimization techniques, from basic parallelization to advanced algorithms for solving NP-hard problems using shared and distributed memory architectures.

**Institution:** Insper - Instituto de Ensino e Pesquisa  
**Program:** Computer Engineering  
**Course:** Supercomputing

## Key Projects

### 1. Bioinformatics DNA Analysis (projetoBioDNA)

A parallel implementation for analyzing genomic sequences from human chromosomes (chr1-chr22).

**Technologies:** C++, OpenMP, MPI

**Features:**
- Base nucleotide frequency analysis
- DNA to RNA transcription
- Protein initiation site detection (AUG codon)
- RNA to protein translation
- Parallel processing of genomic data

**Performance:** Processes over 2.5 billion nucleotides with significant speedup using hybrid MPI+OpenMP parallelization.

[View Full Report](projetoBioDNA/relatorio.md)

### 2. Maximum Clique Problem (projetoFinalCliqueMaxima)

Implementation of algorithms to find the maximum clique in a graph, a classic NP-hard problem in graph theory and social network analysis.

**Technologies:** C++, OpenMP, MPI

**Implementations:**
- Exhaustive search using Bron-Kerbosch algorithm with pivoting
- OpenMP parallelization for shared memory systems
- MPI parallelization for distributed memory systems

**Results:** Successfully identified maximum cliques in graphs with 100+ vertices and thousands of edges.

[View Full Report](projetoFinalCliqueMaxima/relatorio.md)

### 3. Genetic Algorithm for Knapsack Problem (projetoAlgGenetico)

Implementation of a genetic algorithm to solve the classic knapsack optimization problem.

**Technologies:** C++, Genetic Algorithms

**Features:**
- Population-based evolutionary optimization
- Selection, crossover, and mutation operators
- Fitness evaluation for knapsack constraints
- Convergence analysis

## Repository Structure

```
.
├── aula03pt1/          # Introduction to C++ programming
├── aula03pt2/          # Advanced C++ concepts
├── aula06/             # Sequential algorithms
├── aula07/             # Algorithm optimization
├── aula08/             # Greedy algorithms
├── aula09/             # Dynamic programming
├── aula10/             # Heuristics and approximation
├── aula11/             # Random algorithms
├── aula12/             # Advanced optimization
├── aula14/             # GPU computing introduction
├── aula15/             # OpenMP parallelization
├── aula16/             # Advanced OpenMP
├── aula17/             # MPI distributed computing
├── avaliacaoIntermediaria_pedrotpc/  # Midterm evaluation
├── pedrotpcFinal/      # Final evaluation
├── projetoAlgGenetico/ # Genetic algorithm project
├── projetoBioDNA/      # Bioinformatics project
└── projetoFinalCliqueMaxima/  # Maximum clique project
```

## Technologies & Tools

- **Programming Language:** C++17
- **Parallel Computing:**
  - OpenMP (Open Multi-Processing) - Shared memory parallelization
  - MPI (Message Passing Interface) - Distributed memory parallelization
- **Job Scheduling:** SLURM Workload Manager
- **Development Environment:** Linux-based HPC clusters
- **Build Tools:** g++, mpic++
- **Data Processing:** Large-scale genomic data, graph algorithms

## Learning Outcomes

Throughout this course, the following competencies were developed:

1. **Parallel Programming:** Understanding and implementing parallel algorithms using OpenMP and MPI
2. **Performance Optimization:** Analyzing and improving computational efficiency
3. **Algorithm Design:** Implementing and optimizing various algorithmic approaches:
   - Greedy algorithms
   - Dynamic programming
   - Heuristics and approximation algorithms
   - Evolutionary algorithms
   - Graph algorithms
4. **Distributed Systems:** Working with multi-node computing clusters
5. **Real-World Applications:** Solving practical problems in bioinformatics and network analysis

## Installation & Usage

### Prerequisites

```bash
# C++ compiler with C++17 support
g++ --version

# OpenMP support (usually included with g++)
# MPI implementation (OpenMPI or MPICH)
mpirun --version

# SLURM (for cluster execution)
sbatch --version
```

### Compilation Examples

**Sequential C++ program:**
```bash
g++ -std=c++17 -o program program.cpp
./program
```

**OpenMP program:**
```bash
g++ -std=c++17 -fopenmp -o program_omp program.cpp
./program_omp
```

**MPI program:**
```bash
mpic++ -std=c++17 -o program_mpi program.cpp
mpirun -np 4 ./program_mpi
```

**Hybrid MPI+OpenMP:**
```bash
mpic++ -std=c++17 -fopenmp -o program_hybrid program.cpp
mpirun -np 2 ./program_hybrid
```

### Running on SLURM Cluster

```bash
sbatch submit_job.sh
```

## Performance Highlights

| Project | Problem Size | Parallelization | Speedup |
|---------|-------------|-----------------|---------|
| BioDNA Analysis | 2.5B nucleotides | MPI + OpenMP | 4x-8x |
| Maximum Clique | 100 vertices, 3453 edges | OpenMP | 3x-4x |
| Maximum Clique | 100 vertices, 3453 edges | MPI | 2x-3x |

## Author

**Pedro Toledo Piza Civita**

- Email: pedrotpc@al.insper.edu.br
- LinkedIn: [linkedin.com/in/pedrocivita](https://linkedin.com/in/pedrocivita)
- GitHub: [@pedrocivita](https://github.com/pedrocivita)

## License

This repository contains academic coursework. All code is provided for educational purposes.

## Acknowledgments

- Insper - Instituto de Ensino e Pesquisa
- Supercomputing Course Faculty
- High-Performance Computing community for tools and libraries