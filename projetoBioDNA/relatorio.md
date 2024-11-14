# Relatório Final - Projeto de Bioinformática - Supercomputação
## Pedro Toledo Piza Civita - pedrotpc@al.insper.edu.br
---

## Introdução

Este projeto tem como objetivo aplicar técnicas de bioinformática para análise de sequências genéticas, com foco na paralelização e otimização de código utilizando MPI (Message Passing Interface) e OpenMP (Open Multi-Processing). A manipulação e análise de grandes volumes de dados genômicos requerem abordagens eficientes para processamento em ambientes de alto desempenho.

As principais tarefas envolvem:

- **Análise de frequência de bases nucleotídicas.**
- **Transcrição de DNA em RNA.**
- **Contagem de inícios de proteínas (códon AUG).**
- **Tradução de RNA em proteínas.**
- **Implementação de algoritmos paralelos para otimizar o processamento.**

Este relatório apresenta a metodologia adotada, os resultados obtidos, a comparação entre as versões sequenciais e paralelas, além da discussão sobre os benefícios e limitações da paralelização.

---

## Metodologia

### Preparação dos Dados

**Download e Descompactação dos Arquivos Genômicos:**

Os arquivos dos cromossomos 1 a 22 foram baixados do servidor FTP:

```bash
wget "ftp://hgdownload.cse.ucsc.edu/goldenPath/hg19/snp147Mask/chrX.subst.fa.gz"
```

onde **X** varia de 1 a 22.

A descompactação foi realizada com:

```bash
gunzip chrX.subst.fa.gz
```

**Organização dos Dados:**

Os arquivos foram organizados em uma pasta chamada `cromossomos`, facilitando o acesso e processamento pelos programas desenvolvidos.

---

### Exercício 1: Análise de Frequência de Bases

**Objetivo:**

Contar o número de ocorrências de cada base nucleotídica (A, T, C, G) nos cromossomos 1 a 22.

**Metodologia:**

- **Versão Paralela (MPI e OpenMP):**
  - Utilizar MPI para distribuir os cromossomos entre processos.
  - Dentro de cada processo, utilizar OpenMP para paralelizar a contagem das bases nas sequências atribuídas.

**Implementação:**

- **MPI:**
  - Distribuição dos cromossomos considerando o rank do processo.
- **OpenMP:**
  - Paralelização dos laços de contagem com redução das contagens locais.

---

### Exercício 2: Transcrição de DNA em RNA

**Objetivo:**

Converter sequências de DNA em RNA, substituindo as bases nitrogenadas conforme as regras de transcrição.

**Metodologia:**

- **Versão Paralela (MPI e OpenMP):**
  - MPI para distribuir os cromossomos.
  - OpenMP para paralelizar a substituição das bases.

**Implementação:**

- **Tabela de Transcrição:**
  - A → U
  - T → A
  - C → G
  - G → C

---

### Exercício 3: Contagem de Inícios de Proteínas (Códon AUG)

**Objetivo:**

Contar quantas proteínas são potencialmente iniciadas em cada sequência de RNA, identificando o códon de início **AUG**.

**Metodologia:**

- **Versão Paralela (MPI e OpenMP):**
  - Distribuição das sequências entre processos MPI.
  - Paralelização da busca por **AUG** com OpenMP.

**Implementação:**

- **MPI:**
  - Cada processo trabalha com um subconjunto de cromossomos.
- **OpenMP:**
  - Paralelização do laço de busca pelos códons.

---

### Exercício 4: Tradução de RNA em Proteínas

**Objetivo:**

Traduzir as sequências de RNA em proteínas até encontrar um códon de parada, utilizando a tabela de códons fornecida.

**Tabela de Códons e Aminoácidos Correspondentes:**

| Códon (RNAm)       | Aminoácido         | Número |
|--------------------|--------------------|--------|
| AUG                | Metionina (Início) |   1    |
| CCA, CCG, CCU, CCC | Prolina            |   2    |
| UCU, UCA, UCG, UCC | Serina             |   3    |
| CAG, CAA           | Glutamina          |   4    |
| ACA, ACC, ACU, ACG | Treonina           |   5    |
| UGC, UGU           | Cisteína           |   6    |
| GUG, GUA, GUC, GUU | Valina             |   7    |
| UGA                | **Códon STOP**     |   0    |

**Metodologia:**

- **Versão Paralela (MPI e OpenMP):**
  - MPI para distribuir as sequências de RNA dos cromossomos entre processos.
  - OpenMP para paralelizar a tradução dos códons em aminoácidos.

**Implementação:**

- **Tradução da Sequência:**
  - Iniciar a tradução ao encontrar o códon de início **AUG**.
  - Traduzir cada códon subsequente utilizando a tabela de códons.
  - Interromper a tradução ao encontrar o códon de parada **UGA**.
