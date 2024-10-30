# 🟦🟩🟥🟨 GPU-lib 🟩🟨🟥🟦
Biblioteca com funções gráficas para o Processador Gráfico CoLenda, que permitem facilitar a implementação de jogos digitais para a plataforma DE1-SOC.

# Equipe de desenvolvimento
[Anesio Sousa](https://github.com/AnesioSousa)

[Caick Wendel](https://github.com/Caickk)

[Caike Dylon](https://github.com/Ckdylon)

# Descrição do problema
Foi implementado uma bibloioteca usando a liguagem assembly, desenvolvendo assim uma interface de conexão entre HPS e FPGA da DE1-SoC. sendo esse trabalho focado no desenvolvimento dessa conexão, unma vez que ao referente trabalho do Gabriel Sá Barreto Alves, desenvolvedor do processador grafico utilizado processador capaz de desenhar poligonos convexos, gerenciamento de sprites, porem o projeot original usa a NIOS II como processador principal, mas, na placa DE1-SoC adpta para ultilização do seu processador próprio HPS. O sistema é comandado pela placa DE1-SoC, sendo capaz de gerar imagem em um monitor via conexão VGA. O projetofoi desenvolvido acessando componetes do Processador Gráfico através de mapeamento de memória.

# Softwares Ultilizados
#
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
