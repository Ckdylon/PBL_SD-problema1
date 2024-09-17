/*
* Universidade Estadual de Feira de Santana
* Departamento de Tecnologia
*
**/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <ncurses.h>

#define LINHAS 20 
#define COLUNAS 15
#define TRUE 1
#define FALSE 0

typedef struct {
    char **array;
    int largura, linha, coluna;
} Forma;

Forma forma_atual;

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

Forma CopiarForma(Forma forma);
void MovimentarForma(int direcao);
void ExibirTabela();
void RemoverLinhaEAtualizarPontuacao();
void SobrescreverMatriz();
void GerarNovaFormaAleatoriamente();
int ChecarPosicao(Forma forma);
void ApagarForma(Forma forma);
int temQueAtualizar();

int main() {
	int direcao;

	pontuacao = 0;
    srand(time(0));
    initscr();
	gettimeofday(&before_now, NULL);
	timeout(1);

	GerarNovaFormaAleatoriamente();
    ExibirTabela();

	while(jogoEstaExecutando){
		if ((direcao = getch()) != ERR) {
		  MovimentarForma(direcao);
		}
		gettimeofday(&now, NULL);
		if (temQueAtualizar()) { //diferença de tempo dada em precisão de milisegundos
			MovimentarForma('s');
			gettimeofday(&before_now, NULL);
		}
	}
	ApagarForma(forma_atual);
	endwin();
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
	nova_forma.array = (char**)malloc(nova_forma.largura*sizeof(char*));
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
	clear();
	for(i=0; i<COLUNAS-9; i++)
		printw(" ");
	printw("Tetris\n");
	for(i = 0; i < LINHAS ;i++){
		for(j = 0; j < COLUNAS ; j++){
			printw("%c ", (Matriz[i][j] + Buffer[i][j])? '#': '.');
		}
		printw("\n");
	}
	printw("\nPontuacao: %d\n", pontuacao);
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
