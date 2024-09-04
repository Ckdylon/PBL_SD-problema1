#include <stdio.h>
#include "ranking.h"

char jogador[4] = {'A', 'A', 'A', '\0'};

void mostrar_tela_inicial() {
    exibir_ranking();
    selecionar_jogador();
}

void selecionar_jogador() {
    int letra_atual = 0;
    int posicao_nome = 0;

    //posibilidade de escolha de um nome
    while (posicao_nome < 3) {
        //escolher uma letra e confirmar
        jogador[posicao_nome] = 'A' + letra_atual;
        posicao_nome++;
    }

    confirmar_nome(jogador);
}

int iniciar_jogo() {
    //verificar se o botão "play" foi pressionado
    return 1; //retorna 1 quando for precionado par ir para proxima tela.
}

void confirmar_nome(char nome[]) {
}
