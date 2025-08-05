/*
	lmat: Leitor de matriz - sera implementado em nmop.c
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main (int argc, char* argv[]) {
	FILE* f;
	char buffer[10];
	char c;
	int *v1, *v2;
	int linhas, colunas, i, total, v1_preenchido, j;
	
	f = (argc == 2) ? fopen (argv[1], "r") : stdin;

	i = 0;
	c = fgetc(f);
	while (c != '\n') {
		if (c == '\t') {
			buffer[i] = '\0';
			linhas = atoi(buffer);
			i = 0;
		}
		else {
			buffer[i++] = c;
		}
		c = fgetc(f);
	}
	buffer[i] = '\0';
	colunas = atoi(buffer);
	
	/* leitura dos valores nos vetores */
	total = linhas * colunas;
	v1 = (int*) malloc (total * sizeof (int));
	v2 = (int*) malloc (total * sizeof (int));

	i = 0;
	j = 0;
	c = fgetc(f);
	while (c != EOF) {
		if (c == '\n') {
			int num;
			buffer[i] = '\0';
			num = atoi(buffer);
			i = 0;
			if (v1_preenchido) {
				v2[j++] = num;
			}
			else {
				if (j == total) {
					v1_preenchido = 1;
					v2[0] = num;
					j = 1;
				}
				else {
					v1[j++] = num;
				}	
			}
		}
		else {
			buffer[i++] = c;
		}
		c = fgetc(f);
	}
	
	printf("%d\t%d\n", linhas, colunas);
	for (i = 0; i < total; i++)
		printf("%d\n", v1[i]);
	for (i = 0; i < total; i++)
		printf("%d\n", v2[i]);

	fclose(f);

	return 0;
}
