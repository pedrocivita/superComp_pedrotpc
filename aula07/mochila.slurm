#!/bin/bash
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=1
#SBATCH --mem=4096
#SBATCH --time=00:50:00
#SBATCH --partition=espec

echo "Comparação de Busca Exaustiva e Heurísticas para o Problema da Mochila"
echo " "

# Busca Exaustiva
echo "================ Busca Exaustiva ================"
echo "Executando Busca Exaustiva com Entrada_1.txt:"
./mochila Entrada_1.txt
echo " "

echo "Executando Busca Exaustiva com Entrada_2.txt:"
./mochila Entrada_2.txt
echo " "

echo "Executando Busca Exaustiva com Entrada_3.txt:"
./mochila Entrada_3.txt
echo " "

echo "Executando Busca Exaustiva com Entrada_4.txt:"
./mochila Entrada_4.txt
echo " "

# Heurística Embaralhada - 5 Execuções
echo "================ Heurística Embaralhada ================"
for i in {1..5}
do
  echo "Execução $i com Entrada_1.txt:"
  ./mochilaAleatoria Entrada_1.txt
  echo " "
done

for i in {1..5}
do
  echo "Execução $i com Entrada_2.txt:"
  ./mochilaAleatoria Entrada_2.txt
  echo " "
done

for i in {1..5}
do
  echo "Execução $i com Entrada_3.txt:"
  ./mochilaAleatoria Entrada_3.txt
  echo " "
done

for i in {1..5}
do
  echo "Execução $i com Entrada_4.txt:"
  ./mochilaAleatoria Entrada_4.txt
  echo " "
done

# Heurística Probabilística - 5 Execuções
echo "================ Heurística Probabilística ================"
for i in {1..5}
do
  echo "Execução $i com Entrada_1.txt:"
  ./mochilaProbabilidade Entrada_1.txt
  echo " "
done

for i in {1..5}
do
  echo "Execução $i com Entrada_2.txt:"
  ./mochilaProbabilidade Entrada_2.txt
  echo " "
done

for i in {1..5}
do
  echo "Execução $i com Entrada_3.txt:"
  ./mochilaProbabilidade Entrada_3.txt
  echo " "
done

for i in {1..5}
do
  echo "Execução $i com Entrada_4.txt:"
  ./mochilaProbabilidade Entrada_4.txt
  echo " "
done

echo "================ Fim das Execuções ================"
