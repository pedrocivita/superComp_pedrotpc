#!/bin/bash
#SBATCH --job-name=ex04                  # Nome do job
#SBATCH --output=output_%x_%j.txt        # Arquivo de saída baseado no nome do job e ID
#SBATCH --ntasks=4                       # Número total de tarefas (processos MPI)
#SBATCH --cpus-per-task=5                # Número de CPUs por tarefa
#SBATCH --mem=4096                       # Memória alocada (4GB)
#SBATCH --time=03:00:00                  # Tempo máximo de execução
#SBATCH --partition=espec                # Partição utilizada

# Carrega os módulos necessários
# module load gcc
# module load openmpi

# Define o número de threads para OpenMP
export OMP_NUM_THREADS=$SLURM_CPUS_PER_TASK

# Compila o programa com MPI e OpenMP
# mpic++ -fopenmp ex04.cpp -o ex04

# Executa a implementação com MPI
mpirun -np $SLURM_NTASKS ./ex04 > output_ex04_$SLURM_JOB_ID.txt
