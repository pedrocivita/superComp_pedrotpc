#!/bin/bash
#SBATCH --job-name=clique_project    # Nome do job
#SBATCH --output=output_%x_%j.txt    # Arquivo de saída baseado no nome do job e ID
#SBATCH --ntasks=4                   # Número total de tarefas (processos MPI)
#SBATCH --cpus-per-task=1            # Número de CPUs por tarefa
#SBATCH --mem=4096                   # Memória alocada (4GB)
#SBATCH --time=03:00:00              # Tempo máximo de execução
#SBATCH --partition=espec            # Partição utilizada

# Carrega os módulos necessários
# module load gcc
# module load openmpi

# Compila os programas
# mpic++ clique_mpi.cpp -o clique_mpi
# g++ -fopenmp clique_openmp.cpp -o clique_openmp
# g++ clique_exaustiva.cpp -o clique_exaustiva

# Executa a implementação com MPI
mpirun -np $SLURM_NTASKS ./clique_mpi > output_clique_mpi_$SLURM_JOB_ID.txt

# Executa a implementação com OpenMP
./clique_openmp > output_clique_openmp_$SLURM_JOB_ID.txt

# Executa a implementação exaustiva
./clique_exaustiva > output_clique_exaustiva_$SLURM_JOB_ID.txt
