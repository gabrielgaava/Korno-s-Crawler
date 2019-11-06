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
void create_new_phase();
void create_room(int size_room);
void conect_room();
void create_wall();
void create_map();
void build_map();
void build_phase();

void print_map();

//Definição das variáveis globais
phase *current_phase = NULL;

// Função que inicializa a struct fase
void create_new_phase() {
	int i, j;
	
	current_phase = new phase();

	//Define o tamanho do mapa
	current_phase->size_x = current_phase->size_z = 128;

    //Seta o mapa para 0
	for (i=0; i < current_phase->size_x; i++) {
		for (j=0; j < current_phase->size_z; j++) {
			current_phase->map[i][j] = 0;
		}
	}

    //Seta a lista com NULL
    current_phase->room_list = NULL;

	//Seta para 0 a finished
	current_phase->finished = 0;
}

// Função para criar uma sala, adicionando um lista de sala e set no mapa como jogável
void create_room(int size_room) {
	int i, j, x, z;
	room *new_room = NULL, *aux = NULL;

	do {
		x = rand() % current_phase->size_x;
	} while (x <= size_room || x >= current_phase->size_x - size_room - 1);

	do {
		z = rand() % current_phase->size_z;
	} while (z <= size_room || z >= current_phase->size_z - size_room - 1);

	i = (x-size_room < 0) ? (0) : (x-size_room);
	while (i <= x+size_room && i < current_phase->size_x) {
		j = (z-2 < 0) ? (0) : (z-size_room);
		while (j <= z+size_room && j < current_phase->size_z) {
			current_phase->map[i][j] = 1;
			j++;
		}
		i++;
	}

	new_room = new room();
	new_room->coord_x = x;
	new_room->coord_z = z;
	new_room->next = NULL;
	
	if (current_phase->room_list == NULL){
		current_phase->room_list = new_room;
	} else {
        aux = current_phase->room_list;
    	while (aux->next != NULL) {
			aux = aux->next;
		}
		aux->next = new_room;
	}
}

// Função para criar corredor entre as salas
void conect_room () {
	room *aux = NULL, *aux2 = NULL;
	int i, j;

	aux = current_phase->room_list;
	for (aux2 = aux->next; aux2 != NULL; aux2 = aux2->next) {
		j = aux->coord_z;
		
		//Criando corredor em X
		if (aux->coord_x < aux2->coord_x) {
			for (i = aux->coord_x; i <= aux2->coord_x; i++) {
				current_phase->map[i][j] = 1;
			}
		} else {
			for (i=aux->coord_x; i >= aux2->coord_x; i--) {
				current_phase->map[i][j] = 1;
			}
		}
		
		//Criando corredor em Y
		if (aux->coord_z < aux2->coord_z) {
			for (j = aux->coord_z; j <= aux2->coord_z; j++) {
				current_phase->map[i][j] = 1;
			}
		} else {
			for (j = aux->coord_z; j >= aux2->coord_z; j--) {
				current_phase->map[i][j] = 1;
			}	
		}
	}
}

//Função que seta na matriz a localização das paredes
void create_wall() {
	/*int i, j;

	for (i = 0; i < current_phase->size_x; i++) {
		for (j = 0; j < current_phase->size_z - 1; j++) {
			if (current_phase->map[i][j] == 0) {
				if (current_phase->map[i+1][j] == 1 || current_phase->map[i][j+1] || current_phase->map[i+1][j+1]) {
					current_phase->map[i][j] = 2;
				}
			}

			if (current_phase->map[i][j] == 1 && current_phase->map[i][j + 1] == 0) {
				current_phase->map[i][j + 1] = 2;
			}
		}
	}*/
}

//Função que monta a matriz do mapa
void create_map() {
	int number_room = 8;
	int size_room = 10;
	int i, j;

	//Cria number_room salas aleatoriamente
	for (i = 0; i < number_room; i++) {
		create_room(size_room);
	}
	
	//Conecta as salas criadas
	conect_room();

	//Cria as paredes em volta dos corredores e salas
	create_wall();
}

void build_map() {
	int i, j;

	for (i = 0; i < current_phase->size_x; i++) {
		for (j = 0;  j < current_phase->size_z; j++) {
			
			glPushMatrix();

			switch (current_phase->map[i][j]) {
				case 1:
					glTranslatef(i, -0.2, j);
					glScalef(1, 0.1, 1);
					glColor3ub(255, 255, 255);
					glutSolidCube(1);
					break;
				case 2:
					glTranslatef(i, -0.2, j);
					glScalef(1, 5, 1);
					glColor3ub(255, 255, 255);
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
void build_phase() {

	if (current_phase == NULL || current_phase->finished == 1) {
		//Cria uma nova fase
		create_new_phase();

		//Monta a matriz com sala, corredores e paredes
		create_map();
	}

	// Monta os objetos da cena
	build_map();
}

/* Funções de DEBUG */

// Função para imprimir o mapa
void print_map(){
	int i, j;
	for (i=0; i<current_phase->size_x; i++) {
		for (j=0; j<current_phase->size_z; j++) {
			switch (current_phase->map[i][j]) {
				case 0:
					printf("-");
					break;
				default:
					printf("%d", current_phase->map[i][j]);
			}
		}
		printf("\n");
	}
}