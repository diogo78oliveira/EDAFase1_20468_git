/*******************************************************************************************************************
* @file matriz.c
* @author Diogo Oliveira (a20468@alunos.ipca.pt)
* @brief matriz cont�m a implementa��o das fun��es relacionadas � manipula��o de matrizes e lista ligada.
 * Inclui fun��es para criar matrizes, inserir e limpar matrizes de uma lista ligada, visualizar matrizes,
 * guardar e carregar matrizes de/para um arquivo, calcular a soma m�xima poss�vel em uma matriz, inserir valores
 * manualmente em uma matriz, inserir linhas e colunas, remover colunas e linhas, alterar um valor, entre outras funcionalidades.
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
  * @brief Cria uma nova matriz com valores aleat�rios entre 1 e 999.
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
 * @brief Limpa a memoria que a matriz est� a utilizar.
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
 * @param head (Ponteiro para o in�cio da lista ligada). 
 * @param matriz (Matriz a ser inserida).
 * @param rows.
 * @param cols.
 * @return nonoNo (Ponteiro para o novo n� inserido na lista ligada).
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


#pragma region Imprimir Matriz
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
    printf("Numero de linhas: %d\n", rows);
    printf("Numero de colunas: %d\n", cols);
    printf("Matriz:\n");
    printf("    ");
    for (int j = 0; j < cols; j++) {
        printf("%5d ", j + 1); // Enumerar as colunas
    }
    printf("\n");
    printf("  ");
    for (int j = 0; j < cols; j++) {
        printf("------"); 
    }
    printf("\n");
    for (int i = 0; i < rows; i++) {
        printf("%2d |", i + 1); // Enumerar as linhas
        for (int j = 0; j < cols; j++) {
            printf("%5d ", matriz[i][j]);
        }
        printf("\n");
    }
}
#pragma endregion


#pragma region Limpar Lista Ligada
/**
 * @brief Limpa a lista ligada e limpa a mem�ria atrav�s da fun�ao 'LimparMatriz'.
 * 
 * @param head.
 * @autor Diogo Oliveira
 * 
 */
void limparLL(struct No* head) {    //'temp' utilizado por precau�ao para limpar corretamente a memoria
    struct No* current = head;      //ponteiro 'current' utilizado para percorrer a lista ligada e apontar para o n� atual    
    while (current != NULL) {
        struct No* temp = current;
        current = current->prox;
        LimparMatriz(temp->matriz, temp->rows);
        free(temp);
    }
}
#pragma endregion


#pragma region Guardar Matriz
/**
 * @brief Salva a matriz em um ficheiro de texto.
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


#pragma region Carregar a Matriz
/**
 * @brief Carrega uma matriz de um ficheiro de texto.
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

    
    fclose(file);

    // Insere a matriz na lista ligada
    *head = inserirMatriz(*head, matriz, rows, cols);
}
#pragma endregion


#pragma region Inserir Linha
/**
 * @brief Insere uma nova linha na matriz.
 * 
 * @param matriz (Ponteiro para o n� da lista ligada contendo a matriz).
 * @autor Diogo Oliveira
 * 
 */
void inserirNovaLinha(struct No* matriz) {
    int rows = matriz->rows;
    int cols = matriz->cols;

    // Alocar espa�o para a nova matriz
    int** novaMatriz = (int**)malloc((rows + 1) * sizeof(int*));
    for (int i = 0; i < rows; i++) {
        novaMatriz[i] = (int*)malloc(cols * sizeof(int));
        for (int j = 0; j < cols; j++) {
            novaMatriz[i][j] = matriz->matriz[i][j];
        }
        free(matriz->matriz[i]);
    }
    free(matriz->matriz);

    // Preencher a nova linha
    novaMatriz[rows] = (int*)malloc(cols * sizeof(int));
    for (int j = 0; j < cols; j++) {
        novaMatriz[rows][j] = rand() % 999 + 1;
    }

    // Atualiza os dados da matriz 
    matriz->matriz = novaMatriz;
    matriz->rows++;
}

