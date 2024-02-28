#ifndef MATRIZ_H
#define MATRIZ_H

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
void removerLinhaColuna(struct No* matriz, int posicao);
void listarMatriz(const char* nomeficheiro);
int maxSum(int** matriz, int rows, int cols);
void inserirValoresManualmente(struct No** head);

#endif 
