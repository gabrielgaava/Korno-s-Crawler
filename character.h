#include <stdlib.h>
#include <GL/glut.h>

/* Definição das structs */
typedef struct character {
    //Utilizada para definir a posição do personagem
    int charx;
    int chary;
    int charz;

    //Utilizada para definir a direção onde o personagem está de frente
    //1: frente para o positivo, 0: sem influencia, -1: frente para o negativo
    int direcaox;
    int direcaoy;
    int direcaoz;
} character;

typedef struct monster {
    //Utilizada para definir a posição do personagem
    int charx;
    int chary;
    int charz;

    //Utilizada para definir a direção onde o personagem está de frente
    int direcaox;
    int direcaoy;
    int direcaoz;

    //Variável lógica para definir se os monstros estão vivos
    int life;

    //Utilizada para criar uma lista de monstros
    struct monster *next;
} monster;

/* Definição de Variáveis Globais */
//Variáveis que definem a posição do personagem
character *mainChar = NULL;
monster *listMonsters = NULL;

/* Funções do personagem */
void createMainChar();
void buildMainChar();
void createNewChar();
void createMonsters();
void buildMonsters();

//Função que cria o personagem principal
void createMainChar(){
    //Inicializando o personagem principal
    mainChar = new character();

    //Coloca o personagem em uma sala aleatória
    for (room *aux = currentPhase->room_list; aux->next != NULL; aux = aux->next) {
        //Aleatoriamente escolhe uma sala para o personagem começar
        if(rand() % 2 == 1) {
            mainChar->charx = aux->coord_x;
            mainChar->charz = aux->coord_z;
        }
    }
    
    //Definindo posição em y do personagem principal
    mainChar->chary = 0;

    //Definindo a direção
    mainChar->direcaox = 1;
    mainChar->direcaoy = 0;
    mainChar->direcaoz = 0;
}

//Função para construir o personagem principal
void buildMainChar() {
    //Caso o personagem não tenha sido criado
    if (mainChar == NULL) {
        createMainChar();
    }

    //Variáveis que definem as escalas do modelo do personagem  
    float headX = 0.23, headY = 0.23, headZ = 0.23; 
    float bodyX = 0.3, bodyY = 1.8, bodyZ = 0.5;
    float armX = 0.1, armZ = 0.1;
    float leftArmY = 0.8;
    float rightArmY = 0.4;

    //Salva a matriz
    glPushMatrix();

    //Translação para a posição do personagem
    glTranslatef(mainChar->charx + 0.5, mainChar->chary, mainChar->charz + 0.5);

    //Rotação de acordo com o lado positivo
    if (mainChar->direcaox != 0) {
        if (mainChar->direcaox == 1) {
            //glRotatef(90, 1, 0, 0);
        } else {
            glRotatef(180, 0, 1, 0);
        }
    } else if (mainChar->direcaoz != 0) {
        if (mainChar->direcaoz == 1) {
            glRotatef(270, 0, 1, 0);
        } else {
            glRotatef(90, 0, 1, 0);
        }
    }

    //Corpo do personagem
    glPushMatrix();
    glColor3ub(0, 0, 255);
    glTranslatef(0, (bodyY / 2), 0);
    glScalef(bodyX, bodyY, bodyZ);
    glutSolidCube(1);
    glPopMatrix();

    //Cor para cabeça, e braços do personagem
    glColor3f(1.5, 0, 0);

    //Cabeça do personagem
    glPushMatrix();
    glTranslatef(0, bodyY + (headY / 2), 0);
    glScalef(headX, headY, headZ);
    glutSolidSphere(1, 50, 50);
    glPopMatrix();

    //Braço esquerdo do personagem
    glPushMatrix();
    glTranslatef(0, bodyY - (leftArmY / 2) - 0.1, - (bodyZ / 2) - (armZ / 2));
    glScalef(armX, leftArmY, armZ);
    glutSolidCube(1);
    glPopMatrix();

    //Braço direito do personagem
    glPushMatrix();
    glTranslatef(0, bodyY - (rightArmY / 2) - 0.1, (bodyZ / 2) + (armZ / 2));
    glScalef(armX, rightArmY, armZ);
    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(rightArmY / 2, bodyY - (3 * rightArmY / 2), (bodyZ / 2) + (armZ / 2));
    glScalef(rightArmY + 0.1, armX, armZ);
    glutSolidCube(1);
    glPopMatrix();

    //Volta a matriz ao estado anterior
    glPopMatrix();
}

