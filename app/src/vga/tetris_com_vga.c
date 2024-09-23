#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include "tetris.h"
#include <time.h>
#include <stdio.h>
#include "./address_map_arm.h"

#define LINHAS 20 
#define COLUNAS 15
#define TRUE 1
#define FALSE 0

#define video_WHITE 0xFFFF
#define video_YELLOW 0xFFE0
#define video_RED 0xF800
#define video_GREEN 0x07E0
#define video_BLUE 0x041F
#define video_CYAN 0x07FF
#define video_MAGENTA 0xF81F
#define video_GREY 0xC618
#define video_PINK 0xFC18
#define video_ORANGE 0xFC00

Forma forma_atual;
int screen_x, screen_y;
int char_x, char_y;


const Forma VetorDeFormas[7]= {
	{(char *[]){(char []){0,1,1},(char []){1,1,0}, (char []){0,0,0}}, 3},                           //formato S     
	{(char *[]){(char []){1,1,0},(char []){0,1,1}, (char []){0,0,0}}, 3},                           //formato Z   
	{(char *[]){(char []){0,1,0},(char []){1,1,1}, (char []){0,0,0}}, 3},                           //formato T   
	{(char *[]){(char []){0,0,1},(char []){1,1,1}, (char []){0,0,0}}, 3},                           //formato L   
	{(char *[]){(char []){1,0,0},(char []){1,1,1}, (char []){0,0,0}}, 3},                           //formato L invertido    
	{(char *[]){(char []){1,1},(char []){1,1}}, 2},                                                 //formato quadrado
	{(char *[]){(char []){0,0,0,0}, (char []){1,1,1,1}, (char []){0,0,0,0}, (char []){0,0,0,0}}, 4} //formato vareta
};

char Matriz[LINHAS][COLUNAS] = {0};
int pontuacao = 0;
char jogoEstaExecutando = TRUE;
suseconds_t temporizador = 400000; // é só diminuir esse valor pro jogo executar mais rápido
int decrementar = 1000;
struct timeval before_now, now;



int main() {
	if(!video_open()){
		return -1;
	}

	video_clear();
	video_read (&screen_x, &screen_y, &char_x, &char_y);
	
	//mapa improvisado
	//video_box(0,0, 150, 239, video_WHITE);
	//video_box(10,0,140,229,video_RED);
	
	
	video_show();

	int direcao;
	char c;
	int x1, y1, x2, y2;
	pontuacao = 0;
	srand(time(0));
	gettimeofday(&before_now, NULL);

	GerarNovaFormaAleatoriamente();
    ExibirTabela();

	while(jogoEstaExecutando){
		// Captura de input (a, d, s para esquerda, direita e baixo) se houver
		if ((direcao = getchar()) != -1) {
		  MovimentarForma(direcao);
		}
		
		// Verifica se o tempo passou para fazer a peça cair automaticamente
		gettimeofday(&now, NULL);
		if (temQueAtualizar()) { // diferença de tempo dada em precisão de milisegundos
			MovimentarForma('s');
			gettimeofday(&before_now, NULL);
		}
	}
	ApagarForma(forma_atual);
	int i, j;
	for(i = 0; i < LINHAS ;i++){
		for(j = 0; j < COLUNAS ; j++){
			printf("%c ", Matriz[i][j] ? '#': '.');
		}
		printf("\n");
	}
	printf("\nGame over!\n");
	printf("\nPontuacao: %d\n", pontuacao);

	return 0;
}

Forma CopiarForma(Forma forma){
	Forma nova_forma = forma;
	char **aux = forma.array;
	nova_forma.array = (char**) malloc(nova_forma.largura*sizeof(char*));
    int i, j;
    for(i = 0; i < nova_forma.largura; i++){
		nova_forma.array[i] = (char*) malloc(nova_forma.largura*sizeof(char));

		for(j=0; j < nova_forma.largura; j++) {
			nova_forma.array[i][j] = aux[i][j];
		}
    }
    return nova_forma;
}

