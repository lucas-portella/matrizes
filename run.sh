#!/usr/bin/env bash

if [ ! -f paralelo -o ! -f sequencial -o ! -f mrand ]; then
	echo "Executáveis não encontrados. Use 'make'."
	exit 1
fi

echo "Realizando teste de tempo de execução de soma de matrizes de forma sequencial e paralela."
read -p "Digite o número de linhas: " linhas
read -p "Digite o número de colunas: " colunas

echo "Criando matrizes.."
./mrand $linhas $colunas > matrizes.txt
echo "Tempo de execução sequencial:"
./sequencial matrizes.txt > soma_sequencial.txt
echo "Tempo de execução paralela:"
./paralelo matrizes.txt > soma_paralelo.txt

echo "Comparando resultados de soma."
if [ $(diff soma_sequencial.txt soma_paralelo.txt | wc -l) -gt 0 ]; then
	echo "Erro do algoritmo: Resultados divergem."
else
	echo "Matrizes soma identicas."
fi
