/*******************************************************************************************************************
* @file matriz.h 
* @author Diogo Oliveira (a20468@alunos.ipca.pt) 
* @brief Header matriz para definir as funções e a lista ligada 
* @date fevereiro 2024 *
* 
* @copyright Copyright (c) 2024 
* 
*******************************************************************************************************************/


#ifndef MATRIZ_H
#define MATRIZ_H

/**
 * @brief Estrutura de nó para representar uma matriz na lista ligada.
 *
 * Esta estrutura contém um ponteiro para a matriz, as dimensões da matriz (número de linhas e colunas)
 * e um ponteiro para o próximo nó na lista ligada.
 *
 * @param matriz Ponteiro para a matriz.
 * @param rows Número de linhas na matriz.
 * @param cols Número de colunas na matriz.
 * @param prox Ponteiro para o próximo nó na lista ligada.
 * @author Diogo Oliveira
 *
 */
struct No {
    int** matriz;
    int rows;
    int cols;
    struct No* prox;
};

int** criarMatriz(int rows, int cols);
void LimparMatriz(int** matriz, int rows);
struct No* inserirMatriz(struct No* head, int** matriz, int rows, int cols);
void verMatriz(int** matriz, int rows, int cols);
void limparLL(struct No* head);
void guardarMatriz(struct No* head, const char* nomeficheiro);
void carregarMatriz(struct No** head, const char* nomeficheiro);
void inserirNovaLinhaColuna(struct No* matriz, int novaLinhaColuna);
void removerLinha(struct No* matriz, int posicao);
void listarMatriz(const char* nomeficheiro);
int maxSum(int** matriz, int rows, int cols);
void inserirValoresManualmente(struct No** head);

#endif 
