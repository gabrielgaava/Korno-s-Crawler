#include <stdlib.h>
#include <GL/glut.h>

/* Definição das structs */
typedef struct character {
    //Utilizada para definir a posição do personagem
    float charx;
    float chary;
    float charz;

    //Utilizada para definir a direção onde o personagem está de frente
    float direcaox;
    float direcaoy;
    float direcaoz;

    //Utilizada para criar uma lista de personagem
    struct character *next;
} character;

/* Definição de Variáveis Globais */
//Variáveis que definem a posição do personagem
character *mainChar = NULL;
character *monsters = NULL;

/* Funções do personagem */
void createMainChar();
void buildMainChar();
void createNewChar();

//Função que cria o personagem principal
void createMainChar(){
    //Inicializando o personagem principal
    mainChar = new character();
    
    //Definindo posição do personagem principal
    mainChar->charx = 0;
    mainChar->chary = 4.4;
    mainChar->charz = 0;

    //Definindo a direção
    mainChar->direcaox = 1;
    mainChar->direcaoy = 1;
    mainChar->direcaoz = 1;
}

//Função para construir o personagem principal
void buildMainChar() {
    //Caso o personagem não tenha sido criado
    if (mainChar == NULL) {
        createMainChar();
    }

    //Variáveis que definem o modelo do personagem  
    int armsize = 4.7, bodysize = 6, headsize = 1;
    float armposx = 1.7, armposy = 0.5, armposz = 0;
    float headposx = 0,headposy = 10,headposz = 0;
    float bodyposx = 0,bodyposy = 5,bodyposz = 0;

    //Salva a matriz
    glPushMatrix();

    glTranslatef(mainChar->charx, mainChar->chary, mainChar->charz);
   
    //Corpo do personagem
    glPushMatrix();
    glColor3f(1.5, 0, 0);
    glScalef(1, bodysize, 1);
    glutWireCube(1.5);
    glTranslatef(bodyposx, bodyposy, bodyposz);
    glPopMatrix();
    // glRotatef(90,0,1,0);

    //Cabeça do personagem
    glPushMatrix();
    glTranslatef(0.0, 5, 0);
    glutSolidSphere(1, 50, 50);
    glPopMatrix();

    //Braço dos personagem
    glPushMatrix();
    glScalef(0.6,armsize,0.6);
    glTranslatef(armposx,armposy,armposz);
    glRotatef(0, 1, 0, 0);
    glutWireCube(1);
    glPopMatrix();

    //Braço dos personagem
    glPushMatrix();
    glScalef(0.6, armsize, 0.6);
    glTranslatef(-armposx, armposy, armposz);
    glRotatef(0, 1, 0, 0);
    glutWireCube(1);
    glPopMatrix();

    //Volta a matriz ao estado anterior
    glPopMatrix();
}