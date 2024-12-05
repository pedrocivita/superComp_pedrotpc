#!/bin/bash
#SBATCH --job-name=ex03                   # Nome do job
#SBATCH --output=output_%x_%j.txt         # Arquivo de saída baseado no nome do job e ID
#SBATCH --ntasks=1                        # Número total de tarefas (processos MPI)
#SBATCH --cpus-per-task=8                 # Número de CPUs por tarefa
#SBATCH --mem=4096                        # Memória alocada (4GB)
#SBATCH --time=03:00:00                   # Tempo máximo de execução
#SBATCH --partition=espec                 # Partição utilizada

# Carrega os módulos necessários
# module load gcc

# Define o número de threads para OpenMP
export OMP_NUM_THREADS=$SLURM_CPUS_PER_TASK

# Compila o programa com OpenMP
# ++ -fopenmp ex03.cpp -o ex03

# Executa a implementação com OpenMP
./ex03 > output_ex03_$SLURM_JOB_ID.txt