#pragma endregion


#pragma region Inserir Coluna
/**
 * @brief Insere uma nova coluna na matriz.
 *
 * @param matriz (Ponteiro para o n� da lista ligada contendo a matriz).
 * @autor Diogo Oliveira
 *
 */
void inserirNovaColuna(struct No* matriz) {
    int rows = matriz->rows;
    int cols = matriz->cols;

    // Alocar espa�o para a nova matriz
    int** novaMatriz = (int**)malloc(rows * sizeof(int*));
    for (int i = 0; i < rows; i++) {
        // Alocar espa�o para uma coluna adicional na nova linha
        novaMatriz[i] = (int*)malloc((cols + 1) * sizeof(int));
        // Copiar os valores da matriz original para a nova matriz
        for (int j = 0; j < cols; j++) {
            novaMatriz[i][j] = matriz->matriz[i][j];
        }
        // Preencher a nova coluna com valores aleat�rios
        novaMatriz[i][cols] = rand() % 999 + 1;
        // Liberar a mem�ria da linha anterior da matriz original
        free(matriz->matriz[i]);
    }
    // Liberar a mem�ria da matriz original
    free(matriz->matriz);

    // Atualiza os dados da matriz 
    matriz->matriz = novaMatriz;
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
    int pos = posicao - 1;
    int rows = matriz->rows;
    int cols = matriz->cols;

    // Verificar se a posi��o � v�lida
    if (pos < 0 || pos >= rows) {
        printf("Posicao invalida.\n");
        return;
    }

    // Alocar espa�o para a nova matriz com uma linha a menos
    int** novaMatriz = (int**)malloc((rows - 1) * sizeof(int*));
    for (int i = 0, k = 0; i < rows; i++) {
        if (i != pos) {
            novaMatriz[k] = (int*)malloc(cols * sizeof(int));
            for (int j = 0; j < cols; j++) {
                novaMatriz[k][j] = matriz->matriz[i][j];
            }
            k++;
        }
    }

    // Free  matriz antiga
    for (int i = 0; i < rows; i++) {
        free(matriz->matriz[i]);
    }
    free(matriz->matriz);

    // Atualiza os dados da matriz na estrutura
    matriz->matriz = novaMatriz;
    matriz->rows--;
}
#pragma endregion


#pragma region Remover Coluna
/**
 * @brief Remove uma coluna
 *
 * @param matriz.
 * @param posicao.
 * @autor Diogo Oliveira
 *
 */
void removerColuna(struct No* matriz, int posicao) {
    int pos = posicao - 1;
    int rows = matriz->rows;
    int cols = matriz->cols;

    // Verificar se a posi��o � v�lida
    if (pos < 0 || pos >= cols) {
        printf("Posicao invalida.\n");
        return;
    }

    // Alocar espa�o para a nova matriz com uma coluna a menos
    int** novaMatriz = (int**)malloc(rows * sizeof(int*));
    for (int i = 0; i < rows; i++) {
        novaMatriz[i] = (int*)malloc((cols - 1) * sizeof(int));
        for (int j = 0, k = 0; j < cols; j++) {
            if (j != pos) {
                novaMatriz[i][k++] = matriz->matriz[i][j];
            }
        }
    }

    // Free matriz antiga
    for (int i = 0; i < rows; i++) {
        free(matriz->matriz[i]);
    }
    free(matriz->matriz);

    // Atualiza os dados da matriz na estrutura
    matriz->matriz = novaMatriz;
    matriz->cols--;
}
#pragma endregion


//Fun�ao s� consegue fazer a soma at� 9 valores