- **Paralelização:**
  - Divisão da sequência de RNA em segmentos para processamento paralelo com OpenMP.
  - Sincronização adequada para evitar condições de corrida.

---

## Resultados

### Dados Obtidos

#### **Exercício 1: Análise de Frequência de Bases**

**Saída:**

```
Contagem total de bases:
A: 763.517.118
T: 764.606.176
C: 511.685.713
G: 511.971.484
Tempo total de execução: 62.0935 segundos.
```

#### **Exercício 2: Transcrição de DNA em RNA**

**Saída:**

- Os processos reportaram a conclusão da transcrição dos cromossomos atribuídos.
- **Tempos de execução:**
  - Processo 0: 49.1387 segundos
  - Processo 1: 49.1721 segundos
  - Processo 2: 38.1395 segundos
  - Processo 3: 32.8954 segundos

#### **Exercício 3: Contagem de Inícios de Proteínas (AUG)**

**Saída:**

```
Total de proteínas inicializadas (AUG): 25.785.550
Tempo total de execução: 26.2817 segundos.
```

#### **Exercício 4: Tradução de RNA em Proteínas**

**Saída:**

- As sequências de RNA foram traduzidas em proteínas, representadas por números conforme a tabela de códons.
- Exemplos de saídas por processo:

```
Processo 0, Cromossomo 1, Proteína traduzida: 1-7-2-3-Fim
Processo 1, Cromossomo 2, Proteína traduzida: 1-3-6-5-7-5-5-2-7-2-1-3-4-7-5-3-5-7-6-7-3-7-3-2-3-2-7-7-7-5-7-2-Fim
Processo 2, Cromossomo 3, Proteína traduzida: 1-5-Fim
Processo 3, Cromossomo 4, Proteína traduzida: 1-5-5-7-2-2-5-7-7-5-3-Fim
...
Processo 0: tempo de execução = 37.7468 segundos.
Processo 1: tempo de execução = 39.0704 segundos.
Processo 2: tempo de execução = 39.1777 segundos.
Processo 3: tempo de execução = 34.127 segundos.
```

- **Tempos de execução:**
  - Os tempos variaram entre aproximadamente 34 e 39 segundos entre os processos.

---

### Tempos de Execução

Com base nos resultados obtidos, atualizamos a tabela de tempos de execução:

| Exercício                               | Tempo de Execução (s) |
|-----------------------------------------|-----------------------|
| 1. Análise de Frequência de Bases       | 62.09                 |
| 2. Transcrição de DNA em RNA            | 49.17 (máximo)        |
| 3. Contagem de Inícios de Proteínas     | 26.28                 |
| 4. Tradução de RNA em Proteínas         | 39.18 (máximo)        |

**Observação:**

- Para o exercício 4, consideramos o tempo máximo entre os processos, já que o programa finaliza quando todos os processos concluem.

### Gráficos

#### 1. Frequência de Bases Nucleotídicas

![Gráfico de Frequência de Bases](img/frequencia_bases.png)

**Descrição do Gráfico:**

- **Eixo X:** Bases nucleotídicas (A, T, C, G).
- **Eixo Y:** Número total de ocorrências de cada base.

#### 2. Tempos de Execução dos Exercícios

![Gráfico de Tempos de Execução](img/tempos_execucao.png)

**Descrição do Gráfico:**

- **Eixo X:** Exercícios (1 a 4).
- **Eixo Y:** Tempo de execução em segundos.

---

## Análise dos Resultados

### Análise de Frequência de Bases

- **Distribuição Equilibrada:**
  - As contagens de A e T são similares, assim como C e G, refletindo a complementaridade das bases no DNA.
- **Volume de Dados:**
  - O número total de bases processadas é elevado, indicando o processamento eficiente de grandes volumes de dados genômicos.
- **Tempo de Execução:**
  - O tempo total de 62 segundos é considerado eficiente, graças à paralelização.

### Transcrição de DNA em RNA

- **Desempenho Variado entre Processos:**
  - Os tempos de execução variaram entre os processos devido ao tamanho diferente dos cromossomos.
  - Processos que receberam cromossomos maiores levaram mais tempo.
- **Balanceamento de Carga:**
  - Observa-se a necessidade de um melhor balanceamento de carga para otimizar o tempo total de execução.

### Contagem de Inícios de Proteínas (AUG)

- **Número de Códons de Início:**
  - Foram identificadas mais de 25 milhões de ocorrências do códon **AUG**.
- **Tempo de Execução:**
  - O tempo total de aproximadamente 26 segundos demonstra a eficiência do programa paralelo.

### Tradução de RNA em Proteínas

