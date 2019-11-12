#include <stdlib.h>
#include <GL/glut.h>

// Struct da sala
typedef struct room {
	int coord_x;
	int coord_z;
	struct room *next;
} room;

// Struct da fase
typedef struct phase {
    int size_x, size_z;
	// Valores possíveis: 1 => chão, 2 => parede, 0 => nada
    int map[128][128];
	int finished;
    room *room_list;
} phase;

//Definição das primitivas
void createNewPhase();
void createRoom(int size_room);
void conectRoom();
void createWall();
void createMap();
void buildMap();
void buildPhase();
void printMap();

//Definição das variáveis globais
phase *currentPhase = NULL;

// Função que inicializa a struct fase
void createNewPhase() {
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

	//Seta para 0 a finished
	currentPhase->finished = 0;
}

// Função para criar uma sala, adicionando um lista de sala e set no mapa como jogável
void createRoom(int size_room) {
	int i, j, x, z;
	room *new_room = NULL, *aux = NULL;

	do {
		x = rand() % currentPhase->size_x;
	} while (x <= size_room || x >= currentPhase->size_x - size_room - 1);

	do {
		z = rand() % currentPhase->size_z;
	} while (z <= size_room || z >= currentPhase->size_z - size_room - 1);

	i = (x-size_room < 0) ? (0) : (x-size_room);
	while (i <= x+size_room && i < currentPhase->size_x) {
		j = (z-2 < 0) ? (0) : (z-size_room);
		while (j <= z+size_room && j < currentPhase->size_z) {
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
void conectRoom () {
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
	int number_room = 8;
	int size_room = 10;
	int i, j;

	//Cria number_room salas aleatoriamente
	for (i = 0; i < number_room; i++) {
		createRoom(size_room);
	}
	
	//Conecta as salas criadas
	conectRoom();

	//Cria as paredes em volta dos corredores e salas
	createWall();
}

void buildMap() {
	int i, j;

	for (i = 0; i < currentPhase->size_x; i++) {
		for (j = 0;  j < currentPhase->size_z; j++) {
			
			glPushMatrix();

			switch (currentPhase->map[i][j]) {
				case 1:
					//Chão
					glTranslatef(i, -0.2, j);
					glScalef(1, 0.1, 1);
					glColor3ub(255, 255, 255);
					glutSolidCube(1);
					break;
				case 2:
					//Paredes
					glTranslatef(i, -0.2, j);
					glScalef(1, 10, 1);
					glColor3ub(0, 255, 0);
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
		//Cria uma nova fase
		createNewPhase();

		//Monta a matriz com sala, corredores e paredes
		createMap();
	}

	// Monta os objetos da cena
	buildMap();
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