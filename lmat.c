/*
	lmat: Leitor de matriz - sera implementado em nmop.c
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main (int argc, char* argv[]) {
	FILE* f;
	char buffer[10];
	int* v;
	int linhas, colunas, i;
	
	f = (argc == 2) ? fopen (argv[1], "r") : stdin;

	
	return 0;
}
