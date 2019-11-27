// Biblioteca relacionada as funções do personagem principal

//Verifica se a biblioteca já foi importada
#ifndef _CHARACTER_H_
#define _CHARACTER_H_

// Bibliotecas utilizadas nesse arquivo
#include <stdlib.h>
#include <GL/glut.h>
#include "map.h"

/* Definição das structs */
typedef struct character {
    //Utilizada para definir a posição do personagem
    int charx;
    int chary;
    int charz;

    //Utilizada para definir a direção onde o personagem está de frente
    //1: frente para o positivo, 0: sem influencia, -1: frente para o negativo
    float direcaox;
    float direcaoy;
    float direcaoz;

    //Utilizada para armazenar a arma
    int currentAmmo;

    //Armadilhas usados no jogo
    int currentTraps;

    //Vida do jogador
    float pLife;
    float lifePerc;
    bool isDead;
} character;

/* Definição de Variáveis Globais */
character *mainChar = NULL;

/* Definição dos Protótipos das Funções */
void createMainChar();
void buildMainChar();
void buildGun(float, float, float);
void getLife(int x, int z);
void damageChar(float damage);
void updateCharRotDirection(float);
void clearMainChar();

//Função que cria o personagem principal
void createMainChar(){
    //Inicializando o personagem principal
    mainChar = new character();

    //Coloca o personagem na primeira sala criada
    mainChar->charx = currentPhase->room_list->coord_x;
    mainChar->charz = currentPhase->room_list->coord_z;

    //Definindo posição em y do personagem principal
    mainChar->chary = 0;

    //Definindo a direção
    mainChar->direcaox = 1;
    mainChar->direcaoy = 0;
    mainChar->direcaoz = 0;

    //Definição de outras variáveis
    mainChar->pLife = 100;
    mainChar->currentAmmo = 10;
    mainChar->currentTraps = 5;
    mainChar->lifePerc = 1;
    mainChar->isDead = false;
}

//Função para construir o personagem principal
void buildMainChar() {
    //Caso o personagem não tenha sido criado
    if (mainChar == NULL) {
        createMainChar();
    }

    //Variáveis que definem as escalas do modelo do personagem  
    float headX = 0.20, headY = 0.20, headZ = 0.20; 
    float bodyX = 0.3, bodyY = 1.8, bodyZ = 0.5;
    float armX = 0.1, armZ = 0.1;
    float leftArmY = 0.6;
    float rightArmY = 0.4;

    //Salva a matriz
    glPushMatrix();

    //Translação para a posição do personagem
    glTranslatef(mainChar->charx + 0.5, mainChar->chary, mainChar->charz + 0.5);
    
    //Rotação de acordo com o lado positivo
    if (mainChar->direcaox == -1) {
        glRotatef(180, 0, 1, 0);
    } else if (mainChar->direcaoz != 0) {
        if (mainChar->direcaoz == 1) {
            glRotatef(270, 0, 1, 0);
        } else {
            glRotatef(90, 0, 1, 0);
        }
    }        

    //Cabeça do personagem
    glPushMatrix();
        glColor3ub(255, 0, 0);
        glTranslatef(0, bodyY + (headY / 2) + 0.075, 0);
        glScalef(headX, headY, headZ);
        glutSolidSphere(1, 50, 50);
    glPopMatrix();

    //Corpo do personagem
    glPushMatrix();
        glColor3ub(0, 0, 255);
        glTranslatef(0, (bodyY / 2), 0);
        glScalef(bodyX, bodyY, bodyZ);
        glutSolidCube(1);
    glPopMatrix();

    //Braço esquerdo do personagem
    glPushMatrix();
        glColor3ub(255, 0, 0);
        glTranslatef(0, bodyY - (leftArmY / 2) - 0.1, - (bodyZ / 2) - (armZ / 2));
        glScalef(armX, leftArmY, armZ);
        glutSolidCube(1);
    glPopMatrix();

    //Braço direito do personagem
    glColor3ub(255, 0, 0);

    glPushMatrix();
        glTranslatef(0, bodyY - (rightArmY / 2) - 0.1, (bodyZ / 2) + (armZ / 2));
        glScalef(armX, rightArmY, armZ);
        glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
        glTranslatef((rightArmY / 2) - 0.05, bodyY - rightArmY - 0.1, (bodyZ / 2) + (armZ / 2));
        glRotatef(90, 0, 0, 1);
        glScalef(armX, rightArmY, armZ);
        glutSolidCube(1);
    glPopMatrix();

    // Modelagem da arma
    buildGun(rightArmY, bodyY - rightArmY - 0.1, (bodyZ / 2) + (armZ / 2));

    //Volta a matriz ao estado anterior
    glPopMatrix();
}

// Função que constroi a arma
void buildGun(float x, float y, float z) {
    glColor3ub(0, 255, 0);

    glPushMatrix();
        glTranslatef(x, y, z);
        glScalef(0.1, 0.20, 0.1);
        glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(x + 0.05, y + 0.1, z);
        glRotatef(90, 0, 0, 1);
        glScalef(0.10, 0.3, 0.1);
        glutSolidCube(1);
    glPopMatrix();
}

// Função que marca o dano no character
void damageChar(float damage) {
    mainChar->pLife = mainChar->pLife - damage;
}

// Atualiza as variaveis de direção de acordo com a rotacao
void updateCharRotDirection(float rot) {
    if (rot > 315 && rot <= 45) {
        mainChar->direcaox = 1;
        mainChar->direcaoz = 0;
    } else if (rot > 45 && rot <= 135) {
        mainChar->direcaox = 0;
        mainChar->direcaoz = 1;
    } else if (rot > 135 && rot <= 225) {
        mainChar->direcaox = -1;
        mainChar->direcaoz = 0;
    } else {
        mainChar->direcaox = 0;
        mainChar->direcaoz = -1;
    }
}

void clearMainChar() {
    delete(mainChar);
    mainChar = NULL;
}

#endif