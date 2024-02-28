#include "matriz.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>


int** criarMatriz(int rows, int cols) {
    int** matriz = (int**)malloc(rows * sizeof(int*));
    for (int i = 0; i < rows; i++) {
        matriz[i] = (int*)malloc(cols * sizeof(int));
        for (int j = 0; j < cols; j++) {
            matriz[i][j] = rand() % 999 + 1;
        }
    }
    return matriz;
}

void LimparMatriz(int** matriz, int rows) {
    for (int i = 0; i < rows; i++) {
        free(matriz[i]);
    }
    free(matriz);
}

struct No* inserirMatriz(struct No* head, int** matriz, int rows, int cols) {
    struct No* novoNo = (struct No*)malloc(sizeof(struct No));
    novoNo->matriz = matriz;
    novoNo->rows = rows;
    novoNo->cols = cols;
    novoNo->prox = head;
    return novoNo;
}

void verMatriz(int** matriz, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%4d ", matriz[i][j]);
        }
        printf("\n");
    }
}

void limparLL(struct No* head) {
    struct No* current = head;
    while (current != NULL) {
        struct No* temp = current;
        current = current->prox;
        LimparMatriz(temp->matriz, temp->rows);
        free(temp);
    }
}

void guardarMatriz(struct No* head, const char* nomeficheiro) {
    FILE* file = fopen(nomeficheiro, "w");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo para escrita.\n");
        return;
    }

    struct No* current = head;
    while (current != NULL) {
        for (int i = 0; i < current->rows; i++) {
            for (int j = 0; j < current->cols; j++) {
                fprintf(file, "%d", current->matriz[i][j]);
                if (j != current->cols - 1) {
                    fprintf(file, ";");
                }
            }
            fprintf(file, "\n");
        }
        current = current->prox;
    }

    fclose(file);
}

void carregarMatriz(struct No** head, const char* nomeficheiro) {
    FILE* file = fopen(nomeficheiro, "r");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo para leitura.\n");
        return;
    }

    int rows = 0, cols = 0;
    char ch;

    // Primeiro, conta o número de linhas e colunas
    while ((ch = fgetc(file)) != EOF) {
        if (ch == '\n') {
            rows++;
        }
        else if (ch == ';') {
            cols++;
        }
    }
    cols = cols / rows + 1; // Calcula o número de colunas

    // Reposiciona o ponteiro do arquivo para o início
    rewind(file);

    // Aloca espaço para a matriz
    int** matriz = (int**)malloc(rows * sizeof(int*));
    for (int i = 0; i < rows; i++) {
        matriz[i] = (int*)malloc(cols * sizeof(int));
    }

    // Lê os valores do arquivo e preenche a matriz
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            fscanf_s(file, "%d", &matriz[i][j]);
            // Ignora o ';' após cada número, exceto o último número em uma linha
            if (j != cols - 1) {
                fgetc(file); // Descartar o próximo caractere
            }
        }
        // Ignora o '\n' após cada linha, exceto a última linha
        if (i != rows - 1) {
            fgetc(file); // Descartar o próximo caractere
        }
    }

    // Fecha o arquivo
    fclose(file);

    // Insere a matriz na lista ligada
    *head = inserirMatriz(*head, matriz, rows, cols);
}

// Função para inserir uma nova linha e uma nova coluna na matriz
void inserirNovaLinhaColuna(struct No* matriz, int novaPosicao) {
    int novaIndex = novaPosicao - 1;
    int rows = matriz->rows;
    int cols = matriz->cols;

    // Alocar espaço para a nova matriz com uma linha e uma coluna adicionais
    int** novaMatriz = (int**)malloc((rows + 1) * sizeof(int*));
    for (int i = 0; i < rows + 1; i++) {
        novaMatriz[i] = (int*)malloc((cols + 1) * sizeof(int));
        if (i < rows) {
            for (int j = 0; j < cols; j++) {
                novaMatriz[i][j] = matriz->matriz[i][j];
            }
        }
    }

    // Preencher a nova linha e a nova coluna com números aleatórios de 1 a 999
    for (int j = 0; j < cols; j++) {
        novaMatriz[novaIndex][j] = rand() % 999 + 1;
    }
    for (int i = 0; i < rows + 1; i++) {
        novaMatriz[i][cols] = rand() % 999 + 1;
    }

    // Libera a matriz antiga
    for (int i = 0; i < rows; i++) {
        free(matriz->matriz[i]);
    }
    free(matriz->matriz);

    // Atualiza os dados da matriz na estrutura
    matriz->matriz = novaMatriz;
    matriz->rows++;
    matriz->cols++;
}

// Função para remover uma linha e uma coluna da matriz
void removerLinhaColuna(struct No* matriz, int posicao) {
    int index = posicao - 1;
    int rows = matriz->rows;
    int cols = matriz->cols;

    // Verificar se a posição é válida
    if (index < 0 || index >= rows) {
        printf("Posicao invalida.\n");
        return;
    }

    // Alocar espaço para a nova matriz com uma linha e uma coluna a menos
    int** novaMatriz = (int**)malloc((rows - 1) * sizeof(int*));
    for (int i = 0, k = 0; i < rows; i++) {
        // Excluir a linha da posição indicada
        if (i != index) {
            novaMatriz[k] = (int*)malloc((cols - 1) * sizeof(int));
            for (int j = 0, l = 0; j < cols; j++) {
                // Excluir a coluna da posição indicada
                if (j != index) {
                    novaMatriz[k][l++] = matriz->matriz[i][j];
                }
            }
            k++;
        }
    }

    // Libera a matriz antiga
    for (int i = 0; i < rows; i++) {
        free(matriz->matriz[i]);
    }
    free(matriz->matriz);

    // Atualiza os dados da matriz na estrutura
    matriz->matriz = novaMatriz;
    matriz->rows--;
    matriz->cols--;
}

void listarMatriz(const char* nomeficheiro) {
    FILE* file = fopen(nomeficheiro, "r");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo para leitura.\n");
        return;
    }

    printf("Matriz:\n");

    char line[999]; // Assumindo um tamanho máximo de 3 digitos
    while (fgets(line, sizeof(line), file) != NULL) {
        char* token = NULL;
        char* next_token = NULL;
        token = strtok_s(line, ";", &next_token);
        while (token != NULL) {
            printf("%6s", token); // Ajuste o espaçamento conforme necessário
            token = strtok_s(NULL, ";", &next_token);
        }
        printf("\n");
    }

    fclose(file);
}