#include "tp2virtual.h"

// ---------------- Inicializacao de variaveis ----------------

int access = 0;         // Numero de acesso
int readpages = 0;      // Numero de paginas lidas
int writepages = 0;     // Numero de paginas escritas
int i = 0;				// Iterador padrao

// -------------------------- Funcoes -------------------------

int shiftPage( ){								// Calculo do deslocamento de pagina pelo endereço
	int pst = pagesize;
	int i = 0;
	while (pst > 1){
		pst = pst >> 1;
		i++;
	}
	return i;
}

void readFile( cell *instance, int shift ){		// Leitura arquivo entrada, seleciona politicas de substituicao, in
   	hits = 0; 
	faults = 0;
	clockT = 0;
	readpages = 0;
	writepages = 0;

	char Raddress[8];
	char rw;
	int hash;
	int arrayTime[memsize];
	long int decimal, page;

	FILE *read = fopen(filelocation, "r");

	while ( !feof(read) ){
		fscanf( read, "%s %c", Raddress, &rw );
		decimal = HexToDec( Raddress );
		page = decimal >> shift;

 // ----- Se memoria cheia -> "Escolher" politica de substituicao -----
		if ( (NextEmptySlot( ) ) == -1 ){

 // ------------------------------ LRU --------------------------------
			if( strcmp(subpolicy, "lru") == 0 ){
				lru( instance );

 // ------------------------------ NRU --------------------------------
			}else if ( strcmp( subpolicy, "nru" ) == 0 ){
				nru( instance, arrayTime );

 // ------------------------- Segunda Chance --------------------------
			}else if ( strcmp( subpolicy, "segundachance" ) == 0 ){
				segundaChance( instance, arrayTime );

 // --------------------- Senao erro na digitacao ---------------------
			}else{
				printf("Politica de Substituicao nao reconhecida.\n");
				return;
			}
		}
		
		hash = page % memsize;
		if(rw == 'W'){
			writepages++;
		}else if (rw == 'R'){
			readpages++;
		}
		insertPage( instance, arrayTime, hash, page, rw );
		clockT++;
	}
	fclose(read);
}

long int HexToDec(char hex[8]){					// Conversao do endereçamento hexadecimal para decimal
	int value, lenth;
	long long decimal = 0;

	lenth = strlen(hex);
	lenth--;
	for ( i = 0 ; hex[i] != '\0' ; i++ ){
		if (hex[i] >= '0' && hex[i] <= '9'){
			value = hex[i] - 48;
		}
		else if ( hex[i] >= 'a' && hex[i] <= 'f' ){
			value = hex[i] - 97 + 10;
		}
		else if ( hex[i] >= 'A' && hex[i] <= 'F' ){
			value = hex[i] - 65 + 10;
		}
		decimal += value * pow(16, lenth);
		lenth--;
	}
	return decimal;
}

int NextEmptySlot( ){							// Calcula qual a proxima posicao vazia para substituicao/insercao
	for ( int pos = 0 ; pos < memsize ; pos++ ){
		if ( physicmemo[pos] == -1 ){
			return pos;
		}
	}
	return -1;
}

void lru( cell *instance ){								// Politica de substituicao LRU - Least Recently Used

	int leastUsed = __INT_MAX__;
	cell *temp, *smaller = instance;

 // ----- Busca pelo elemento menos utilizado na tabela ------
	for( i = 0 ; i < memsize ; i++ ){
		temp = instance[i].next;
		while( temp != NULL ){
			if( temp->refcount < leastUsed ){
				smaller = temp;
				leastUsed = temp->refcount;
			}
			temp = temp->next;
		}
	}
	removePage( instance, smaller, 0, 1 );
}

void nru( cell *instance, int arrayTime[] ){			// Politica de substituicao NRU - Not Recently Used
	int timeAux;
	int counter = 0;
	int random;
	cell *tmp = (cell *)malloc(memsize * sizeof(cell));

	if( clockT >= ( memsize -1 ) ){
		timeAux = clockT - (memsize -1 );
	}else{
		timeAux = clockT - 1;
	}

 // --------- Vetor de nao utilizados recentemente -----------
	int arrayAux[memsize];
	for( i = 0 ; i < memsize ; i++ ){
		if( (arrayTime[i]/timeAux) == 0 ){
			arrayAux[counter] = i;
			counter++;
		}
	}
	srand ( time(NULL) );
	random = rand( ) % counter;
 
 // ------------ Remove uma posicao aleatoria  ---------------
	removePage( instance, tmp, arrayAux[random], 0 );
	physicmemo[arrayAux[random]] = -1;
	arrayTime[arrayAux[random]] = -1;
}

