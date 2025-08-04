/* 	mrand: Programa que gera duas matrizes aleatórias
	Uso: mrand LINHAS COLUNAS [OUTPUT]
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NRAND (rand()%10000)

int main (int argc, char* argv[]) {
	int linhas, colunas;
	FILE *f;

	if (argc < 3) {
		printf("Uso: mrand LINHAS COLUNAS [OUTPUT].\n");
		return 1;
	}

	srand (time(NULL));

	linhas = atoi(argv[1]);
	colunas = atoi(argv[2]);
	f = (argc == 4) ? fopen(argv[3], "w") : stdout;

	for (int i =0; i < 2 * linhas * colunas; i++)
		fprintf(f, "%d\t", NRAND);

	fclose(f);

	return 0;
}
