#!/bin/bash
#SBATCH --job-name=leve
#SBATCH --output=leve_%j.txt
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=1
#SBATCH --mem=1024
#SBATCH --time=00:18:00
#SBATCH --partition=espec

echo "Rodando o Problema da Mochila com mais_leve"
echo " "

# mais_leve - 10 Execuções
echo "================ mais_leve ================"
for i in {1..3}
do
  echo "Execução $i com Entrada_1.txt:"
  ./mais_leve Entrada_1.txt
  echo " "
done

for i in {1..3}
do
  echo "Execução $i com Entrada_2.txt:"
  ./mais_leve Entrada_2.txt
  echo " "
done

for i in {1..3}
do
  echo "Execução $i com Entrada_3.txt:"
  ./mais_leve Entrada_3.txt
  echo " "
done

echo "================ Fim das Execuções ================"