void MovimentarForma(int direcao){
	Forma temp = CopiarForma(forma_atual);
	switch(direcao){
		case 'w':
			RotacionarForma(temp); // rotate clockwise
			if(ChecarPosicao(temp))
				RotacionarForma(forma_atual);
			break;
		case 's':
			temp.linha++;  //move pra baixo
			if(ChecarPosicao(temp))
				forma_atual.linha++;
			else {
				SobrescreverMatriz();
				RemoverLinhaEAtualizarPontuacao();
                GerarNovaFormaAleatoriamente();
			}
			break;
		case 'd':
			temp.coluna++;  //move pra direita
			if(ChecarPosicao(temp))
				forma_atual.coluna++;
			break;
		case 'a':
			temp.coluna--;  //move pra esquerda
			if(ChecarPosicao(temp))
				forma_atual.coluna--;
			break;
	}
	ApagarForma(temp);
	ExibirTabela();
}

void ExibirTabela(){
	char Buffer[LINHAS][COLUNAS] = {0};
	int i, j;
	for(i = 0; i < forma_atual.largura ;i++){
		for(j = 0; j < forma_atual.largura ; j++){
			if(forma_atual.array[i][j])
				Buffer[forma_atual.linha+i][forma_atual.coluna+j] = forma_atual.array[i][j];
		}
	}
	//clear();
	for(i=0; i<COLUNAS-9; i++)
		printf(" ");
	printf("Tetris\n");
	for(i = 0; i < LINHAS ;i++){
		for(j = 0; j < COLUNAS ; j++){
			printf("%c ", (Matriz[i][j] + Buffer[i][j])? '#': '.');
		}
		printf("\n");
	}
	printf("\nPontuacao: %d\n", pontuacao);
}

void RemoverLinhaEAtualizarPontuacao(){
	int i, j, soma, contagem=0;
	for(i=0;i<LINHAS;i++){
		soma = 0;
		for(j=0;j< COLUNAS;j++) {
			soma+=Matriz[i][j];
		}
		if(soma==COLUNAS){
			contagem++;
			int l, k;
			for(k = i;k >=1;k--)
				for(l=0;l<COLUNAS;l++)
					Matriz[k][l]=Matriz[k-1][l];
			for(l=0;l<COLUNAS;l++)
				Matriz[k][l]=0;
			temporizador-=decrementar--;
		}
	}
	pontuacao += 100*contagem;
}

void SobrescreverMatriz(){
	int i, j;
	for(i = 0; i < forma_atual.largura ;i++){
		for(j = 0; j < forma_atual.largura ; j++){
			if(forma_atual.array[i][j])
				Matriz[forma_atual.linha+i][forma_atual.coluna+j] = forma_atual.array[i][j];
		}
	}
}


/*
* Função responsável por gerar uma nova forma aleatoriamente
**/
void GerarNovaFormaAleatoriamente(){ 
	Forma nova_forma = CopiarForma(VetorDeFormas[rand()%7]);

    nova_forma.coluna = rand()%(COLUNAS-nova_forma.largura+1);
    nova_forma.linha = 0;
    ApagarForma(forma_atual);
	forma_atual = nova_forma;
	if(!ChecarPosicao(forma_atual)){
		jogoEstaExecutando = FALSE;
	}
}

/*
* Função responsável por verificar a posicao de uma forma
**/
int ChecarPosicao(Forma forma){ 
	// Pega a referencia pro "desenho" da forma, que num tabuleiro definido por uma matriz é naturalmente tambem uma matriz ou "submatriz".
	char **array = forma.array;
	int i, j;
	for(i = 0; i < forma.largura;i++) {
		for(j = 0; j < forma.largura ;j++){
			if((forma.coluna+j < 0 || forma.coluna+j >= COLUNAS || forma.linha+i >= LINHAS)){ //Fora da borda do game
				if(array[i][j]) 
					return FALSE;
				
			}
			else if(Matriz[forma.linha+i][forma.coluna+j] && array[i][j])
				return FALSE;
		}
	}
	return TRUE;
}

/*
* Função responsável por apagar uma forma
**/
void ApagarForma(Forma forma){
    int i;
    for(i = 0; i < forma.largura; i++){
		free(forma.array[i]);
    }
    free(forma.array);
}

int temQueAtualizar(){
	return ((suseconds_t)(now.tv_sec*1000000 + now.tv_usec) -((suseconds_t)before_now.tv_sec*1000000 + before_now.tv_usec)) > temporizador;
}

void RotacionarForma(Forma forma){ 
	Forma temp = CopiarForma(forma);
	int i, j, k, width;
	width = forma.largura;
	for(i = 0; i < width ; i++){
		for(j = 0, k = width-1; j < width ; j++, k--){
				forma.array[i][j] = temp.array[k][i];
		}
	}
	ApagarForma(temp);
}


