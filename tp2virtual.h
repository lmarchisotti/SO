#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

// --------------- Estrutura da pagina ---------------

    typedef struct no{
        struct no *next;
        char operation;     // Tipo de operacao
        int modify;         // Identificacao se foi modificado
        int refcount;       // Contagem de referenciamento
        int physicaddress;  // Enderecamento
        int lastaccess;     // Marcacao de ultimo acesso
        long int page;      // Pagina
    } cell;

// ---------------- Variaveis Globais ----------------

    #define KByte 1024
    #define N 30

    char filelocation[N];   // Inicializacao - endereço dos arquivos .log
	char file[N];           // Inicializacao - arquivo .log escolhido
    char subpolicy[N];      // Inicializacao - politica de substituicao escolhida
    int pagesize;           // Inicializacao - tamanho da pagina
    int memsize;            // Inicializacao - tamanho da memoria
    int clockT;             // Contagem de tempo para verificar as paginas mais antigas
    long int hits;     	    // Numero de acertos
    long int faults;   	    // Numero de erros de leitura
    long int *physicmemo;   // Vetor 


// --------------------- Funcoes ---------------------

//calcula a pagina a partir do endereço
int shiftPage( );

// --------------- Leitura dos arquivos --------------
void readFile( cell *instance, int shift );

// ------ Conversao dos endereços Hex para Dec -------
long int HexToDec( char hex[8] );

// ------ Busca por posicao vazia na memoria --------
int NextEmptySlot( );

// ------------------- Politica LRU ------------------
void lru( cell *instance );

// ------------------- Politica NRU ------------------
void nru( cell *instance, int arrayTime[] );

// ------------- Politica Segunda Chance -------------
void segundaChance( cell *instance, int arrayTime[] );

// -------- Insere pagina na tabela invertida --------
void insertPage( cell *instance, int arrayTime[], int hash, long int page, char rw );

// -------- Remove pagina na tabela invertida --------
void removePage( cell *instance, cell *minimo, int pos, int subpol );

// ----------- Mostra a tabela resumo final ----------
void printOut ( );
