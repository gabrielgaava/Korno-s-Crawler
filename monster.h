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

    //Utilizada para criar uma lista de monstros
    struct monster *next;
} monster;

/* Definição de Variáveis Globais */
monster *listMonsters = NULL;

/* Definição dos Protótipos das Funções */
void createMonsters();
void buildMonsters();
void moveMonsters();

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

//Função que realiza a movimentação dos monstros
void moveMonsters() {
    monster *aux = NULL;
    float distance, distX, distZ;
    int step;

    for (aux = listMonsters; aux != NULL; aux = aux->next) {
        //Se o monstro não está morto
         
        if (aux->life > 0) {
            //Calcula a distancia do monstro para o player
            distX = mainChar->charx - aux->charx;
            distZ = mainChar->charz - aux->charz;
            distance = sqrt(pow(distX, 2) + pow(distZ, 2));

            //Se estiver no campo de visão percorre o caminho
            if (distance < 25){
                if (distance == 1) {
                    continue;
                }

                //ATENÇÃO: Essa parte do código abaixo foi realizada as pressas
                //Foi mal :/
                //if (tipoCam > 0) {
                    //Caso o tipo da câmera for 2D

                    //Se entrar em algum IF, vai para a próxima iteração do for
                    if (distX > 0 && verifyMapContent(aux->charx+1, aux->charz) != 2) {
                        aux->charx = aux->charx + 1;
                        aux->direcaox = 0;
                        aux->direcaoz = 1;
                        continue;
                    }

                    if (distX < 0 && verifyMapContent(aux->charx-1, aux->charz) != 2) {
                        aux->charx = aux->charx - 1;
                        aux->direcaox = 0;
                        aux->direcaoz = -1;
                        continue;
                    }

                    if (distZ > 0 && verifyMapContent(aux->charx, aux->charz+1) != 2) {
                        aux->charz = aux->charz + 1;
                        aux->direcaox = 0;
                        aux->direcaoz = 1;
                        continue;
                    }

                    if (distZ > 0 && verifyMapContent(aux->charx, aux->charz-1) != 2) {
                        aux->charz = aux->charz - 1;
                        aux->direcaox = 0;
                        aux->direcaoz = -1;
                        continue;
                    }
                //}
            }
        }
    }
}