// Biblioteca relacionada as funções do mapa

#include <stdlib.h>
#include <GL/glut.h>

#ifndef SIZE_ROOM
#define SIZE_ROOM 10
#endif

#ifndef NUMBER_ROOM
#define NUMBER_ROOM 10
#endif

// Struct da sala
typedef struct room {
	int coord_x;
	int coord_z;
	struct room *next;
} room;

// Struct da fase
typedef struct phase {
    int size_x, size_z;
	// Valores possíveis: 1 => chão, 2 => parede,  5 => Obstaculo
	// 8 => Saída, 9 => Vida, 0 => nada
    int map[128][128]; 
	int finished;
	int numberRoom;
    room *room_list;
} phase;

/* Definição de Variáveis Globais */
phase *currentPhase = NULL;

/* Definição dos Protótipos das Funções */
void createPhase();
void createRoom();
void createHall();
void createWall();
void createMap();
void buildMap();
void buildPhase();
int verifyMapContent(int, int);
void printMap();
void genereteThingsOnMap();

// Função que inicializa a struct fase
void createPhase() {
	int i, j;
	
	currentPhase = new phase();

	//Define o tamanho do mapa
	currentPhase->size_x = currentPhase->size_z = 128;

    //Seta o mapa para 0
	for (i=0; i < currentPhase->size_x; i++) {
		for (j=0; j < currentPhase->size_z; j++) {
			currentPhase->map[i][j] = 0;
		}
	}

    //Seta a lista com NULL
    currentPhase->room_list = NULL;

	//Seta o numero de salas que o mapa deve ter
	currentPhase->numberRoom = NUMBER_ROOM;

	//Seta para 0 a finished
	currentPhase->finished = 0;

	//Cria um novo mapa
	createMap();
}

// Função para criar uma sala, adicionando um lista de sala e set no mapa como jogável
void createRoom() {
	int i, j, x, z;
	room *new_room = NULL, *aux = NULL;

	do {
		x = rand() % currentPhase->size_x;
	} while (x <= SIZE_ROOM || x >= currentPhase->size_x - SIZE_ROOM - 1);

	do {
		z = rand() % currentPhase->size_z;
	} while (z <= SIZE_ROOM || z >= currentPhase->size_z - SIZE_ROOM - 1);

	i = (x-SIZE_ROOM < 0) ? (0) : (x-SIZE_ROOM);
	while (i <= x+SIZE_ROOM && i < currentPhase->size_x) {
		j = (z-2 < 0) ? (0) : (z-SIZE_ROOM);
		while (j <= z+SIZE_ROOM && j < currentPhase->size_z) {
			currentPhase->map[i][j] = 1;
			j++;
		}
		i++;
	}

	new_room = new room();
	new_room->coord_x = x;
	new_room->coord_z = z;
	new_room->next = NULL;
	
	if (currentPhase->room_list == NULL){
		currentPhase->room_list = new_room;
	} else {
        aux = currentPhase->room_list;
    	while (aux->next != NULL) {
			aux = aux->next;
		}
		aux->next = new_room;
	}
}

// Função para criar corredor entre as salas
void createHall() {
	room *aux = NULL, *aux2 = NULL;
	int i, j;

	aux = currentPhase->room_list;
	for (aux2 = aux->next; aux2 != NULL; aux2 = aux2->next) {
		j = aux->coord_z;
		
		//Criando corredor em X
		if (aux->coord_x < aux2->coord_x) {
			for (i = aux->coord_x; i <= aux2->coord_x; i++) {
				currentPhase->map[i][j] = 1;
			}
		} else {
			for (i=aux->coord_x; i >= aux2->coord_x; i--) {
				currentPhase->map[i][j] = 1;
			}
		}
		
		//Criando corredor em Y
		if (aux->coord_z < aux2->coord_z) {
			for (j = aux->coord_z; j <= aux2->coord_z; j++) {
				currentPhase->map[i][j] = 1;
			}
		} else {
			for (j = aux->coord_z; j >= aux2->coord_z; j--) {
				currentPhase->map[i][j] = 1;
			}	
		}
	}
}

//Função que seta na matriz a localização das paredes
void createWall() {
	int i, j, auxi, auxj;
	int size_x = currentPhase->size_x;
	int size_z = currentPhase->size_z;

	//Percorre a matriz em busca do chão para colocar paredes em volta
	for (i = 1; i < size_x - 1; i++) {
		for (j = 1; j < size_z - 1; j++) {
			if (currentPhase->map[i][j] == 1) {
				//Verifica as bordas da posição da matriz
				for (auxi = i-1; auxi <= i+1; auxi++) {
					for (auxj = j-1; auxj <= j+1; auxj++){
						if (currentPhase->map[auxi][auxj] == 0) {
							currentPhase->map[auxi][auxj] = 2;
						}
					}
				}
			}
		}
	}
}

