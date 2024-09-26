/*
 * Universidade Estadual de Feira de Santana
 * TEC499 - MI - SISTEMAS DIGITAIS - 2024.2
 * Engenharia de Computação - Departamento de Tecnologia (DTEC)
 * Discentes: Anésio Neto, Caick Wendell, Caike Dylon
 * Docente: Ângelo Duarte
 **/

#include "./src/tetris/tetris.h"
#include "./src/acelerometro/acelerometro.h"
#include "./address_map_arm.h"
#include "pthread.h"

void *input(void **args);
void catchSIGINT(int signum);

static void *i2c0base_virtual, *sysmgrbase_virtual;
static int fd_i2c0base = -1, fd_sysmgr = -1;
volatile sig_atomic_t stop;

int main()
{
	/* Definição das peças */
	const Forma VetorDeFormas[7] = {
		// Formato S
		{
			(char *[]){
				(char[]){0, 1, 1},
				(char[]){1, 1, 0},
				(char[]){0, 0, 0}},
			3},

		// Formato Z
		{
			(char *[]){
				(char[]){1, 1, 0},
				(char[]){0, 1, 1},
				(char[]){0, 0, 0}},
			3},

		// Formato T
		{
			(char *[]){
				(char[]){0, 1, 0},
				(char[]){1, 1, 1},
				(char[]){0, 0, 0}},
			3},

		// Formato L
		{
			(char *[]){
				(char[]){0, 0, 1},
				(char[]){1, 1, 1},
				(char[]){0, 0, 0}},
			3},

		// Formato L invertido
		{
			(char *[]){
				(char[]){1, 0, 0},
				(char[]){1, 1, 1},
				(char[]){0, 0, 0}},
			3},

		// Formato quadrado
		{
			(char *[]){
				(char[]){1, 1},
				(char[]){1, 1}},
			2},

		// Formato vareta
		{
			(char *[]){
				(char[]){0, 0, 0, 0},
				(char[]){1, 1, 1, 1},
				(char[]){0, 0, 0, 0},
				(char[]){0, 0, 0, 0}},
			4}};

	/* Peça que cai */
	Forma forma_atual;
	/* "Motor" do game */
	char jogoEstaExecutando = TRUE, c;
	int pontuacao = 0, decrementar = 1000, direcao, x1, y1, x2, y2, i, j;

	uint8_t devid = 0;
	int16_t XYZ[3];

	srand(time(0));
	gettimeofday(&before_now, NULL);

	signal(SIGINT, catchSIGINT);
	GerarNovaFormaAleatoriamente();

	// Configure MUX to connect I2C0 controller to ADXL345
	if ((fd_sysmgr = open_physical(fd_sysmgr)) == -1)
		return (-1);

	if ((sysmgrbase_virtual = map_physical(fd_sysmgr, SYSMGR_BASE, SYSMGR_SPAN)) == NULL)
		return (-1);

	sysmgr_base_ptr = (unsigned int *)sysmgrbase_virtual;

	printf("%p\n", sysmgr_base_ptr);
	printf("Starting mux_init()\n");
	mux_init();
	printf("Finished mux_init()\n");

	if ((fd_i2c0base = open_physical(fd_i2c0base)) == -1)
		return (-1);

	if ((i2c0base_virtual = map_physical(fd_i2c0base, I2C0_BASE, I2C0_SPAN)) == NULL)
		return (-1);

	i2c0_base_ptr = (unsigned int *)i2c0base_virtual;
	printf("Starting I2C0_Init()\n");
	I2C0_Init();
	printf("Getting ID\n");
	ADXL345_IdRead(&devid);
	printf("%#x\n", devid);

	ExibirTabela();

	if (devid == 0xE5)
	{
		stop = 0;
		printf("Found ADXL345\n");
		ADXL345_init();
		while (!stop)
		{
			if (ADXL345_IsDataReady())
			{
				ADXL345_XYZ_Read(XYZ);
				transform_xyz(&XYZ, 3.2);

				if (XYZ[0] < 0)
					MovimentarForma('a', forma_atual);
				else if (XYZ[0] > 0)
					MovimentarForma('s', forma_atual);

				// Verifica se o tempo passou para fazer a peça cair automaticamente
				gettimeofday(&now, NULL);
				if (temQueAtualizar())
				{ // diferença de tempo dada em precisão de milisegundos
					MovimentarForma('s', forma_atual);
					gettimeofday(&before_now, NULL);
				}
				ApagarForma(forma_atual);

				/*
				for (i = 0; i < LINHAS; i++)
				{
					for (j = 0; j < COLUNAS; j++)
						printf("%c ", Matriz[i][j] ? '#' : ' ');
					printf("\n");
				}*/
				printf("\nGame over!\n");
				printf("\nPontuacao: %d\n", pontuacao);
				// reiniciar game aqui
			}
		}
	}

	unmap_physical(i2c0base_virtual, I2C0_SPAN);
	close_physical(fd_i2c0base);
	unmap_physical(sysmgrbase_virtual, SYSMGR_SPAN);
	close_physical(fd_sysmgr);
}

void catchSIGINT(int signum)
{
	printf("Unmapping\n");
	stop = 1;
}

void transform_xyz(int16_t *data, float mg_per_lsb)
{
	int i;
	for (i = 0; i < 3; i++)
		data[i] *= mg_per_lsb;

	return data;
}