#include <stdio.h>
#include "pecas.h"

#define ALTURA 24
#define LARGURA 10

int campo_jogo[ALTURA][LARGURA];

void iniciar_tela_jogo() {
    while (!verificar_game_over()) {
        gerar_pecas();
        mover_peca();
        atualizar_tela();
    }
}

void gerar_pecas() {
    //colocar a formar que vai criar a peça
}

void mover_peca() {
    //como vai mover, eu acho que a questão de colizão entra aqui
}

void atualizar_tela() {
    //caso precise para atualizar durante a queda da peça ou no final da queda. tem tambem a colisao 
}

int verificar_game_over() {
    //verifica se a peça passou da linha 20
    return 0; //retorna 1 se for game over
}
