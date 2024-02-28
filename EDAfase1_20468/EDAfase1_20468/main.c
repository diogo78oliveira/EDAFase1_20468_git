#include <stdio.h>
#include <stdlib.h>
#include "matriz.h"


void printMenu() {
    printf("\nSelecione uma opcao:\n");
    printf("1 - Inserir nova matriz\n");
    printf("2 - Ver matriz\n");
    printf("3 - Guardar matriz\n");
    printf("4 - Carregar Matriz\n");
    printf("5 - Inserir nova linha e coluna\n");
    printf("6 - Remover linha e coluna\n");
    printf("7 - Listar matriz de forma tabular\n");
    printf("0 - Sair\n");
    printf("Opcao: ");
}

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
            // Se j� houver uma matriz anterior, limpa a mem�ria antes de inserir uma nova
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
            printf("Insira a posicao da linha e da coluna a ser removida: ");
            scanf_s("%d", &posicao);
            removerLinhaColuna(head, posicao);
            printf("Linha e coluna removida.\n");
        case 7:
            listarMatriz("matriz.txt");
        case 0:
            printf("A sair ...\n");
            break;
        default:
            printf("Selecione uma op�ao v�lida.\n");
            break;
        }
    } while (opcao != 0);

    limparLL(head);

    return 0;
}