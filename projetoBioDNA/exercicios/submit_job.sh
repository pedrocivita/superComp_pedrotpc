#!/bin/bash
#SBATCH --job-name=mpi_exercises    # Nome do job
#SBATCH --output=output_%x_%j.txt   # Nome do arquivo de saída baseado no nome do job
#SBATCH --ntasks=4                  # Número de tarefas por exercício
#SBATCH --cpus-per-task=1           # Número de CPUs por tarefa
#SBATCH --mem=4096                  # Memória alocada (4GB)
#SBATCH --time=03:00:00             # Tempo máximo de execução
#SBATCH --partition=espec           # Partição utilizada

# Compila cada exercício
# mpic++ ex1.cpp -o ex1
# mpic++ ex2.cpp -o ex2
# mpic++ ex3.cpp -o ex3
# mpic++ ex4.cpp -o ex4

# Executa cada exercício e grava a saída em um arquivo separado
# mpirun -np 4 ./ex1 > output_ex1_$SLURM_JOB_ID.txt
# mpirun -np 4 ./ex2 > output_ex2_$SLURM_JOB_ID.txt
# mpirun -np 4 ./ex3 > output_ex3_$SLURM_JOB_ID.txt
mpirun -np 4 ./ex4 > output_ex4_$SLURM_JOB_ID.txt
