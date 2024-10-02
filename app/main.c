/*
 * Universidade Estadual de Feira de Santana
 * TEC499 - MI - SISTEMAS DIGITAIS - 2024.2
 * Engenharia de Computação - Departamento de Tecnologia (DTEC)
 * Discentes: Anésio Neto, Caick Wendell, Caike Dylon
 * Docente: Ângelo Duarte
 **/

#include "./src/acelerometro/acelerometro.h"
#include "./address_map_arm.h"
#include "pthread.h"
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#define LINHAS 20
#define COLUNAS 15
#define TRUE 1
#define FALSE 0

typedef struct
{
	char** array;
	int largura, linha, coluna;
} Forma;

Forma CopiarForma(Forma forma);
void MovimentarForma(int direcao);
void ExibirTabela();
void RemoverLinhaEAtualizarPontuacao();
void SobrescreverMatriz();
void GerarNovaFormaAleatoriamente();
int ChecarPosicao(Forma forma);
void ApagarForma(Forma forma);
int temQueAtualizar();
void IniciarJogo();
void RotacionarForma(Forma forma);

void* input(void** args);
void catchSIGINT(int signum);
int16_t* transform_xyz(int16_t* data, float mg_per_lsb);

static void* i2c0base_virtual, * sysmgrbase_virtual;
static int fd_i2c0base = -1, fd_sysmgr = -1;

/* Definição das peças */
const Forma VetorDeFormas[7] = {
	{(char* []) { (char[]) { 0,1,1 },(char[]) { 1,1,0 }, (char[]) { 0,0,0 } }, 3},                           //formato S     
	{(char* []) { (char[]) { 1,1,0 },(char[]) { 0,1,1 }, (char[]) { 0,0,0 } }, 3},                           //formato Z   
	{(char* []) { (char[]) { 0,1,0 },(char[]) { 1,1,1 }, (char[]) { 0,0,0 } }, 3},                           //formato T   
	{(char* []) { (char[]) { 0,0,1 },(char[]) { 1,1,1 }, (char[]) { 0,0,0 } }, 3},                           //formato L   
	{(char* []) { (char[]) { 1,0,0 },(char[]) { 1,1,1 }, (char[]) { 0,0,0 } }, 3},                           //formato L invertido    
	{(char* []) { (char[]) { 1,1 },(char[]) { 1,1 } }, 2},                                                 //formato quadrado
	{(char* []) { (char[]) { 0,0,0,0 }, (char[]) { 1,1,1,1 }, (char[]) { 0,0,0,0 }, (char[]) { 0,0,0,0 } }, 4} //formato vareta
};

volatile sig_atomic_t stop;
struct timeval before_now, now;
int decrementar = 1000, pontuacao = 0, direcao, x1, y1, x2, y2, i, j;
char Matriz[LINHAS][COLUNAS] = { 0 }, jogo_esta_rodando;
Forma forma_atual;
suseconds_t temporizador = 400000; // é só diminuir esse valor pro jogo executar mais rápido
int16_t XYZ[3];

