#!/bin/bash
#SBATCH --job-name=mochila
#SBATCH --output=mochila_%j.txt
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=1
#SBATCH --mem=1024
#SBATCH --time=00:18:00
#SBATCH --partition=espec

echo "Rodando o Problema da Mochila com Hill Climbing"
echo " "

# Hill Climbing - 10 Execuções
echo "================ Hill Climbing ================"
for i in {1..10}
do
  echo "Execução $i com Entrada_1.txt:"
  ./mochilaHill Entrada_1.txt
  echo " "
done

for i in {1..10}
do
  echo "Execução $i com Entrada_2.txt:"
  ./mochilaHill Entrada_2.txt
  echo " "
done

for i in {1..10}
do
  echo "Execução $i com Entrada_3.txt:"
  ./mochilaHill Entrada_3.txt
  echo " "
done

echo "================ Fim das Execuções ================"
