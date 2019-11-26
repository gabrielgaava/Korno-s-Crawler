// Biblioteca para as funções relacionadas aos monstros

//Verifica se a biblioteca já foi importada
#ifndef _MONSTER_H_
#define _MONSTER_H_

#define MONSTER_DAMAGE 1
#define MONSTER_COOLDOWN 10000000

#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>

/* Definição das structs */
typedef struct monster {
    //Utilizada para definir a posição dos monstros
    float charx;
    float chary;
    float charz;

    //Utilizada para definir a direção onde o personagem está de frente
    float direcaox;
    float direcaoy;
    float direcaoz;

    //Variável lógica para definir se os monstros estão vivos
    int life;

    //Variável que conta o cooldown do hit
    short atackCooldown;

    //Utilizada para criar uma lista de monstros
    struct monster *next;
} monster;

/* Definição de Variáveis Globais */
monster *listMonsters = NULL;

/* Definição dos Protótipos das Funções */
void createMonsters();
void buildMonsters();
void moveMonsters();
bool verifyMonsterPosition(float x, float z);

//Função que cria novos monstros
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
        newMonster->atackCooldown = 0;
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

            //Translação para a posição do monstro
            glTranslatef(aux->charx + 0.5, aux->chary, aux->charz + 0.5);

            //Rotação de acordo com o lado positivo
            if (aux->direcaox != 0) {
                if (aux->direcaox == 1) {
                    //glRotatef(90, 1, 0, 0);
                } else {
                    glRotatef(180, 0, 1, 0);
                }
            } else if (aux->direcaoz != 0) {
                if (aux->direcaoz == 1) {
                    glRotatef(270, 0, 1, 0);
                } else {
                    glRotatef(90, 0, 1, 0);
                }
            }

            //Cabeça do personagem
            glPushMatrix();
                glColor3ub(255, 255, 0);
                glTranslatef(0, bodyY + (headY / 2) + 0.075, 0);
                glScalef(headX, headY, headZ);
                glutSolidSphere(1, 50, 50);
            glPopMatrix();

            //Corpo do personagem
            glPushMatrix();
                glColor3ub(255, 0, 255);
                glTranslatef(0, (bodyY / 2), 0);
                glScalef(bodyX, bodyY, bodyZ);
                glutSolidCube(1);
            glPopMatrix();

            //Braço esquerdo do personagem
            glColor3ub(255, 255, 0);

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
            glColor3ub(255, 255, 0);

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

//Função que realiza a movimentação dos monstros
void moveMonsters() {
    monster *aux = NULL;
    float distance, distX, distZ;

    for (aux = listMonsters; aux != NULL; aux = aux->next) {
        //Se o monstro não está morto
         
        if (aux->life > 0) {
            //Calcula a distancia do monstro para o player
            distX = mainChar->charx - aux->charx;
            distZ = mainChar->charz - aux->charz;
            distance = sqrt(pow(distX, 2) + pow(distZ, 2));

            //Se estiver no campo de visão percorre o caminho
            if (distance < 25){
                if (aux->atackCooldown == 0) {
                    //Verifica se está no range
                    if ((fabs(distX) <= 1 && fabs(distZ) == 0) || (fabs(distX) == 0 && fabs(distZ <= 1))) {
                        //Aqui dá o dano
                        //damageChar(MONSTER_DAMAGE);

                        //Se atacou não faz nenhuma ação
                        continue;
                    }
                } else {
                    aux->atackCooldown--;
                }

                //Se entrar em algum IF, vai para a próxima iteração do for
                if (distX > 0 && verifyMapContent(aux->charx + 1, aux->charz) >= 0) {
                    if (!verifyMonsterPosition(aux->charx + 1, aux->charz)) {
                        if (aux->direcaox != 1) {
                            aux->direcaox = 1;
                            aux->direcaoz = 0;
                        } else {
                            aux->charx = aux->charx + 1;
                        }
                        continue;    
                    }
                }

                if (distX < 0 && verifyMapContent(aux->charx - 1, aux->charz) >= 0) {
                    if (!verifyMonsterPosition(aux->charx - 1, aux->charz)) {
                        if (aux->direcaox != -1) {
                            aux->direcaox = -1;
                            aux->direcaoz = 0;
                        } else {
                            aux->charx = aux->charx - 1;
                        }
                        continue;
                    }
                }
                
                if (distZ > 0 && verifyMapContent(aux->charx, aux->charz + 1) >= 0) {
                    if (!verifyMonsterPosition(aux->charx, aux->charz + 1)) {
                        if (aux->direcaoz != 1) {
                            aux->direcaox = 0;
                            aux->direcaoz = 1;
                        } else {
                            aux->charz = aux->charz + 1;
                        }
                        continue;
                    }
                }

                if (distZ < 0 && verifyMapContent(aux->charx, aux->charz - 1) >= 0) {
                    if (!verifyMonsterPosition(aux->charx, aux->charz - 1)) {
                        if (aux->direcaoz != -1) {
                            aux->direcaox = 0;
                            aux->direcaoz = -1;
                        } else {
                            aux->charz = aux->charz - 1;
                        }
                        continue;
                    }
                }
            }
        }
    }
}

// Função que verifica se possui algum monstro naquela posição
bool verifyMonsterPosition(float x, float z) {
    monster *aux = NULL;

    // Busca se naquele ponto existe algum zumbi
    for (aux = listMonsters; aux != NULL; aux = aux->next) {
        if (aux->charx == x && aux->charz == z) {
            return true;
        }
    }

    // Se não encontrou retorna false
    return false;
}

#endif