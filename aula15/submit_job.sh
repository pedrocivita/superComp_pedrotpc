#!/bin/bash
#SBATCH --job-name=ex1          # Nome do job
#SBATCH --output=ex1_%j.txt          # Nome do arquivo de saída
#SBATCH --ntasks=4
#SBATCH --cpus-per-task=1
#SBATCH --mem=4096
#SBATCH --time=03:00:00
#SBATCH --partition=espec

mpirun -np 4 ./ex1