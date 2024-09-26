#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include "tetris.h"

#include <stdio.h>
#include "../../address_map_arm.h"

char Matriz[LINHAS][COLUNAS] = {0};
suseconds_t temporizador = 400000; // é só diminuir esse valor pro jogo executar mais rápido
struct timeval before_now, now;

/*
 * Função de utilidade responsável por copiar uma peça
 * @param forma - Peça a ser copiada
 **/
Forma CopiarForma(Forma forma)
{
	Forma nova_forma = forma;
	char **aux = forma.array;
	nova_forma.array = (char **)malloc(nova_forma.largura * sizeof(char *));
	int i, j;
	for (i = 0; i < nova_forma.largura; i++)
	{
		nova_forma.array[i] = (char *)malloc(nova_forma.largura * sizeof(char));

		for (j = 0; j < nova_forma.largura; j++)
		{
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
void MovimentarForma(int direcao, Forma forma_atual)
{
	Forma temp = CopiarForma(forma_atual);
	switch (direcao)
	{
	case 'w':
		RotacionarForma(temp); // rotate clockwise
		if (ChecarPosicao(temp))
			RotacionarForma(forma_atual);
		break;
	case 's':
		temp.linha++; // move pra baixo
		if (ChecarPosicao(temp))
			forma_atual.linha++;
		else
		{
			SobrescreverMatriz();
			RemoverLinhaEAtualizarPontuacao();
			GerarNovaFormaAleatoriamente();
		}
		break;
	case 'd':
		temp.coluna++; // move pra direita
		if (ChecarPosicao(temp))
			forma_atual.coluna++;
		break;
	case 'a':
		temp.coluna--; // move pra esquerda
		if (ChecarPosicao(temp))
			forma_atual.coluna--;
		break;
	}
	ApagarForma(temp);
	ExibirTabela();
}

/*
 * Função responsável por exibir no terminal a matriz do game
 **/
void ExibirTabela()
{ /*
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
void RemoverLinhaEAtualizarPontuacao()
{
	int i, j, soma, contagem = 0;
	for (i = 0; i < LINHAS; i++)
	{
		soma = 0;
		for (j = 0; j < COLUNAS; j++)
		{
			soma += Matriz[i][j];
		}
		if (soma == COLUNAS)
		{
			contagem++;
			int l, k;
			for (k = i; k >= 1; k--)
				for (l = 0; l < COLUNAS; l++)
					Matriz[k][l] = Matriz[k - 1][l];
			for (l = 0; l < COLUNAS; l++)
				Matriz[k][l] = 0;
			temporizador -= decrementar--;
		}
	}
	pontuacao += 100 * contagem;
}

/*
 * Função responsável por "repintar" a matriz do jogo, fazendo com que a peça atual se torne visível
 **/
void SobrescreverMatriz()
{
	int i, j;
	for (i = 0; i < forma_atual.largura; i++)
	{
		for (j = 0; j < forma_atual.largura; j++)
		{
			if (forma_atual.array[i][j])
				Matriz[forma_atual.linha + i][forma_atual.coluna + j] = forma_atual.array[i][j];
		}
	}
}

/*
 * Função responsável por gerar uma nova peça de forma randomica
 **/
void GerarNovaFormaAleatoriamente()
{
	Forma nova_forma = CopiarForma(VetorDeFormas[rand() % 7]);

	nova_forma.coluna = rand() % (COLUNAS - nova_forma.largura + 1);
	nova_forma.linha = 0;
	ApagarForma(forma_atual);
	forma_atual = nova_forma;
	if (!ChecarPosicao(forma_atual))
	{
		jogoEstaExecutando = FALSE;
	}
}

/*
 * Função responsável por verificar a posicao de uma forma
 * @param forma - Peça a ser apagada
 **/
int ChecarPosicao(Forma forma)
{
	// Pega a referência pro "desenho" da forma, que num tabuleiro definido por uma matriz é naturalmente tambem uma matriz ou uma "submatriz".
	char **array = forma.array;
	int i, j;
	for (i = 0; i < forma.largura; i++)
	{
		for (j = 0; j < forma.largura; j++)
		{
			if ((forma.coluna + j < 0 || forma.coluna + j >= COLUNAS || forma.linha + i >= LINHAS))
			{ // Fora da borda do game
				if (array[i][j])
					return FALSE;
			}
			else if (Matriz[forma.linha + i][forma.coluna + j] && array[i][j])
				return FALSE;
		}
	}
	return TRUE;
}

/*
 * Função responsável por apagar uma forma
 * @param forma - Peça a ser apagada
 **/
void ApagarForma(Forma forma)
{
	int i;
	for (i = 0; i < forma.largura; i++)
	{
		free(forma.array[i]);
	}
	free(forma.array);
}

/*
 * Função responsável por verificar se está na hora de atualizar a exibição
 **/
int temQueAtualizar()
{
	return ((suseconds_t)(now.tv_sec * 1000000 + now.tv_usec) - ((suseconds_t)before_now.tv_sec * 1000000 + before_now.tv_usec)) > temporizador;
}

/*
 * Função responsável por rotacionar uma forma
 * @param forma - Peça a ser rotacionada
 **/
void RotacionarForma(Forma forma)
{
	Forma temp = CopiarForma(forma);
	int i, j, k, width;
	width = forma.largura;
	for (i = 0; i < width; i++)
	{
		for (j = 0, k = width - 1; j < width; j++, k--)
		{
			forma.array[i][j] = temp.array[k][i];
		}
	}
	ApagarForma(temp);
}
