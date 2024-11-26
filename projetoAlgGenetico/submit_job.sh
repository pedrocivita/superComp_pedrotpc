#!/bin/bash
#SBATCH --job-name=mochila_genetica    # Nome do job
#SBATCH --output=output_%x_%j.txt      # Nome do arquivo de saída baseado no nome do job
#SBATCH --ntasks=1                     # Número de tarefas
#SBATCH --cpus-per-task=1              # Número de CPUs por tarefa
#SBATCH --mem=4096                     # Memória alocada (4GB)
#SBATCH --time=01:00:00                # Tempo máximo de execução
#SBATCH --partition=espec              # Partição utilizada

# Carrega módulos necessários (se aplicável)
# module load gcc/9.2.0

# Compila o programa
# g++ -std=c++11 mochila_genetica.cpp -o mochila_genetica

# Executa o programa e grava a saída em um arquivo
./mochila_genetica > output_mochila_genetica_$SLURM_JOB_ID.txt