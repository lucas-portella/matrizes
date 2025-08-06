#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#define IDX(m,i,j) (i*m->colunas + j)

typedef struct {
	int linhas;
	int colunas;
	int* m;
} matriz;

typedef struct {
	matriz* m1;
	matriz* m2;
	matriz* soma;
	int linha;
} dados_linha;

matriz* cria_matriz (int linhas, int colunas, int* m);
matriz* destroi_matriz (matriz* m);
void imprime_matriz (matriz* m);
int habilita_soma (matriz* m1, matriz* m2);
matriz* soma_matrizes_sequencial (matriz* m1, matriz*m2);

dados_linha* cria_dados_linha (matriz* m1, matriz* m2, matriz* soma, int linha);
dados_linha* destroi_dados_linha (dados_linha* dados);
void* soma_linha (void* arg);

int main (int argc, char* argv[]) {
	FILE* f;
	char buffer[10];
	char c;
	int *v1, *v2;
	int linhas, colunas, i, total, v1_preenchido, j;
	clock_t inicio, fim;
	
	f = (argc == 2) ? fopen (argv[1], "r") : stdin;

	/* leitura do tamanho das matrizes */
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

	fclose(f);

	matriz* matriz1 = cria_matriz (linhas, colunas, v1);
	matriz* matriz2 = cria_matriz (linhas, colunas, v2);

	
	#ifdef PARALELO
	matriz* soma = cria_matriz (linhas, colunas, NULL);
	dados_linha* dados[linhas];
	pthread_t threads[linhas];
	inicio = clock ();
	for (i = 0; i < linhas; i++) {
		dados[i] = cria_dados_linha (matriz1, matriz2, soma, i);
		pthread_create(&threads[i], NULL, soma_linha, dados[i]);
	}

	for (i = 0; i < linhas; i++)
		pthread_join(threads[i], NULL);
	fim = clock ();

	for (i = 0; i < linhas; i++)
		dados[i] = destroi_dados_linha (dados[i]);

	#else
	inicio = clock ();
	matriz* soma = soma_matrizes_sequencial (matriz1, matriz2);
	fim = clock ();
	#endif

	fprintf(stderr, "Tempo de execucao: %f\n", (float) (fim - inicio) / CLOCKS_PER_SEC);
	imprime_matriz (soma);

	matriz1 = destroi_matriz (matriz1);
	matriz2 = destroi_matriz (matriz2);
	soma = destroi_matriz (soma);

	return 0;
}

matriz* cria_matriz (int linhas, int colunas, int* m) {
	matriz* nova;
	int* nm;

	nova = (matriz*) malloc (sizeof(matriz));
	nm = (int*) malloc (linhas * colunas * sizeof(int));

	if (!nova || !nm) {
		if (nova) {
			free (nova);
			nova = NULL;
		}
		if (nm) {
			free (nm);
			nm = NULL;
		}
	}

	if (m) {
		memcpy(nm, m, linhas * colunas * sizeof(int));
	}

	nova->linhas = linhas;
	nova->colunas = colunas;
	nova->m = nm;

	return nova;
}

matriz* destroi_matriz (matriz* m) {
	if (!m)
		return NULL;

	if (m->m) {
		free (m->m);
		m->m = NULL;
	}

	free (m);
	return NULL;
}

void imprime_matriz (matriz* m) {
	if (!m) {
		printf("Matriz nula!\n");
		return;
	}
		
	for (int i = 0; i < m->linhas; i++) {
		for (int j = 0; j < m->colunas; j++) {
			printf("%d\t", m->m[IDX(m,i,j)]);
		}
		printf("\n");
	}
}

int habilita_soma (matriz* m1, matriz* m2) {
	if (!m1 || !m2)
		return 0;

	if (m1->linhas != m2->linhas || m1->colunas != m2->colunas)
		return 0;
	
	return 1;
}

matriz* soma_matrizes_sequencial (matriz* m1, matriz*m2) {
	matriz* soma;

	if (!habilita_soma (m1, m2))
		return NULL;

	soma = cria_matriz (m1->linhas, m1->colunas, NULL);

	if (!soma) 
		return NULL;

	for (int i = 0; i < m1->linhas; i++) {
		for (int j = 0; j < m1->colunas; j++) {
			soma->m[IDX(soma, i, j)] = m1->m[IDX(m1, i, j)] + m2->m[IDX(m2, i, j)];
		}
	}

	return soma;
}

dados_linha* cria_dados_linha (matriz* m1, matriz* m2, matriz* soma, int linha) {
	dados_linha* dados = (dados_linha*) malloc (sizeof(dados_linha));

	if (!dados)
		return NULL;

	dados->m1 = m1;
	dados->m2 = m2;
	dados->soma = soma;
	dados->linha = linha;

	return dados;
}

dados_linha* destroi_dados_linha (dados_linha* dados) {
	if (dados) {
		dados->m1 = NULL;
		dados->m2 = NULL;
		dados->soma = NULL;
		free(dados);
	}

	return NULL;
}

void* soma_linha (void* arg) {
	dados_linha* dados = (dados_linha*) arg;
	int linha = dados->linha;

	for (int i = 0; i < dados->m1->colunas; i++) 
		dados->soma->m[IDX(dados->soma, linha, i)] = dados->m1->m[IDX(dados->m1, linha, i)] + dados->m2->m[IDX(dados->m2, linha, i)];

	pthread_exit(NULL);
}
