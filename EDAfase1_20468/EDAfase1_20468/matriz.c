/*
* @file matriz.c
* @author Diogo Oliveira (a20468@alunos.ipca.pt)
* @brief matriz cont�m a implementa��o das fun��es relacionadas � manipula��o de matrizes e lista ligada.
 * Inclui fun��es para criar matrizes, inserir e limpar matrizes de uma lista ligada, visualizar matrizes,
 * guardar e carregar matrizes de/para um arquivo, calcular a soma m�xima poss�vel em uma matriz, inserir valores
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


//fun�ao responsavel por associar valores random aos espa�os da matriz
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

//usado na fun�ao onde se limpa a Lista Ligada
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

    // Primeiro, conta o n�mero de linhas e colunas
    while ((ch = fgetc(file)) != EOF) {
        if (ch == '\n') {
            rows++;
        }
        else if (ch == ';') {
            cols++;
        }
    }
    cols = cols / rows + 1; // Calcula o n�mero de colunas

    // Reposiciona o ponteiro do arquivo para o in�cio
    rewind(file);

    // Aloca espa�o para a matriz
    int** matriz = (int**)malloc(rows * sizeof(int*));
    for (int i = 0; i < rows; i++) {
        matriz[i] = (int*)malloc(cols * sizeof(int));
    }

    // L� os valores do arquivo e preenche a matriz
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            fscanf_s(file, "%d", &matriz[i][j]);
            // Ignora o ';' ap�s cada n�mero, exceto o �ltimo n�mero em uma linha
            if (j != cols - 1) {
                fgetc(file); // Descartar o pr�ximo caractere
            }
        }
        // Ignora o '\n' ap�s cada linha, exceto a �ltima linha
        if (i != rows - 1) {
            fgetc(file); // Descartar o pr�ximo caractere
        }
    }

    // Fecha o arquivo
    fclose(file);

    // Insere a matriz na lista ligada
    *head = inserirMatriz(*head, matriz, rows, cols);
}

// Fun��o para inserir uma nova linha e uma nova coluna na matriz
void inserirNovaLinhaColuna(struct No* matriz, int novaPosicao) {
    int novaIndex = novaPosicao - 1;
    int rows = matriz->rows;
    int cols = matriz->cols;

    // Alocar espa�o para a nova matriz com uma linha e uma coluna adicionais
    int** novaMatriz = (int**)malloc((rows + 1) * sizeof(int*));
    for (int i = 0; i < rows + 1; i++) {
        novaMatriz[i] = (int*)malloc((cols + 1) * sizeof(int));
        if (i < rows) {
            for (int j = 0; j < cols; j++) {
                novaMatriz[i][j] = matriz->matriz[i][j];
            }
        }
    }

    // Preencher a nova linha e a nova coluna com n�meros aleat�rios de 1 a 999
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

// Fun��o para remover uma linha e uma coluna da matriz
void removerLinhaColuna(struct No* matriz, int posicao) {
    int index = posicao - 1;
    int rows = matriz->rows;
    int cols = matriz->cols;

    // Verificar se a posi��o � v�lida
    if (index < 0 || index >= rows) {
        printf("Posicao invalida.\n");
        return;
    }

    // Alocar espa�o para a nova matriz com uma linha e uma coluna a menos
    int** novaMatriz = (int**)malloc((rows - 1) * sizeof(int*));
    for (int i = 0, k = 0; i < rows; i++) {
        // Excluir a linha da posi��o indicada
        if (i != index) {
            novaMatriz[k] = (int*)malloc((cols - 1) * sizeof(int));
            for (int j = 0, l = 0; j < cols; j++) {
                // Excluir a coluna da posi��o indicada
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

    char line[999]; // Assumindo um tamanho m�ximo de 3 digitos
    while (fgets(line, sizeof(line), file) != NULL) {
        char* token = NULL;
        char* next_token = NULL;
        token = strtok_s(line, ";", &next_token);
        while (token != NULL) {
            printf("%9s", token); // Ajuste o espa�amento conforme necess�rio
            token = strtok_s(NULL, ";", &next_token);
        }
        printf("\n");
    }

    fclose(file);
}


// Fun��o auxiliar para verificar se um n�mero est� contido em um array
bool contains(int* arr, int size, int num) {
    for (int i = 0; i < size; i++) {
        if (arr[i] == num) {
            return true;
        }
    }
    return false;
}

// Fun��o auxiliar recursiva para encontrar a soma m�xima e os n�meros selecionados
void findMaxSum(int** matriz, int rows, int cols, int row, int* selectedNumbers, int* maxSumResult, int currentSum, int* bestSelectedNumbers) {
    // Verifica se atingiu a �ltima linha da matriz
    if (row == rows) {
        if (currentSum > *maxSumResult) {
            *maxSumResult = currentSum;
            // Se encontrarmos uma nova soma m�xima, atualizamos os n�meros selecionados correspondentes
            for (int i = 0; i < rows; i++) {
                bestSelectedNumbers[i] = matriz[i][selectedNumbers[i]];
            }
        }
        return;
    }

    // Explora todas as possibilidades de sele��o de n�meros para esta linha
    for (int col = 0; col < cols; col++) {
        // Verifica se o n�mero na posi��o (row, col) pode ser selecionado
        if (!contains(selectedNumbers, row, col)) {
            // Atualiza a soma atual
            currentSum += matriz[row][col];

            // Marca o n�mero selecionado
            selectedNumbers[row] = col;

            // Chama recursivamente para a pr�xima linha
            findMaxSum(matriz, rows, cols, row + 1, selectedNumbers, maxSumResult, currentSum, bestSelectedNumbers);

            // Desfaz as altera��es para explorar outras op��es
            currentSum -= matriz[row][col];
            selectedNumbers[row] = -1;
        }
    }
}

// Fun��o principal para calcular a soma m�xima poss�vel e imprimir os n�meros selecionados
int maxSum(int** matriz, int rows, int cols) {
    // Array para rastrear os n�meros selecionados em cada linha
    int* selectedNumbers = (int*)malloc(rows * sizeof(int));
    int* bestSelectedNumbers = (int*)malloc(rows * sizeof(int));
    for (int i = 0; i < rows; i++) {
        selectedNumbers[i] = -1; // Inicializa com -1, indicando que nenhuma sele��o foi feita
        bestSelectedNumbers[i] = -1; // Inicializa com -1, indicando que nenhuma sele��o foi feita
    }

    // Vari�vel para armazenar a soma m�xima encontrada
    int maxSumResult = 0;

    // Chamada da fun��o auxiliar recursiva para encontrar a soma m�xima
    findMaxSum(matriz, rows, cols, 0, selectedNumbers, &maxSumResult, 0, bestSelectedNumbers);

    // Imprime os n�meros selecionados
    printf("Numeros selecionados para a soma maxima:\n");
    for (int i = 0; i < rows; i++) {
        printf("%d ", bestSelectedNumbers[i]);
    }
    printf("\n");

    // Libera a mem�ria alocada para os arrays de n�meros selecionados
    free(selectedNumbers);
    free(bestSelectedNumbers);

    return maxSumResult;
}

void inserirValoresManualmente(struct No** head) {
    int rows, cols;

    // Solicita ao usu�rio o n�mero de linhas e colunas
    printf("Insira o numero de linhas e colunas: ");
    scanf_s("%d", &rows);
    cols = rows;

    // Aloca mem�ria para a matriz
    int** matriz = (int**)malloc(rows * sizeof(int*));
    for (int i = 0; i < rows; i++) {
        matriz[i] = (int*)malloc(cols * sizeof(int));
    }

    // Solicita ao usu�rio os valores para cada posi��o da matriz
    printf("Insira os valores para a matriz (valores entre 1 e 999):\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            int valor;
            do {
                printf("Insira o valor para a posi��o [%d][%d]: ", i, j);
                scanf_s("%d", &valor);
                if (valor < 1 || valor > 999) {
                    printf("Valor fora do intervalo permitido (1 a 999). Tente novamente.\n");
                }
            } while (valor < 1 || valor > 999);
            matriz[i][j] = valor;
        }
    }

    // Insere a matriz na lista ligada
    *head = inserirMatriz(*head, matriz, rows, cols);
    printf("Matriz inserida.\n");
}
