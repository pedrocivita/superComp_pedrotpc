#!/bin/bash
#SBATCH --job-name=meu_job             # Nome do job
#SBATCH --output=output_%x_%j.txt      # Arquivo de saída baseado no nome do job e ID
#SBATCH --nodes=5                      # Número de nós de computação
#SBATCH --ntasks=5                     # Número total de tarefas (1 por nó)
#SBATCH --ntasks-per-node=1            # Número de tasks por nó
#SBATCH --cpus-per-task=4              # Número de CPUs por tarefa
#SBATCH --mem=16G                      # Memória alocada (16GB)
#SBATCH --time=01:00:00                # Tempo máximo de execução
#SBATCH --partition=prova              # Partição utilizada

# Define o número de threads para OpenMP
export OMP_NUM_THREADS=15

# Executa o programa
srun ./programa
