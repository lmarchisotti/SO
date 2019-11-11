TP Sistemas Operacionais - Simulador de Memoria virtual
Integrantes do grupo:

 Aluno:  Filipi Estevao   	Matricula:  172050104
 Aluno:  Lucas Marchisotti	Matricula:  172050060
 Aluno:  Thayller Maia		Matricula:  172050105

----------------------------------------------------------------------------------------------------
Compilação:
Para compilar é necessário utilizar-se do comando make no terminal

Execucao:
./executavel politica_de_substituicao arquivo.log tamanho_de_pagina  tamanho_da_memoria
./tp2virtual lru compilador.log 2 128

Caso utilizar GDB
gdb --args executablename arg1 arg2 arg3
gdb --args tp2virtual lru compilador.log 2 128

----------------------------------------------------------------------------------------------------
Algoritmos de politica de substituicao:

1- LRU - Least recently used;
2- NRU - Not recently used;
3- Segunda chance - Fifo circular

----------------------------------------------------------------------------------------------------
Arquivos de entrada:

1- compilador.log
2- compressor.log
3- matriz.log
4- simulador.log

----------------------------------------------------------------------------------------------------
Tamanho de pagina:

1-  2 - Menor tanho
2-  4
3-  8
4- 16
5- 32
6- 64 - Maior tamanho

----------------------------------------------------------------------------------------------------
Tamanho de memoria: 

1-   128 - Menor tanho
2-   256
3-   512
4-  1024
5-  2048
6-  4096
7-  8192
8- 16384 - Maior tamanho

----------------------------------------------------------------------------------------------------

