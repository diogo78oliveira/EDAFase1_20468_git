/*******************************************************************************************************************
* @file matriz.c
* @author Diogo Oliveira (a20468@alunos.ipca.pt)
* @brief matriz contém a implementação das funções relacionadas à manipulação de matrizes e lista ligada.
 * Inclui funções para criar matrizes, inserir e limpar matrizes de uma lista ligada, visualizar matrizes,
 * guardar e carregar matrizes de/para um arquivo, calcular a soma máxima possível em uma matriz, inserir valores
 * manualmente em uma matriz, entre outras funcionalidades.
*
* @copyright Copyright (c) 2024
*
 *******************************************************************************************************************/


#include "matriz.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>


#pragma region Criar Matriz
 /**
  * @brief Cria uma nova matriz com valores aleatórios entre 1 e 999.
  * 
  * @param rows.
  * @param cols.
  * @return matriz.
  * @autor Diogo Oliveira
  * 
  */
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
#pragma endregion



#pragma region Limpar Matriz
/**
 * @brief Limpa a memoria que a matriz está a utilizar.
 * 
 * @param matriz.
 * @param rows.
 * @autor Diogo Oliveira
 * 
 */
void LimparMatriz(int** matriz, int rows) {
    for (int i = 0; i < rows; i++) {
        free(matriz[i]);
    }
    free(matriz);
}
#pragma endregion



#pragma region Inserir Matriz
/**
 * @brief Insere uma nova matriz na lista ligada.
 * 
 * @param head (Ponteiro para o início da lista ligada). 
 * @param matriz (Matriz a ser inserida).
 * @param rows.
 * @param cols.
 * @return nonoNo (Ponteiro para o novo nó inserido na lista ligada).
 * @autor Diogo Oliveira
 * 
 */
struct No* inserirMatriz(struct No* head, int** matriz, int rows, int cols) {
    struct No* novoNo = (struct No*)malloc(sizeof(struct No));
    novoNo->matriz = matriz;
    novoNo->rows = rows;
    novoNo->cols = cols;
    novoNo->prox = head;
    return novoNo;
}
#pragma endregion


#pragma region Visualizar Matriz
/**
 * @brief Exibe uma matriz na tela.
 * 
 * @param matriz.
 * @param rows.
 * @param cols.
 * @autor Diogo Oliveira
 * 
 */
void verMatriz(int** matriz, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%4d ", matriz[i][j]);
        }
        printf("\n");
    }
}
#pragma endregion


#pragma region Limpar Lista Ligada
/**
 * @brief Limpa a lista ligada e limpa a memória através da funçao 'LimparMatriz'.
 * 
 * @param head.
 * @autor Diogo Oliveira
 * 
 */
void limparLL(struct No* head) {    //'temp' utilizado por precauçao para limpar corretamente a memoria
    struct No* current = head;      //ponteiro 'current' utilizado para percorrer a lista ligada e apontar para o nó atual    
    while (current != NULL) {
        struct No* temp = current;
        current = current->prox;
        LimparMatriz(temp->matriz, temp->rows);
        free(temp);
    }
}
#pragma endregion


#pragma region Guardar Matriz no Ficheiro de Texto
/**
 * @brief Salva a matriz em um arquivo.
 * 
 * @param head.
 * @param nomeficheiro.
 * @autor Diogo Oliveira
 */
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
#pragma endregion 


#pragma region Carregar a Matriz através de um Ficheiro de Texto
/**
 * @brief Carrega uma matriz de um arquivo.
 * 
 * @param head.
 * @param nomeficheiro.
 * @autor Diogo Oliveira
 * 
 */
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
#pragma endregion


#pragma region Inserir Nova Linha e Coluna na Matriz
/**
 * @brief Insere uma nova linha e uma nova coluna na matriz.
 * 
 * @param matriz (Ponteiro para o nó da lista ligada contendo a matriz).
 * @param novaPosicao (Nova posição da linha e coluna a serem inseridas).
 * @autor Diogo Oliveira
 * 
 */
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
#pragma endregion



#pragma region Remover Linha
/**
 * @brief Remove uma linha
 * 
 * @param matriz.
 * @param posicao.
 * @autor Diogo Oliveira
 * 
 */
