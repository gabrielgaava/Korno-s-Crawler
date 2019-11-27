// Biblioteca para as funções relacionadas aos monstros

//Verifica se a biblioteca já foi importada
#ifndef _MONSTER_H_
#define _MONSTER_H_

#define MONSTER_DAMAGE 10
#define MONSTER_ATACK_COOLDOWN 1.5
#define MONSTER_WALK_COOLDOWN 0.8
#define MONSTER_NUMBER 0

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

    //Variável que controla a vida dos zumbi
    int life;

    // Variável que verifica se o monstro tomou dano
    bool tookDamage;

    //Variável que conta o cooldown das ações do zumbi
    clock_t atackCooldown, walkCooldown;

    //Utilizada para criar uma lista de monstros
    struct monster *next;
} monster;

/* Definição de Variáveis Globais */
monster *listMonsters = NULL;

/* Definição dos Protótipos das Funções */
void createMonsters();
void buildMonsters();
void moveMonsters(ISoundEngine* engine);
monster * verifyMonsterPosition(float x, float z);
void clearMonsters();

//Função que cria novos monstros
void createMonsters() {
    int i, j, k;
    monster *newMonster = NULL, *aux = NULL;

    for (k = 0; k < MONSTER_NUMBER + idPhase * 10; k++) {
        do {
            i = rand() % currentPhase->size_x;
            j = rand() % currentPhase->size_z;
        } while (currentPhase->map[i][j] <= 0);

        newMonster = new monster();

        newMonster->charx = i;
        newMonster->chary = 0;
        newMonster->charz = j;

        newMonster->direcaox = 1;
        newMonster->direcaoy = 0;
        newMonster->direcaoz = 0;

        newMonster->life = 1;
        newMonster->tookDamage = false;

        newMonster->atackCooldown = 0;
        newMonster->walkCooldown = 0;

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

    for (aux = listMonsters; aux != NULL; aux = aux->next) {
        if (aux->life == 1){
            //Salva a matriz
            glPushMatrix();

            //Translação para a posição do monstro
            glTranslatef(aux->charx + 0.5, aux->chary, aux->charz + 0.5);

            //Rotação de acordo com o lado positivo
            if (aux->direcaox != 0) {
                if (aux->direcaox == -1) {
                    glRotatef(180, 0, 1, 0);
                }
            } else {
                if (aux->direcaoz == 1) {
                    glRotatef(270, 0, 1, 0);
                } else {
                    glRotatef(90, 0, 1, 0);
                }
            }

            //Cabeça do personagem
            glPushMatrix();
                glColor3ub(123, 164, 49);
                glTranslatef(0, bodyY + (headY / 2) + 0.075, 0);
                glScalef(headX, headY, headZ);
                glutSolidSphere(1, 50, 50);
            glPopMatrix();

            //Corpo do personagem
            glPushMatrix();
                glColor3ub(18, 10, 143);
                glTranslatef(0, (bodyY / 2), 0);
                glScalef(bodyX, bodyY, bodyZ);
                glutSolidCube(1);
            glPopMatrix();

            //Braço esquerdo do personagem
            glColor3ub(123, 164, 49);

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
            glColor3ub(123, 164, 49);

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
void moveMonsters(ISoundEngine* engine) {
    monster *aux, *previous;
    int distX, distZ;
    float distance;
    double tempoDecorrido;

    if (listMonsters == NULL) {
        createMonsters();
    }

    previous = NULL;
    aux = listMonsters;
    while (aux != NULL) {
        // Se o monstro tomou dano
        if(verifyMapContent(aux->charx,aux->charz) == TRAP){
            aux->tookDamage = true;
        }

        if (aux->tookDamage) {
            aux->life = 0;
            ISound* music = engine->play2D("assets/zombie.mp3", false);
        }
         
        if (aux->life > 0) {

            //Calcula a distancia do monstro para o player
            distX = mainChar->charx - aux->charx;
            distZ = mainChar->charz - aux->charz;
            distance = sqrt(pow(distX, 2) + pow(distZ, 2));

            //Se estiver no campo de visão percorre o caminho
            if (distance < 30){
                
                // Verifica se o monstro está no range
                if ((abs(distX) == 1 && abs(distZ) == 0) || (abs(distX) == 0 && abs(distZ) == 1)) {
                    
                    // Calcula o cooldown do hit
                    tempoDecorrido = ((double) clock() - aux->atackCooldown) / CLOCKS_PER_SEC;

                    // Se passou do tempo de cooldown, o zumbi pode atacar
                    if (tempoDecorrido >= MONSTER_ATACK_COOLDOWN) {
                        //Aqui dá o dano
                        damageChar(MONSTER_DAMAGE, engine);

                        // Registra a hora que atacou
                        aux->atackCooldown = clock();
                    }

                    // Está a 1 de distancia em X ou Z
                    // Nesse caso, se atacar não faz mais nada
                    // Mesmo se não atacar, não faz nada
                    previous = aux;
                    aux = aux->next;
                    continue;
                }

                // Calcula o cooldown para andar
                tempoDecorrido = ((double) clock() - aux->walkCooldown) / CLOCKS_PER_SEC;
                if (tempoDecorrido >= MONSTER_WALK_COOLDOWN) {
                    aux->walkCooldown = clock();
                
                    //Se entrar em algum IF, vai para a próxima iteração do for
                    if (distX > 0 && verifyMapContent(aux->charx + 1, aux->charz) >= 0) {
                        if (verifyMonsterPosition(aux->charx + 1, aux->charz) == NULL) {
                            if (aux->direcaox != 1) {
                                aux->direcaox = 1;
                                aux->direcaoz = 0;
                            } else {
                                aux->charx = aux->charx + 1;
                            }
                            previous = aux;
                            aux = aux->next;
                            continue;    
                        }
                    }

                    if (distX < 0 && verifyMapContent(aux->charx - 1, aux->charz) >= 0) {
                        if (verifyMonsterPosition(aux->charx - 1, aux->charz) == NULL) {
                            if (aux->direcaox != -1) {
                                aux->direcaox = -1;
                                aux->direcaoz = 0;
                            } else {
                                aux->charx = aux->charx - 1;
                            }
                            previous = aux;
                            aux = aux->next;
                            continue;
                        }
                    }
                    
                    if (distZ > 0 && verifyMapContent(aux->charx, aux->charz + 1) >= 0) {
                        if (verifyMonsterPosition(aux->charx, aux->charz + 1) == NULL) {
                            if (aux->direcaoz != 1) {
                                aux->direcaox = 0;
                                aux->direcaoz = 1;
                            } else {
                                aux->charz = aux->charz + 1;
                            }
                            previous = aux;
                            aux = aux->next;
                            continue;
                        }
                    }

                    if (distZ < 0 && verifyMapContent(aux->charx, aux->charz - 1) >= 0) {
                        if (verifyMonsterPosition(aux->charx, aux->charz - 1) == NULL) {
                            if (aux->direcaoz != -1) {
                                aux->direcaox = 0;
                                aux->direcaoz = -1;
                            } else {
                                aux->charz = aux->charz - 1;
                            }
                            previous = aux;
                            aux = aux->next;
                            continue;
                        }
                    }
                }
            }

            // Zumbi vivo avança na lista
            previous = aux;
            aux = aux->next;

        } else {
            // Se o zumbi estiver morto, remover da lista
            if (previous == NULL) {
                // Remove do começo da lista
                previous = aux->next;
                free(aux);
                aux = previous;
                previous = NULL;

                // Atualiza o ponteiro inicial da lista
                listMonsters = aux;

            } else {
                // Remove meio/final
                previous->next = aux->next;
                free(aux);
                aux = previous->next;
            }
        }
    }
}

// Função que verifica se possui algum monstro naquela posição
monster * verifyMonsterPosition(float x, float z) {
    monster *aux = NULL;

    // Busca se naquele ponto existe algum zumbi
    for (aux = listMonsters; aux != NULL; aux = aux->next) {
        if (aux->charx == x && aux->charz == z) {
            return aux;
        }
    }

    // Se não encontrou retorna false
    return NULL;
}

void clearMonsters(){
    monster *aux = NULL, *previous = NULL;

    if (listMonsters != NULL) {
        previous = listMonsters;
        aux = previous->next;
        do {
            delete(previous);
            previous = aux;
            aux = previous->next;
        } while (aux->next != NULL);
        delete(aux);

        listMonsters = NULL;
    }
}

#endif