int main() {
	stop = 0;
	uint8_t devid = 0;
	srand(time(0));
	gettimeofday(&before_now, NULL);

	signal(SIGINT, catchSIGINT);
	GerarNovaFormaAleatoriamente();

	// Configure MUX to connect I2C0 controller to ADXL345
	if ((fd_sysmgr = open_physical(fd_sysmgr)) == -1) return (-1);
	if ((sysmgrbase_virtual = map_physical(fd_sysmgr, SYSMGR_BASE, SYSMGR_SPAN)) == NULL) return (-1);

	sysmgr_base_ptr = (unsigned int*)sysmgrbase_virtual;

	printf("%p\n", sysmgr_base_ptr);
	printf("Starting mux_init()\n");
	mux_init();
	printf("Finished mux_init()\n");

	if ((fd_i2c0base = open_physical(fd_i2c0base)) == -1) return (-1);
	if ((i2c0base_virtual = map_physical(fd_i2c0base, I2C0_BASE, I2C0_SPAN)) == NULL) return (-1);

	i2c0_base_ptr = (unsigned int*)i2c0base_virtual;
	printf("Starting I2C0_Init()\n");
	I2C0_Init();
	printf("Getting ID\n");
	ADXL345_IdRead(&devid);
	printf("%#x\n", devid);

	if (devid == 0xE5) printf("Found ADXL345\n");

	ADXL345_init();
	while (!stop) {
		if (ADXL345_IsDataReady()) {
			ADXL345_XYZ_Read(XYZ);
			transform_xyz(XYZ, 3.2);

			pontuacao = 0;
			srand(time(0));
			gettimeofday(&before_now, NULL);

			GerarNovaFormaAleatoriamente();
			ExibirTabela();

			if (XYZ[0] < 0) MovimentarForma('a');
			else if (XYZ[0] > 0) MovimentarForma('s');

			gettimeofday(&now, NULL);
			if (temQueAtualizar()) {
				MovimentarForma('s');
				gettimeofday(&before_now, NULL);
			}

			ApagarForma(forma_atual);
			int i, j;
			for (i = 0; i < LINHAS; i++) {
				for (j = 0; j < COLUNAS; j++) {
					printf("%c ", Matriz[i][j] ? '#' : '.');
				}
				printf("\n");
			}
			printf("\nGame over!\n");
			printf("\nPontuacao: %d\n", pontuacao);

			gettimeofday(&now, NULL);
			if (temQueAtualizar()) {
				MovimentarForma('s');
				gettimeofday(&before_now, NULL);
			}
			ApagarForma(forma_atual);
		}
	}

	unmap_physical(i2c0base_virtual, I2C0_SPAN);
	close_physical(fd_i2c0base);
	unmap_physical(sysmgrbase_virtual, SYSMGR_SPAN);
	close_physical(fd_sysmgr);
}

/*
 * Função de utilidade responsável por copiar uma peça
 * @param forma - Peça a ser copiada
 **/
Forma CopiarForma(Forma forma) {
	Forma nova_forma = forma;
	char** aux = forma.array;
	nova_forma.array = (char**)malloc(nova_forma.largura * sizeof(char*));
	int i, j;
	for (i = 0; i < nova_forma.largura; i++) {
		nova_forma.array[i] = (char*)malloc(nova_forma.largura * sizeof(char));
		for (j = 0; j < nova_forma.largura; j++) {
			nova_forma.array[i][j] = aux[i][j];
		}
	}
	return nova_forma;
}

/*
 * Função responsável por realizar toda a movimentação da peça que cai
 * @param direcao - Esquerda, Baixo(descer mais rápido), Direita e Cima(rotação): 'a', 's', 'd' e 'w' respectivamente.
 * Aqui entra o acelerômetro.
 **/
void MovimentarForma(int direcao) {
	Forma temp = CopiarForma(forma_atual);
	switch (direcao) {
	case 'w':
		RotacionarForma(temp); // rotate clockwise
		if (ChecarPosicao(temp)) RotacionarForma(forma_atual);
		break;
	case 's':
		temp.linha++; // move pra baixo
		if (ChecarPosicao(temp)) forma_atual.linha++;
		else {
			SobrescreverMatriz();
			RemoverLinhaEAtualizarPontuacao();
			GerarNovaFormaAleatoriamente();
		}
		break;
	case 'd':
		temp.coluna++; // move pra direita
		if (ChecarPosicao(temp)) forma_atual.coluna++;
		break;
	case 'a':
		temp.coluna--; // move pra esquerda
		if (ChecarPosicao(temp)) forma_atual.coluna--;
		break;
	}
	ApagarForma(temp);
	ExibirTabela();
}

/*
 * Função responsável por exibir no terminal a matriz do game
 **/
void ExibirTabela() {
	/*
	char Buffer[LINHAS][COLUNAS] = {0};
	 int i, j;
	 for (i = 0; i < forma_atual.largura; i++)
	 {
		 for (j = 0; j < forma_atual.largura; j++)
		 {
			 if (forma_atual.array[i][j])
				 Buffer[forma_atual.linha + i][forma_atual.coluna + j] = forma_atual.array[i][j];
		 }
	 }
	 for (i = 0; i < COLUNAS - 9; i++)
		 printf(" ");
	 printf("Tetris\n");
	 for (i = 0; i < LINHAS; i++)
	 {
		 for (j = 0; j < COLUNAS; j++)
		 {
			 printf("%c ", (Matriz[i][j] + Buffer[i][j]) ? '#' : '.');
		 }
		 printf("\n");
	 }
	 printf("\nPontuacao: %d\n", pontuacao);

	 */
}