void removerLinha(struct No* matriz, int posicao) {
    int index = posicao - 1;
    int rows = matriz->rows;
    int cols = matriz->cols;

    // Verificar se a posição é válida
    if (index < 0 || index >= rows || index >= cols) {
        printf("Posicao invalida.\n");
        return;
    }

    // Alocar espaço para a nova matriz com uma linha  a menos
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

#pragma region Listar de forma Tabular
/**
 * @brief Lista a matriz de forma tabular.
 * 
 * @param nomeficheiro.
 * @autor DiogoOliveira
 * 
 */
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
#pragma endregion

#pragma region Verificar se está no Array
/**
 * @brief Verifica se um número está contido em um array.
 *
 * @param arr (Array onde será feita a presquisa).
 * @param size (Tamanho do array).
 * @param num (Número a ser precurado).
 * @return true (se o número estiver no array).
 * @return false (se o número nao estiver no array).
 * @autor Diogo Oliveira
 *
 */
bool contains(int* arr, int size, int num) {
    for (int i = 0; i < size; i++) {
        if (arr[i] == num) {
            return true;
        }
    }
    return false;
}
#pragma endregion


#pragma region Encontrar a soma máxima e os números utilizados
/**
 * @brief Função auxiliar  para encontrar a soma máxima e os números utilizados.
 *
 * @param matriz Matriz (Onde será feita a pesquisa).
 * @param rows.
 * @param cols.
 * @param row (Linha atual).
 * @param selectedNumbers (Array de números selecionados de cada linha).
 * @param maxSumResult (Ponteiro para a soma máxima encontrada).
 * @param currentSum (Soma atual).
 * @param bestSelectedNumbers (Array de números selecionados para fazer a soma máxima.
 * @autor Diogo Oliveira
 *
 */
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
#pragma endregion


#pragma region Calcular a Soma Máxima Possível
/**
 * @brief Calcula a soma máxima possível em uma matriz e imprime os números selecionados.
 *
 * @param matriz Matriz (Onde será feita a pesquisa).
 * @param rows.
 * @param cols.
 * @return maxSumResult.
 * @autor Diogo Oliveira
 *
 */
int maxSum(int** matriz, int rows, int cols) {
    // Array para procurar os números selecionados em cada linha
    int* selectedNumbers = (int*)malloc(rows * sizeof(int));
    int* bestSelectedNumbers = (int*)malloc(rows * sizeof(int));
    for (int i = 0; i < rows; i++) {
        selectedNumbers[i] = -1; // Inicializa com -1, indicando que nenhuma seleção foi feita
        bestSelectedNumbers[i] = -1; // Inicializa com -1, indicando que nenhuma seleção foi feita
    }

    // Variável para armazenar a soma máxima encontrada
    int maxSumResult = 0;

    // Chamada da função findMaxSum para encontrar a soma máxima
    findMaxSum(matriz, rows, cols, 0, selectedNumbers, &maxSumResult, 0, bestSelectedNumbers);

    
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

#pragma region Criar a Matriz com Valores Inseridos Manualmente
/**
 * @brief Insere valores manualmente em uma nova matriz.
 * 
 * @param head.
 * @autor Diogo Oliveira
 * 
 */

void inserirValoresManualmente(struct No** head) {
    int rows, cols;

    // Verifica se a lista está vazia
    if (*head == NULL) {
        printf("Nenhuma matriz encontrada\n");
        return;
    }

    // Obtém as dimensões da matriz da primeira célula da lista
    rows = (*head)->rows;
    cols = (*head)->cols;

    // Aloca espaço para a matriz
    int** matriz = (*head)->matriz;


    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            int valor;
            do {
                printf("Insira o valor para a posicao [%d][%d]: ", i, j);
                scanf_s("%d", &valor);
                if (valor < 1 || valor > 999) {
                    printf("Valor fora do intervalo permitido (1 a 999).\n");
                }
            } while (valor < 1 || valor > 999);
            matriz[i][j] = valor;
        }
    }

    printf("Valores inseridos na matriz.\n");
}

#pragma endregion




