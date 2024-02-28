/*
* @file matriz.c
* @author Diogo Oliveira (a20468@alunos.ipca.pt)
* @brief matriz contém a implementação das funções relacionadas à manipulação de matrizes e lista ligada.
 * Inclui funções para criar matrizes, inserir e limpar matrizes de uma lista ligada, visualizar matrizes,
 * guardar e carregar matrizes de/para um arquivo, calcular a soma máxima possível em uma matriz, inserir valores
 * manualmente em uma matriz, entre outras funcionalidades.
*
* @copyright Copyright (c) 2024
*
*/

#include "matriz.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>


//funçao responsavel por associar valores random aos espaços da matriz
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

//usado na funçao onde se limpa a Lista Ligada
void LimparMatriz(int** matriz, int rows) {
    for (int i = 0; i < rows; i++) {
        free(matriz[i]);
    }
    free(matriz);
}

//Inserir matriz na Lista Ligada
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
            printf("%9s", token); // Ajuste o espaçamento conforme necessário
            token = strtok_s(NULL, ";", &next_token);
        }
        printf("\n");
    }

    fclose(file);
}


// Função auxiliar para verificar se um número está contido em um array
bool contains(int* arr, int size, int num) {
    for (int i = 0; i < size; i++) {
        if (arr[i] == num) {
            return true;
        }
    }
    return false;
}

// Função auxiliar recursiva para encontrar a soma máxima e os números selecionados
void findMaxSum(int** matriz, int rows, int cols, int row, int* selectedNumbers, int* maxSumResult, int currentSum, int* bestSelectedNumbers) {
    // Verifica se atingiu a última linha da matriz
    if (row == rows) {
        if (currentSum > *maxSumResult) {
            *maxSumResult = currentSum;
            // Se encontrarmos uma nova soma máxima, atualizamos os números selecionados correspondentes
            for (int i = 0; i < rows; i++) {
                bestSelectedNumbers[i] = matriz[i][selectedNumbers[i]];
            }
        }
        return;
    }

    // Explora todas as possibilidades de seleção de números para esta linha
    for (int col = 0; col < cols; col++) {
        // Verifica se o número na posição (row, col) pode ser selecionado
        if (!contains(selectedNumbers, row, col)) {
            // Atualiza a soma atual
            currentSum += matriz[row][col];

            // Marca o número selecionado
            selectedNumbers[row] = col;

            // Chama recursivamente para a próxima linha
            findMaxSum(matriz, rows, cols, row + 1, selectedNumbers, maxSumResult, currentSum, bestSelectedNumbers);

            // Desfaz as alterações para explorar outras opções
            currentSum -= matriz[row][col];
            selectedNumbers[row] = -1;
        }
    }
}

// Função principal para calcular a soma máxima possível e imprimir os números selecionados
int maxSum(int** matriz, int rows, int cols) {
    // Array para rastrear os números selecionados em cada linha
    int* selectedNumbers = (int*)malloc(rows * sizeof(int));
    int* bestSelectedNumbers = (int*)malloc(rows * sizeof(int));
    for (int i = 0; i < rows; i++) {
        selectedNumbers[i] = -1; // Inicializa com -1, indicando que nenhuma seleção foi feita
        bestSelectedNumbers[i] = -1; // Inicializa com -1, indicando que nenhuma seleção foi feita
    }

    // Variável para armazenar a soma máxima encontrada
    int maxSumResult = 0;

    // Chamada da função auxiliar recursiva para encontrar a soma máxima
    findMaxSum(matriz, rows, cols, 0, selectedNumbers, &maxSumResult, 0, bestSelectedNumbers);

    // Imprime os números selecionados
    printf("Numeros selecionados para a soma maxima:\n");
    for (int i = 0; i < rows; i++) {
        printf("%d ", bestSelectedNumbers[i]);
    }
    printf("\n");

    // Libera a memória alocada para os arrays de números selecionados
    free(selectedNumbers);
    free(bestSelectedNumbers);

    return maxSumResult;
}

void inserirValoresManualmente(struct No** head) {
    int rows, cols;

    // Solicita ao usuário o número de linhas e colunas
    printf("Insira o numero de linhas e colunas: ");
    scanf_s("%d", &rows);
    cols = rows;

    // Aloca memória para a matriz
    int** matriz = (int**)malloc(rows * sizeof(int*));
    for (int i = 0; i < rows; i++) {
        matriz[i] = (int*)malloc(cols * sizeof(int));
    }

    // Solicita ao usuário os valores para cada posição da matriz
    printf("Insira os valores para a matriz:\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("Insira o valor para a posição [%d][%d]: ", i, j);
            scanf_s("%d", &matriz[i][j]);
        }
    }

    // Insere a matriz na lista ligada
    *head = inserirMatriz(*head, matriz, rows, cols);
    printf("Matriz inserida.\n");
}
