// Biblioteca relacionada as funções do mapa

#ifndef _MAP_H_
#define _MAP_H_

#include <stdlib.h>
#include <GL/glut.h>

// Definição de constantes do programa
#define SIZE_ROOM 10
#define NUMBER_ROOM 10

// Valores possíveis para o mapa
#define OBSTACLE -2
#define WALL -1
#define NOTHING 0
#define FLOOR 1
#define TRAP 5
#define EXIT 8
#define LIFE_SPHERE 9
#define PLAYABLE 32767

// Struct da sala
typedef struct room {
	int coord_x;
	int coord_z;
	struct room *next;
} room;

// Struct da fase
typedef struct phase {
    int size_x, size_z;
	// Valores possíveis: ver define
    short map[128][128];
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
void createExit();
void genereteThingsOnMap();
void buildPhase();
void buildMap();
void buildFloor();
short verifyMapContent(short, short);
void printMap();

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

	// Set para 0 a área jogável. Finalidade de manter na matriz só coisas do mapa
	for (i=0; i < currentPhase->size_x; i++) {
		for (j=0; j < currentPhase->size_z; j++) {
			if (currentPhase->map[i][j] == PLAYABLE) {
				currentPhase->map[i][j] = 0;
			}
		}
	}
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
			currentPhase->map[i][j] = PLAYABLE;
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
				if (currentPhase->map[i][j] == 0) {
					currentPhase->map[i][j] = FLOOR;
				}
			}
		} else {
			for (i=aux->coord_x; i >= aux2->coord_x; i--) {
				if (currentPhase->map[i][j] == 0) {
					currentPhase->map[i][j] = FLOOR;
				}
			}
		}
		
		//Criando corredor em Y
		if (aux->coord_z < aux2->coord_z) {
			for (j = aux->coord_z; j <= aux2->coord_z; j++) {
				if (currentPhase->map[i][j] == 0) {
					currentPhase->map[i][j] = FLOOR;
				}
			}
		} else {
			for (j = aux->coord_z; j >= aux2->coord_z; j--) {
				if (currentPhase->map[i][j] == 0) {
					currentPhase->map[i][j] = FLOOR;
				}
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
			if (currentPhase->map[i][j] == FLOOR || currentPhase->map[i][j] == PLAYABLE) {
				//Verifica as bordas da posição da matriz
				for (auxi = i-1; auxi <= i+1; auxi++) {
					for (auxj = j-1; auxj <= j+1; auxj++){
						if (currentPhase->map[auxi][auxj] == 0) {
							currentPhase->map[auxi][auxj] = WALL;
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

	//Gera a saída do mapa
	createExit();
}

//Gera as "vidas" no mapa
void genereteThingsOnMap(){

	//Laço de População na Matriz do Mapa
	for (int i = 0; i < currentPhase->size_x; i++) {
		for (int j = 0;  j < currentPhase->size_z; j++) {
			int x = (rand() % 100) + 1;
			//Caso a posição seja um chão e o valor do rand tenha sido <= a 5
			//Ele gera uma vida nesta posição
			if(x <= 5 && currentPhase->map[i][j] == PLAYABLE){
				currentPhase->map[i][j] = LIFE_SPHERE;
				j = j+3;
				i = i+1;
			}
			//Gera alguns obstaculos no mapa
			if(x > 5 && x < 15 && currentPhase->map[i][j] == PLAYABLE){
				//Evitando criar obstaculos em corredores Horizontais
				if(currentPhase->map[i+1][j] == WALL && currentPhase->map[i-1][j] == WALL)
					continue;

				//Evitando criar obstaculos em corredores Verticais
				if(currentPhase->map[i][j+1] == WALL && currentPhase->map[i][j-1] == WALL)
					continue;

				//Evita criar obstaculos em entrada de corredores (CIMA)
				if(currentPhase->map[i-1][j-1] == WALL && currentPhase->map[i+1][j-1] == WALL)
					continue;

				//Evita criar obstaculos em entrada de corredores (ESQUERDA)
				if(currentPhase->map[i-1][j-1] == WALL && currentPhase->map[i-1][j+1] == WALL)
					continue;
				
				//Evita criar obstaculos em entrada de corredores (DIREITA)
				if(currentPhase->map[i+1][j-1] == WALL && currentPhase->map[i+1][j+1] == WALL)
					continue;

				//Evita criar obstaculos em entrada de corredores (BAIXO)
				if(currentPhase->map[i-1][j+1] == WALL && currentPhase->map[i+1][j+1] == WALL)
					continue;

				currentPhase->map[i][j] = OBSTACLE;
				i++;
			}

		}
	}
}

// Função que cria uma saída no mapa
void createExit() {
	//Gera uma saída
	int haveExit = 1;
	while(haveExit != 0){
		int iX = rand() % 128;
		int iZ = rand() % 128;
		if(currentPhase->map[iX][iZ] != 0 && currentPhase->map[iX][iZ] != WALL){
			currentPhase->map[iX][iZ] = EXIT;
			printf("[DBG] Saida em:  %d : %d \n", iX, iZ);
			haveExit = 0;
		}
	}
}

//Função que monta os objetos do mapa
void buildMap() {
	int i, j;

	for (i = 0; i < currentPhase->size_x; i++) {
		for (j = 0;  j < currentPhase->size_z; j++) {

			switch (currentPhase->map[i][j]) {
				case WALL:
					//Paredes
					glPushMatrix();
						glTranslatef(i + 0.5, 4, j + 0.5);
						glScalef(1, 8, 1);
						glColor3ub(120, 120, 120);
						glutSolidCube(1);
					glPopMatrix();
					break;

				case FLOOR:
					//Chão
					glColor3ub(70, 70, 70);
					glBegin(GL_QUADS);
						glVertex3f(i, 0, j);
						glVertex3f(i, 0, j + 1);
						glVertex3f(i + 1, 0, j + 1);
						glVertex3f(i + 1, 0, j);
					glEnd();
					break;

				case OBSTACLE:
					//Obstaculo - Agua ?
					glPushMatrix();
						glTranslatef(i + 0.5, -0.2, j + 0.5);
						glScalef(1, 4, 1);
						glColor3ub(112, 207, 255);	
						glutSolidCube(1);
					glPopMatrix();
					break;

				case TRAP:
					//Armadilha colocado pelo Jogador
					glPushMatrix();
						glTranslatef(i + 0.5, 0.2, j + 0.5);
						glScalef(1, 0.2, 0.1);
						glRotatef(45.0, 0, 1, 0);
						glColor3ub(0,200,112);
						glutSolidCube(1);
					glPopMatrix();
					break;	

				case LIFE_SPHERE:
					//Life
					glPushMatrix();
						glTranslatef(i + 0.5, 2, j + 0.5);
						glScalef(0.015, 0.015, 0.015);
						glColor3ub(245,86,112);
						glutSolidSphere(20,10,10);
					glPopMatrix();
					break;

				case EXIT:
					//Saída
					glPushMatrix();
						glTranslatef(i + 0.5, -0.2, j + 0.5);
						glScalef(1, 0.1, 1);
						glColor3ub(245, 51, 196);	
						glutSolidCube(1);
					glPopMatrix();
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

	// Monta o chão para o usuário
	buildFloor();

	// Monta os objetos da cena
	buildMap();
}

//Função que verifica o conteudo do mapa no ponto (X,Z)
short verifyMapContent(short x, short z) {
	return currentPhase->map[x][z];
}

// Função que cria o chão
void buildFloor() {
	int x, z;
	room *aux = NULL;

	// Criando o chão das salas
	for (aux = currentPhase->room_list; aux != NULL; aux = aux->next) {
		x = aux->coord_x;
		z = aux->coord_z;

		glColor3ub(70, 70, 70);
		glBegin(GL_QUADS);
			glVertex3f(x - SIZE_ROOM, 0, z - SIZE_ROOM);
			glVertex3f(x - SIZE_ROOM, 0, z + SIZE_ROOM + 1);
			glVertex3f(x + SIZE_ROOM + 1, 0, z + SIZE_ROOM + 1);
			glVertex3f(x + SIZE_ROOM + 1, 0, z - SIZE_ROOM);
		glEnd();
	}
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
				case WALL:
					printf("W");
					break;
				case PLAYABLE:
					printf("F");
					break;
				default:
					printf("%d", currentPhase->map[i][j]);
			}
		}
		printf("\n");
	}
}

#endif