void segundaChance( cell *instance, int arrayTime[] ){ 	// Politica de substituicao Segunda chance - Fifo circular
	int timeAux;
	int i = 0, auxI = 0;		// Auxiliares no deslocamento do tempo
	long int j = 0, auxJ = 0;	// Auxiliares no deslocamento da memoria
	int counter = 0;
	cell *tmp = (cell*)malloc(memsize * sizeof(cell));

	if( clockT >= (memsize -1) ){
		timeAux = clockT - (memsize - 1);
	}else{
		timeAux = clockT - 1;
	}
	while( counter != memsize ){ 
		if( (arrayTime[0])/timeAux == 1 ){
			arrayTime[0] = 0;
 // ----------------- Deslocamento vetor tempo -----------------
			auxI = arrayTime[0];
			for( i = 0 ; i < memsize-1 ; i++ ){
				arrayTime[i]=arrayTime[i+1];
			}
			arrayTime[memsize-1] = auxI;
 // ---------------- Deslocamento vetor memoria ----------------
			auxJ = physicmemo[0];
			for( j = 0 ; j < memsize-1 ; j++ ){
				physicmemo[j] = physicmemo[j+1];
			}
			physicmemo[memsize-1] = auxJ;
			counter++;
		}else{
			removePage( instance, tmp, 0, 0 );
			arrayTime[0] = -1;
			physicmemo[0] = -1;			
			break;
		}
	}

	if( counter == memsize ){
		removePage( instance, tmp, 0, 1 );
		arrayTime[0] = -1;
		physicmemo[0] = -1;
	}
}

void insertPage( cell *instance, int arrayTime[], int hash, long int page, char rw ){	// Funcao para insercao de pagina na tabela invertida
 
 // ------------ Insercção de paginas na tabela  ---------------	

	int vPoint = 0;
	int physicAddr = 0;
	cell *previous;
	cell *temp = (cell*)malloc(sizeof(cell));
	cell *aux = (cell*)malloc(sizeof(cell));

	if ( instance[hash].next == NULL ){
		faults++;
		physicAddr = NextEmptySlot( );
		physicmemo[physicAddr] = page;
		arrayTime[physicAddr] = clockT;
		temp->physicaddress = physicAddr;
		temp->page = page;
		temp->operation = rw;
		temp->refcount = access;
		temp->next = NULL;
		instance[hash].next = temp;
	}else{
		temp = instance[hash].next;	
		while ( temp != NULL ){
			if ( temp->page == page ){
				vPoint = 1;
				int addr = 0;
				for( int i = 0 ; i < memsize ; i++ ){
					if( physicmemo[i] == page ){
						addr = i;
						break;
					}
				}
				arrayTime[addr] = clockT;
				temp->refcount = access;
				hits++;
				break;
			}else{
				previous = temp;
				temp = temp->next;		
			}
		}

		if ( vPoint == 0 ){
			faults++;
			physicAddr = NextEmptySlot( );;
			physicmemo[physicAddr] = page;
			arrayTime[physicAddr] = clockT;
			aux->page = page;
			aux->physicaddress = physicAddr;
			aux->operation = rw;
			aux->refcount = access;
			aux->next = NULL;
			previous->next = aux;	
		}
	}
	access++;
	
}

void removePage( cell *instance, cell *smaller, int pos, int subpol ){					// Funcao para remover pagina na tabela invertida
 // ------------ Remoção de paginas na tabela  ---------------	
	cell *pageAux, *previous;
	int vPoint, index;

 // -------- Percorre a tabela em busca da pagina  -----------	
	for ( i = 0 ; i < memsize ; i++ ){
		pageAux = instance[i].next;
		previous = instance[i].next;
		while( pageAux != NULL ){
 // ---- Se NRU ou Segunda Chance -> troca a mais antiga  ----	
			if( (pageAux->page == physicmemo[pos]) && (subpol == 0) ){
				vPoint = 1;
				index = i;	
				break;
			}else if( (pageAux->page == smaller->page) && (subpol == 1) ){
				vPoint = 1;
				index = i;
				break;
			}
			previous = pageAux;
			pageAux = pageAux->next;
		}
		if( vPoint == 1 ){
			break;
		}
	}

 // --------- Se for unico elemento ---> remover  ------------	
	if( previous->next == pageAux->next ){
		if( subpol == 1 ){
			physicmemo[(instance[index].next)->physicaddress] = -1;
		}
		instance[index].next = pageAux->next;
	}else{
		if( previous->page == pageAux->page ){
			instance[index].next = pageAux->next;
		}else{
			previous->next = pageAux->next;
		}	
 // ---------- Se LRU ---> colocar posicao vazia  ------------	
		if( subpol == 1 ){
			physicmemo[(instance[index].next)->physicaddress] = -1;
		}
	}
}

void printOut(){	// Mostra os resultados
	printf("Politica de Substituição: %s\n", subpolicy);
	printf("Arquivo de Entrada:%s\n", file);
	printf("Tamanho das Paginas: %d KB\n", pagesize / KByte);
	printf("Tamanho da Memoria: %d KB\n", (memsize * pagesize) / KByte);
	printf("Paginas lidas: %d\n", readpages);
	printf("Paginas escritas: %d\n", writepages);
	printf("Hit pages: %ld\n", hits);
	printf("Fault pages: %ld\n", faults);
}