//Função que monta a matriz do mapa
void createMap() {
	int i, j;

	//Cria number_room salas aleatoriamente
	for (i = 0; i < currentPhase->numberRoom; i++) {
		createRoom();
	}
	
	//Conecta as salas criadas com corredores
	createHall();

	//Cria as paredes em volta dos corredores e salas
	createWall();

	//Gera as "coisas" no jogo
	genereteThingsOnMap();
}

//Gera as "vidas" no mapa
void genereteThingsOnMap(){

	//Laço de População na Matriz do Mapa
	for (int i = 0; i < currentPhase->size_x; i++) {
		for (int j = 0;  j < currentPhase->size_z; j++) {
			int x = (rand() % 100) + 1;
			//Caso a posição seja um chão e o valor do rand tenha sido <= a 5
			//Ele gera uma vida nesta posição
			if(x <= 5 && currentPhase->map[i][j] == 1){
				currentPhase->map[i][j] = 9;
				j = j+3;
				i = i+1;
			}
			//Gera alguns obstaculos no mapa
			if(x > 5 && x < 15 && currentPhase->map[i][j] == 1){
				//Evitando criar obstaculos em corredores Horizontais
				if(currentPhase->map[i+1][j] == 2 && currentPhase->map[i-1][j] == 2)
					continue;

				//Evitando criar obstaculos em corredores Verticais
				if(currentPhase->map[i][j+1] == 2 && currentPhase->map[i][j-1] == 2)
					continue;
				
				currentPhase->map[i][j] = 5;
				i++;
			}

		}
	}

	//Gera uma saída
	int haveExit = 1;
	while(haveExit != 0){
		int iX = rand() % 128;
		int iZ = rand() % 128;
		if(currentPhase->map[iX][iZ] != 0 && currentPhase->map[iX][iZ] != 2){
			currentPhase->map[iX][iZ] = 8;
			printf("Saida em:  %d : %d \n", iX, iZ);
			haveExit = 0;
		}
	}
}

//Função que monta os objetos do mapa
void buildMap() {
	int i, j;

	for (i = 0; i < currentPhase->size_x; i++) {
		for (j = 0;  j < currentPhase->size_z; j++) {
			
			glPushMatrix();

			switch (currentPhase->map[i][j]) {
				case 1:
					//Chão
					glTranslatef(i + 0.5, -0.2, j + 0.5);
					glScalef(1, 0.1, 1);
					glColor3ub(70, 70, 70);	
					glutSolidCube(1);
					break;

				case 2:
					//Paredes
					glTranslatef(i + 0.5, 4, j + 0.5);
					glScalef(1, 8, 1);
					glColor3ub(0, 255, 0);
					glutSolidCube(1);
					break;

				case 5:
					//Obstaculo - Agua ?
					glTranslatef(i + 0.5, -0.2, j + 0.5);
					glScalef(1, 4, 1);
					glColor3ub(112, 207, 255);	
					glutSolidCube(1);
					break;

				case 9:
					//Life
					glTranslatef(i + 0.5, 2, j + 0.5);
					glScalef(0.015, 0.015, 0.015);
					glColor3ub(255,0,0);
					glutSolidSphere(20,10,10);

					glTranslatef(0, -2.2, 0);
					glScalef(66.67, 6.67, 66.67);
					glColor3ub(70,70,70);
					glutSolidCube(1);
					break;

				case 8:
					//Saída
					glTranslatef(i + 0.5, -0.2, j + 0.5);
					glScalef(1, 0.1, 1);
					glColor3ub(245, 51, 196);	
					glutSolidCube(1);
					break;

				default:
					break;
			}

			glPopMatrix();
		}
	}
}

// Função principal que cria o mapa e o chão
void buildPhase() {

	if (currentPhase == NULL || currentPhase->finished == 1) {
		//Cria uma nova fase com salas e corredores
		createPhase();
	}

	// Monta os objetos da cena
	buildMap();
}

//Função que verifica o conteudo do mapa no ponto (X,Z)
int verifyMapContent(int x, int z) {
	return currentPhase->map[x][z];
}

/* Funções de DEBUG */

// Função para imprimir o mapa
void printMap(){
	int i, j;
	for (i=0; i<currentPhase->size_x; i++) {
		for (j=0; j<currentPhase->size_z; j++) {
			switch (currentPhase->map[i][j]) {
				case 0:
					printf("-");
					break;
				default:
					printf("%d", currentPhase->map[i][j]);
			}
		}
		printf("\n");
	}
}