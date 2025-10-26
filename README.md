# Computação Paralela

Universidade Federal de São João Del-Rei

Ciência da Computação

## Equipe

#### - Bruno Mamede
#### - João Marcos Sampaio



## Contexto

Esse repositório é destino aos trabalhos práticos realizados na disciplina de computação paralela.

O objetivo é exercitar o usa da biblioteca MPI para desenvolver soluções de alto desempenho utilizando técnicas de programação paralela em linguagem C, dividindo o processamento de um programa em diferentes processos ou mesmo máquinas.

## Compilação

Durante a compilação, existem 3 constantes de pré-processamento que podem ser definidas para modificar o comportamento do código:
#### - LOG_CODE_ENABLED: Ativa logs de execução que serão impressos no stdout.
#### - PARALLEL: Faz a implementação paralela do código ser compilada.
#### - PRINT_RESULT: Faz os valores primos encontrados pelo Crivo de Erastótenes serem impressos no stdout.

## Estratégia implementada: 

Foi implementada uma estratégia embasada no paradigma Master/Slave, onde, para uma entrada N qualquer, teremos o conjunto de números ímpares entre [3,N] segmentados em um número de parcelas equivalente à quantiade de processos trabalhadores. Cada trabalhador tem seu momento de buscar por valores primos (controlado pelo Master) e, ao encontrar um, é enviada uma mensagem para o Master que reenvia o número para os outros trabalhadores que devem marcar em suas parcelas os múltiplos do primo encontrado.