- **Resultados da Tradução:**
  - Com a tabela de códons atualizada, foi possível traduzir as sequências de RNA em proteínas de forma adequada.
  - As sequências de aminoácidos traduzidas variam em comprimento, dependendo do número de códons traduzidos antes de encontrar o códon de parada.
- **Tempo de Execução:**
  - O tempo de execução aumentou em relação à versão anterior devido à complexidade adicional da tradução completa.
  - Os tempos variaram entre aproximadamente 34 e 39 segundos, demonstrando um desempenho consistente entre os processos.
- **Análise Biológica:**
  - A tradução correta das sequências permite futuras análises biológicas, como identificação de proteínas e estudos de mutações.

---

## Discussão

### Benefícios da Paralelização

- **Redução do Tempo de Execução:**
  - A paralelização reduziu significativamente os tempos de execução, tornando o processamento de grandes volumes de dados viável.
- **Eficiência Computacional:**
  - O uso combinado de MPI e OpenMP permitiu aproveitar ao máximo os recursos computacionais disponíveis.
- **Implementação Completa:**
  - Com a tabela de códons atualizada, o programa realiza a tradução conforme o enunciado, permitindo análises mais profundas.

### Limitações e Desafios

- **Balanceamento de Carga:**
  - A distribuição estática dos cromossomos causou desequilíbrios. Cromossomos maiores resultaram em tempos de execução mais longos para alguns processos.
- **Tabela de Códons Simplificada:**
  - A tabela de códons utilizada não contempla todos os 64 códons possíveis, limitando a tradução a um subconjunto de aminoácidos.
- **Escalabilidade:**
  - Para conjuntos de dados ainda maiores ou para utilização em sistemas com mais recursos, técnicas adicionais de otimização podem ser necessárias.

---

## Conclusão

O projeto demonstrou a eficácia da paralelização em bioinformática para o processamento de dados genômicos em larga escala. As implementações paralelas permitiram reduzir os tempos de execução e processar os cromossomos humanos de forma eficiente.

**Principais Contribuições:**

- **Implementação Completa dos Exercícios:**
  - Todos os exercícios foram implementados conforme o enunciado, incluindo a tradução de RNA em proteínas com a tabela de códons fornecida.
- **Análise de Dados Genômicos:**
  - Processamento eficiente dos cromossomos humanos, fornecendo resultados relevantes para análises futuras.
- **Desenvolvimento de Competências:**
  - Aplicação prática de técnicas de paralelização e bioinformática.

**Recomendações para Trabalhos Futuros:**

- **Balanceamento Dinâmico de Carga:**
  - Implementar técnicas que distribuem as tarefas com base no tamanho dos cromossomos para equilibrar a carga entre os processos.
- **Expansão da Tabela de Códons:**
  - Incluir todos os códons e seus aminoácidos correspondentes para permitir uma tradução completa das sequências de RNA.
- **Análises Biológicas Mais Profundas:**
  - Expandir as análises para extrair insights biológicos dos dados processados, como identificação de genes ou regiões de interesse.

---

## Referências

- **OpenMPI Documentation:** [https://www.open-mpi.org/doc/](https://www.open-mpi.org/doc/)
- **OpenMP Documentation:** [https://www.openmp.org/specifications/](https://www.openmp.org/specifications/)
- **Código Genético:** [https://en.wikipedia.org/wiki/Genetic_code](https://en.wikipedia.org/wiki/Genetic_code)

---

## Anexos

### Gráficos Gerados

#### Gráfico de Frequência de Bases

![Gráfico de Frequência de Bases](img/frequencia_bases.png)

#### Gráfico de Tempos de Execução

![Gráfico de Tempos de Execução](img/tempos_execucao.png)

---

## Apêndice: Códigos Implementados

Os códigos fonte dos programas desenvolvidos estão anexados, incluindo:

- **Exercício 1:** `ex1.cpp`
- **Exercício 2:** `ex2.cpp`
- **Exercício 3:** `ex3.cpp`
- **Exercício 4:** `ex4.cpp`
- **Scripts SLURM:** `mpi_exercises.slurm`

---

**Nota:** Os resultados apresentados neste relatório são baseados nas execuções realizadas com os dados dos cromossomos humanos 1 a 22. As implementações podem ser adaptadas para outros conjuntos de dados genômicos, permitindo ampliar as análises e aplicações em bioinformática.

## Considerações Finais

Este relatório detalha as etapas do projeto de bioinformática, desde a preparação dos dados até a análise dos resultados. A paralelização utilizando MPI e OpenMP mostrou-se eficaz para acelerar o processamento de grandes volumes de dados genômicos.

Os resultados obtidos evidenciam a importância do balanceamento de carga e da expansão da tabela de códons para a obtenção de resultados mais precisos e análises mais aprofundadas.

---

**Obrigado pela atenção!**

---