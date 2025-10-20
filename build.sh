#!/bin/bash

# interrompe se houver erro
set -e

# cria a pasta build se não existir
mkdir -p build

# entra nela
cd build

# gera os arquivos de build a partir do CMakeLists.txt da raiz
cmake .. -DPARALLEL=1

# compila o projeto
make -j$(nproc)

# volta pra raiz e executa o binário, se existir
cd ..
