// Biblioteca relacionada as funções do personagem principal

#include <stdlib.h>
#include <GL/glut.h>

/* Definição das structs */
typedef struct character {
    //Utilizada para definir a posição do personagem
    float charx;
    float chary;
    float charz;

    //Utilizada para definir a direção onde o personagem está de frente
    //1: frente para o positivo, 0: sem influencia, -1: frente para o negativo
    float direcaox;
    float direcaoy;
    float direcaoz;

    //Vida do jogador
    float pLife = 100;
    float lifePerc = 1;
    bool isDead = false;

    //Munição
    int ammo = 10;
} character;

/* Definição de Variáveis Globais */
character *mainChar = NULL;

/* Definição dos Protótipos das Funções */
void createMainChar();
void buildMainChar();
void getLife(int x, int z);
void shoot(int x, int z, int in);

void shoot(int x, int z, int in){
    
    //criamos a bala em x,z que é onde o personagem esta
    //criarBala(x,z);

    //Depois disso, utilizamos a varivel IN pra saber se o tiro
    //É no eixo X ou no eixo Z, dependendo de onde o personagem estiver olhando

    //Só não sei ainda como fazer a bala "percorrer" o eixo IN
}

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