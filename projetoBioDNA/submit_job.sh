#!/bin/bash
#SBATCH --job-name=mpi_exercises
#SBATCH --output=output_%x_%j.txt
#SBATCH --ntasks=4
#SBATCH --cpus-per-task=1
#SBATCH --mem=4096
#SBATCH --time=03:00:00
#SBATCH --partition=espec

# module load mpi
# module load gcc

# Compilação
# mpic++ -fopenmp ex1.cpp -o ex1
# mpic++ -fopenmp ex2.cpp -o ex2
# mpic++ -fopenmp ex3.cpp -o ex3
# mpic++ -fopenmp ex4.cpp -o ex4

# Execução
mpirun -np $SLURM_NTASKS ./ex1 > output_ex1_$SLURM_JOB_ID.txt
mpirun -np $SLURM_NTASKS ./ex2 > output_ex2_$SLURM_JOB_ID.txt
mpirun -np $SLURM_NTASKS ./ex3 > output_ex3_$SLURM_JOB_ID.txt
mpirun -np $SLURM_NTASKS ./ex4 > output_ex4_$SLURM_JOB_ID.txt