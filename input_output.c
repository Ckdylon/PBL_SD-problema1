#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#ifndef video_H
#define video_H
//INPUTS
//valores estimados de acordo com video disponivel do lab7 no fpga academy
//valores podem ser alterados de acordo com a necessidade
int centro=0,direita=500,esquerda=-31;
int eixoY= -40;

//funcao que retorna um inteiro de acordo com o valor dado pelo eixo X do acelerometro

int getDirecao(int value){
    //nao move
    if (value==centro)
    {
        return 0;
    }
    //move para esquerda
    else if (value>esquerda && value<centro)
    {
        return -1;
    }
    //move para direita
    else if (value>centro && value<direita)
    {
        return 1;
    }   
}

//funcao que move a peca para baixo de acordo com o valor dado pelo eixo y
int getDirecaoY(int value){
    if (value<eixoY)
    {
        return 1;
    }
    else
    {
        return 0;
    }
    
}

//OUTPUTS

void limparTela(){
    //...
    video_clear();
}

void attBuffer(){
    //...
    video_show();
}