#include "tela_inicial.h"
#include "tela_jogo.h"
#include "ranking.h"
#include "game_over.h"

int main() {
    while (1) {
        mostrar_tela_inicial();
        //acho que isso esta certo, mas aparentar faltar algo
        if (iniciar_jogo()) {
            iniciar_tela_jogo();
        }
        
        if (verificar_game_over()) {
            mostrar_game_over();
        }
    }
    return 0;
}
