# Computação Paralela

Universidade Federal de São João Del-Rei

Ciência da Computação

## Equipe

#### - Bruno Mamede
#### - João Marcos Sampaio



## Contexto

Esse repositório é destino aos trabalhos práticos realizados na disciplina de computação paralela.

O objetivo é exercitar o usa da biblioteca MPI para desenvolver soluções de alto desempenho utilizando técnicas de programação paralela em linguagem C, dividindo o processamento de um programa em diferentes processos ou mesmo máquinas.

### Estratégias: 

Para uma entrada N
1. Estratégia 1
    - Dividir entrada para os N/(nº de processadores-1)
    - p N = 1000, processadores = 5
    - 0 ~ 250, 251 ~ 500, 501 ~ 750, 751 ~1000


2. Estratégia 2
    - Prover digitos a partir do master
    - 0 - master 
    - 1 - slave < 2
    - 2 - slave < 3

3. Estratégia 3
    - Divir igualmente os valores
    - marcar múltiplos na porção
    - calcular próximo múltiplo
    - Envia ao master o primo e o múltiplo