/*
 * Função responsável por realizar a remoção da linha completa e incrementação da pontuação do jogador
 **/
void RemoverLinhaEAtualizarPontuacao() {
	int i, j, soma, contagem = 0;
	for (i = 0; i < LINHAS; i++) {
		soma = 0;
		for (j = 0; j < COLUNAS; j++) soma += Matriz[i][j];
		if (soma == COLUNAS) {
			contagem++;
			int l, k;
			for (k = i; k >= 1; k--)
				for (l = 0; l < COLUNAS; l++) Matriz[k][l] = Matriz[k - 1][l];
			for (l = 0; l < COLUNAS; l++) Matriz[k][l] = 0;
			temporizador -= decrementar--;
		}
	}
	pontuacao += 100 * contagem;
}

/*
 * Função responsável por "repintar" a matriz do jogo, fazendo com que a peça atual se torne visível
 **/
void SobrescreverMatriz() {
	int i, j;
	for (i = 0; i < forma_atual.largura; i++) {
		for (j = 0; j < forma_atual.largura; j++) {
			if (forma_atual.array[i][j]) Matriz[forma_atual.linha + i][forma_atual.coluna + j] = forma_atual.array[i][j];
		}
	}
}

/*
 * Função responsável por gerar uma nova peça de forma randomica
 **/
void GerarNovaFormaAleatoriamente() {
	Forma nova_forma = CopiarForma(VetorDeFormas[rand() % 7]);
	nova_forma.coluna = rand() % (COLUNAS - nova_forma.largura + 1);
	nova_forma.linha = 0;
	ApagarForma(forma_atual);
	forma_atual = nova_forma;
	if (!ChecarPosicao(forma_atual)) {
		jogo_esta_rodando = FALSE;
	}
}

/*
 * Função responsável por verificar a posicao de uma forma
 * @param forma - Peça a ser apagada
 **/
int ChecarPosicao(Forma forma) {
	char** array = forma.array;
	int i, j;
	for (i = 0; i < forma.largura; i++) {
		for (j = 0; j < forma.largura; j++) {
			if ((forma.coluna + j < 0 || forma.coluna + j >= COLUNAS || forma.linha + i >= LINHAS)) {
				if (array[i][j]) return FALSE;
			}
			else if (Matriz[forma.linha + i][forma.coluna + j] && array[i][j]) return FALSE;
		}
	}
	return TRUE;
}

/*
 * Função responsável por apagar uma forma
 * @param forma - Peça a ser apagada
 **/
void ApagarForma(Forma forma) {
	int i;
	for (i = 0; i < forma.largura; i++) free(forma.array[i]);
	free(forma.array);
}


/*
 * Função responsável por verificar se está na hora de atualizar a exibição
 **/
int temQueAtualizar() {
	return ((suseconds_t)(now.tv_sec * 1000000 + now.tv_usec) - ((suseconds_t)before_now.tv_sec * 1000000 + before_now.tv_usec)) > temporizador;
}

/*
 * Função responsável por rotacionar uma forma
 * @param forma - Peça a ser rotacionada
 **/
void RotacionarForma(Forma forma) {
	Forma temp = CopiarForma(forma);
	int i, j, k, width;
	width = forma.largura;
	for (i = 0; i < width; i++) {
		for (j = 0, k = width - 1; j < width; j++, k--) {
			forma.array[i][j] = temp.array[k][i];
		}
	}
	ApagarForma(temp);
}

void catchSIGINT(int signum) {
	printf("Unmapping\n");
	stop = 1;
}

int16_t* transform_xyz(int16_t* data, float mg_per_lsb) {
	int i;
	for (i = 0; i < 3; i++) data[i] *= mg_per_lsb;
	return data;
}