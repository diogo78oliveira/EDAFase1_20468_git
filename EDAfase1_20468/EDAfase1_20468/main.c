/*******************************************************************************************************************
* @file main.c
* @author Diogo Oliveira (a20468@alunos.ipca.pt) 
* @brief main responsável para fazer a intereçao com o utilizador e a chamada das funçoes 
* @date fevereiro 2024 * 
* 
* @copyright Copyright (c) 2024     
* 
*******************************************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "matriz.h"


#pragma region Menu
/**
* 
 * @brief Menu do programa.
 * @author Diogo Oliveira
 * 
 */
void printMenu() {
    printf("\nSelecione uma opcao:\n");
    printf("1 - Inserir nova matriz\n");
    printf("2 - Ver matriz\n");
    printf("3 - Guardar matriz\n");
    printf("4 - Carregar Matriz\n");
    printf("5 - Inserir nova linha e coluna\n");
    printf("6 - Remover linha e coluna\n");
    printf("7 - Listar matriz de forma tabular\n");
    printf("8 - Soma maxima possivel\n");
    printf("9 - Inserir os valores manualmente\n");
    printf("0 - Sair\n");
    printf("Opcao: ");
}
#pragma endregion


#pragma region Switch Case
/**
* 
 * @brief  Main do programa.
 * @return 
 * @author Diogo Oliveira
 * 
 */
int main() {
    srand(time(NULL));

    int opcao, posicao;
    int rows, cols;
    struct No* head = NULL;

    do {
        printMenu();
        scanf_s("%d", &opcao);

        switch (opcao) {
        case 1:
            // Se já houver uma matriz anterior, limpa a memória antes de inserir uma nova
            if (head != NULL) {
                limparLL(head);
                head = NULL; // Reinicie a lista ligada
            }
            printf("Insira o numero de linhas e colunas: ");
            scanf_s("%d", &rows);
            cols = rows;
            int** matriz = criarMatriz(rows, cols);
            head = inserirMatriz(head, matriz, rows, cols);
            printf("Matriz criada.\n");
            break;
        case 2:
            if (head != NULL) {
                printf("Matriz:\n");
                verMatriz(head->matriz, head->rows, head->cols);
            }
            else {
                printf("Nenhuma matriz encontrada.\n");
            }
            break;
        case 3:
            if (head != NULL) {
                guardarMatriz(head, "matriz.txt");
                printf("Matriz guardada.\n");
            }
            else {
                printf("Primeiro insira ou carrega uma matriz por favor.\n");
            }
            break;
        case 4:
            carregarMatriz(&head, "matriz.txt");
            printf("Matriz carregada.\n");
            break;
        case 5:
            inserirNovaLinhaColuna(head, head->rows + 1);
            printf("Nova linha inserida e coluna inseridas.\n");
            break;
        case 6:
            printf("Insira a posicao da linha a ser removida: ");
            scanf_s("%d", &posicao);
            removerLinha(head, posicao);
            printf("Linha removida.\n");
            break;
        case 7:
            listarMatriz("matriz.txt");
            break;
        case 8:
            if (head != NULL) {
                int result = maxSum(head->matriz, head->rows, head->cols);
                printf("Soma maxima possivel: %d\n", result);
            }        
            else {
                printf("Nenhuma matriz encontrada.\n");
            }
            break;
        case 9:          
            inserirValoresManualmente(&head);
            break;
        case 0:
            printf("A sair ...\n");
            break;
        default:
            printf("Selecione uma opçao válida.\n");
            break;
        }
    } while (opcao != 0);

    limparLL(head);

    return 0;
}
#pragma endregion
