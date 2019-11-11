#include "tp2virtual.h"

// ----------------------- Main ----------------------

int main(int argc, char *argv[]){
	
 // -------------------- Entradas --------------------

	strcpy( subpolicy , argv[1] );
	strcpy( file , argv[2] );
	strcpy( filelocation , "arquivos/" );
	strcat( filelocation , file );
	pagesize = atoi(argv[3]);
	memsize = atoi(argv[4]);

 // ------------------ Conversao ---------------------
	
    pagesize = pagesize * KByte;
	memsize = (memsize * KByte)/pagesize;

 // ------------- Inicializacao padrao ---------------

	cell *instance = (cell *)malloc(memsize * sizeof(cell));
	for ( int i = 0 ; i < memsize ; i++ ){
		instance[i].next = NULL;
		instance[i].operation = ' ';
		instance[i].refcount = 0;
		instance[i].lastaccess = -1;
		instance[i].page = -1;
	}
	
 // -------------- Chamadas de funcoes ---------------

	int shift = shiftPage();
	physicmemo = calloc( memsize, sizeof( long int ) );
	for ( int i = 0 ; i < memsize ; i++ ){
		physicmemo[i] = -1;
	}

	printf("Calculando...\n");
	printf("2 + 2 = ...\n");
	readFile( instance, shift );
    printOut();
	return 0;
}