#pragma region Verificar se est� no Array
/**
 * @brief Verifica se um n�mero est� contido em um array.
 *
 * @param arr (Array onde ser� feita a presquisa).
 * @param size (Tamanho do array).
 * @param num (N�mero a ser precurado).
 * @return true (se o n�mero estiver no array).
 * @return false (se o n�mero nao estiver no array).
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


#pragma region Procurar a soma m�xima e os n�meros utilizados
/**
 * @brief Fun��o auxiliar  para procurar a soma m�xima e os n�meros utilizados.
 *
 * @param matriz Matriz (Onde ser� feita a pesquisa).
 * @param rows.
 * @param cols.
 * @param row (Linha atual).
 * @param selectedNumbers (Array de n�meros selecionados de cada linha).
 * @param maxSumResult (Ponteiro para a soma m�xima encontrada).
 * @param currentSum (Soma atual).
 * @param bestSelectedNumbers (Array de n�meros selecionados para fazer a soma m�xima.
 * @autor Diogo Oliveira
 *
 */
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
#pragma endregion


#pragma region Calcular a Soma M�xima 
/**
 * @brief Calcula a soma m�xima poss�vel em uma matriz e imprime os n�meros selecionados.
 *
 * @param matriz Matriz (Onde ser� feita a pesquisa).
 * @param rows.
 * @param cols.
 * @return maxSumResult.
 * @autor Diogo Oliveira
 *
 */
int maxSum(int** matriz, int rows, int cols) {
    // Array para procurar os n�meros selecionados em cada linha
    int* selectedNumbers = (int*)malloc(rows * sizeof(int));
    int* bestSelectedNumbers = (int*)malloc(rows * sizeof(int));
    for (int i = 0; i < rows; i++) {
        selectedNumbers[i] = -1; // Inicializa com -1, indicando que nenhuma sele��o foi feita
        bestSelectedNumbers[i] = -1; // Inicializa com -1, indicando que nenhuma sele��o foi feita
    }

    // Vari�vel para armazenar a soma m�xima encontrada
    int maxSumResult = 0;

    // Chamada da fun��o findMaxSum para encontrar a soma m�xima
    findMaxSum(matriz, rows, cols, 0, selectedNumbers, &maxSumResult, 0, bestSelectedNumbers);

    
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
#pragma endregion


#pragma region Criar a Matriz com Valores Inseridos Manualmente
/**
 * @brief Insere valores manualmente em uma nova matriz utilizando as dimensoes da matriz j� carregada.
 * 
 * @param head.
 * @autor Diogo Oliveira
 * 
 */

void inserirValoresManualmente(struct No** head) {
    int rows, cols;

    // Verifica se a lista est� vazia
    if (*head == NULL) {
        printf("Nenhuma matriz encontrada\n");
        return;
    }

    // Obt�m as dimens�es da matriz da primeira c�lula da lista
    rows = (*head)->rows;
    cols = (*head)->cols;

    // Aloca espa�o para a matriz
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


#pragma region Alterar Valor
/**
 * @brief Altera manualmente o valor em uma posi��o espec�fica da matriz.
 *
 * @param head (Ponteiro para o n� da lista ligada contendo a matriz).
 * @param linha (Linha onde o valor ser� alterado).
 * @param coluna (Coluna onde o valor ser� alterado).
 * @param novoValor (Novo valor a ser inserido na posi��o especificada).
 * @autor Diogo Oliveira
 *
 */
void alterarValor(struct No** head, int linha, int coluna, int novoValor) {
    // Verifica se a lista est� vazia
    if (*head == NULL) {
        printf("Nenhuma matriz encontrada\n");
        return;
    }

    // Obt�m as dimens�es da matriz da primeira c�lula da lista
    int rows = (*head)->rows;
    int cols = (*head)->cols;

    
    if (linha < 1 || linha > rows || coluna < 1 || coluna > cols) {
        printf("Linha ou coluna invalida.\n");
        return;
    }

    
    if (novoValor < 1 || novoValor > 999) {
        printf("Valor fora do intervalo permitido (1 a 999).\n");
        return;
    }

    // Obt�m a matriz da primeira c�lula da lista
    int** matriz = (*head)->matriz;

    // Altera o valor na posi��o 
    matriz[linha - 1][coluna - 1] = novoValor;

    printf("Valor alterado na matriz.\n");
}
#pragma endregion