void createMonsters() {
    int i, j, k;
    monster *newMonster = NULL, *aux = NULL;

    for (k = 0; k < currentPhase->numberRoom; k++) {
        do {
            i = rand() % currentPhase->size_x;
            j = rand() % currentPhase->size_z;
        } while (currentPhase->map[i][j] != 1);

        newMonster = new monster();
        newMonster->charx = i;
        newMonster->chary = 0;
        newMonster->charz = j;
        newMonster->direcaox = 1;
        newMonster->direcaoy = 0;
        newMonster->direcaoz = 0;
        newMonster->life = 1;
        newMonster->next = NULL;

        //Adiciona na lista de monstros
        if (listMonsters == NULL) {
            listMonsters = newMonster;
        } else {
            for (aux = listMonsters; aux->next != NULL; aux = aux->next);
            aux->next = newMonster;
        }
    }
}

void buildMonsters() {
    //Variáveis que definem as escalas do modelo do personagem  
    float headX = 0.23, headY = 0.23, headZ = 0.23; 
    float bodyX = 0.3, bodyY = 1.8, bodyZ = 0.5;
    float armX = 0.1, armZ = 0.1;
    float leftArmY = 0.4, rightArmY = 0.4;

    monster *aux = NULL;
    
    if (listMonsters == NULL) {
        createMonsters();
    }

    for (aux = listMonsters; aux != NULL; aux = aux->next) {
        if (aux->life == 1){
            //Salva a matriz
            glPushMatrix();

            //Translação para a posição correta
            glTranslatef(aux->charx + 0.5, aux->chary, aux->charz + 0.5);

            //Corpo do personagem
            glPushMatrix();
            glColor3ub(255, 0, 255);
            glTranslatef(0, (bodyY / 2), 0);
            glScalef(bodyX, bodyY, bodyZ);
            glutSolidCube(1);
            glPopMatrix();

            //Cor para cabeça, e braços do personagem
            glColor3ub(255, 255, 0);

            //Cabeça do personagem
            glPushMatrix();
            glTranslatef(0, bodyY + (headY / 2), 0);
            glScalef(headX, headY, headZ);
            glutSolidSphere(1, 50, 50);
            glPopMatrix();

            //Braço esquerdo do personagem
            glPushMatrix();
            glTranslatef(0, bodyY - (leftArmY / 2) - 0.1, - (bodyZ / 2) - (armZ / 2));
            glScalef(armX, leftArmY, armZ);
            glutSolidCube(1);
            glPopMatrix();

            glPushMatrix();
            glTranslatef(leftArmY / 2, bodyY - (3 * leftArmY / 2), - (bodyZ / 2) - (armZ / 2));
            glScalef(leftArmY + 0.1, armX, armZ);
            glutSolidCube(1);
            glPopMatrix();

            //Braço direito do personagem
            glPushMatrix();
            glTranslatef(0, bodyY - (rightArmY / 2) - 0.1, (bodyZ / 2) + (armZ / 2));
            glScalef(armX, rightArmY, armZ);
            glutSolidCube(1);
            glPopMatrix();

            glPushMatrix();
            glTranslatef(rightArmY / 2, bodyY - (3 * rightArmY / 2), (bodyZ / 2) + (armZ / 2));
            glScalef(rightArmY + 0.1, armX, armZ);
            glutSolidCube(1);
            glPopMatrix();

            glPopMatrix();
        }
    }
    
    
}