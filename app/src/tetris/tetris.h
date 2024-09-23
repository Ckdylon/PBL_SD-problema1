typedef struct {
    char **array;
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