#include <stdio.h>
#include <string.h>

typedef struct {
    char nome[4];
    int pontos;
} Jogador;

Jogador ranking[5];

void exibir_ranking() {
    for (int i = 0; i < 5; i++) {
        printf("%d. %s - %d\n", i+1, ranking[i].nome, ranking[i].pontos);
    }
}

void atualizar_ranking(char nome[], int pontos) {
    //atualizar o ranking com base na pontuação
}
