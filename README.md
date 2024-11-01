# 🟦🟩🟥🟨 GPU-lib 🟩🟨🟥🟦
Biblioteca com funções gráficas para o Processador Gráfico CoLenda, que permitem facilitar a implementação de jogos digitais para a plataforma DE1-SOC.

# Equipe de desenvolvimento
[Anesio Sousa](https://github.com/AnesioSousa)

[Caick Wendel](https://github.com/Caickk)

[Caike Dylon](https://github.com/Ckdylon)

# Descrição do problema
Foi implementado uma bibloioteca usando a liguagem assembly, desenvolvendo assim uma interface de conexão entre HPS e FPGA da DE1-SoC. sendo esse trabalho focado no desenvolvimento dessa conexão, unma vez que ao referente trabalho do Gabriel Sá Barreto Alves, desenvolvedor do processador grafico utilizado processador capaz de desenhar poligonos convexos, gerenciamento de sprites, porem o projeot original usa a NIOS II como processador principal, mas, na placa DE1-SoC adpta para ultilização do seu processador próprio HPS. O sistema é comandado pela placa DE1-SoC, sendo capaz de gerar imagem em um monitor via conexão VGA. O projetofoi desenvolvido acessando componetes do Processador Gráfico através de mapeamento de memória.

# Recursos ultilizados
O projeto foi desenvolvido na placa Intel® DE1-SoC, uma plataforma de grande versatilidade voltada para experimentações em organização de computadores e sistemas embarcados. Sua arquitetura é formada por dois componentes principais: o Hard Processor System (HPS) e o FPGA, ambos integrados no chip Cyclone® V SoC.

O HPS contém um processador ARM Cortex A9 dual-core, encarregado de executar o processamento de dados. Este sistema inclui também uma memória DDR3 e diversos dispositivos periféricos. O FPGA, por outro lado, hospeda dois processadores Intel Nios® II e possui várias portas periféricas, como memória, temporizadores, módulos de áudio, vídeo, PS/2, conversão analógica-digital e interfaces paralelas para interruptores e LEDs.

O sistema operacional utilizado é o Linux, e o desenvolvimento de software foi realizado com o uso das linguagens C e assembly, utilizando um editor de código que permite trabalhar com várias linguagens.

# Compilar e executar o game
Para rodar o game basta fazer o download do repositório e com um terminal aberto na pasta "app" executar o seguinte comando:
```
$ make
```
<h3> ⚠ Atenção! O sistema também depende da instalação prévia da biblioteca IntelFPGAUP!</h31>

<br>
<br>
<br>
<br>
<br>
<br>

## Referências:
NAJIBGHADRI. Disponivel em: <https://github.com/najibghadri/Tetris200lines/tree/master>. Acesso em: 03/09/2024
<br>
VIMFULDANG. Disponivel em: <https://github.com/VimfulDang/ADXL345-Accelerometer-DE1-SOC>. Acesso em: 20/09/2024
