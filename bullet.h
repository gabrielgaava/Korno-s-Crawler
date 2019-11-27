// Biblioteca relacionada as funções da arma

//Verifica se a biblioteca já foi importada
#ifndef _BULLET_H_
#define _BULLET_H_

// Bibliotecas utilizadas neste arquivo
#include <GL/glut.h>
#include "character.h"
#include "monster.h"

typedef struct bullet {
    float coordX, coordY, coordZ;
    float direcaoX, direcaoY, direcaoZ;

    struct bullet *next;
} bullet;

/* Declaração de variáveis globais */
bullet * bulletList = NULL;

/* Protótipos das funções */
void createBullet();
void buildBullet(bullet *);
void moveBullets();

/* Funções */

// Função que cria uma bala
void createBullet(){
    bullet *aux = NULL, *newBullet = NULL;
    
    // Verifica se o jogador possui tiros
    if (mainChar->currentAmmo > 0) {
        newBullet = new bullet();

        newBullet->next = NULL;

        // Definindo a posição e direção da bala
        newBullet->coordX = mainChar->charx;
        newBullet->coordY = mainChar->chary;
        newBullet->coordZ = mainChar->charz;
        newBullet->direcaoX = mainChar->direcaox;
        newBullet->direcaoY = mainChar->direcaoy;
        newBullet->direcaoZ = mainChar->direcaoz;

        if (bulletList != NULL) {
            //Coloca no fim da lista de balas
            for (aux = bulletList; aux->next != NULL; aux = aux->next);
            aux->next = newBullet;
        } else {
            bulletList = newBullet;
        }

        // Diminui o numero de tiros
        mainChar->currentAmmo--;
    }
}

// Função que controi a munição
void buildBullet(bullet * b) {
    glPushMatrix();
        glColor3ub(0, 255, 0);
        glTranslatef(b->coordX + 0.5, 1.4, b->coordZ + 0.5);
        glScalef(0.1, 0.05, 0.1);
        glutSolidSphere(1, 20, 20);
    glPopMatrix();
}

// Função que verifica colisão nos tiros e chama a função que modela a bala
void moveBullets() {
    bullet *aux, *previous;
    monster *monsterHit = NULL;
    bool removeBullet = false;

    aux = bulletList;
    previous = NULL;
    while (aux != NULL) {
        // Faz a lógica de movimentação da bala
        if (aux->direcaoX != 0) {
            if (aux->direcaoX == 1) {
                aux->coordX++;
            } else {
                aux->coordX--;
            }
        } else {
            if (aux->direcaoZ == 1) {
                aux->coordZ++;
            } else {
                aux->coordZ--;
            }
        }

        // Verifica se não parou em um obstáculo
        if (verifyMapContent(aux->coordX, aux->coordZ) >= 0) {
                // Verifica a colisão
            monsterHit = verifyMonsterPosition(aux->coordX, aux->coordZ);
            if (monsterHit != NULL) {
                // Caso colidiu
                // Sinaliza na struct que o monstrou tomou dano
                removeBullet = true;
                monsterHit->tookDamage = true;
            } else {
                // Se não colidiu
                // Chamada da função que controi a bala
                removeBullet = false;
                buildBullet(aux);
            }
        } else {
            // Se parou, a bala "morre"
            removeBullet = true;
        }

        // Verifica se é para remover a bala
        if (removeBullet) {
            if (previous == NULL) {
                // Remove do começo da lista
                previous = aux->next;
                free(aux);
                aux = previous;
                previous = NULL;

                // Atualiza o ponteiro inicial da lista
                bulletList = aux;
            } else {
                // Remove meio/final
                previous->next = aux->next;
                free(aux);
                aux = previous->next;
            }
        } else {
            previous = aux;
            aux = aux->next;
        }
    }
}

#endif