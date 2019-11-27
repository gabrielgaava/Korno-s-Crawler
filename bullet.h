// Biblioteca relacionada as funções da arma

//Verifica se a biblioteca já foi importada
#ifndef _BULLET_H_
#define _BULLET_H_

// Bibliotecas utilizadas neste arquivo
#include <GL/glut.h>
#include "character.h"
#include "monster.h"

typedef struct bullet {
    float damage;
    int ttl;
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
    bullet *aux = NULL;
    bullet * newBullet = new bullet();

    newBullet->ttl = 3;
    newBullet->damage = 0;
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
}

// Função que controi a munição
void buildBullet(bullet * b) {
    glPushMatrix();
        glTranslatef(b->coordX + 0.5, 1.4, b->coordZ + 0.5);
        glScalef(0.1, 0.05, 0.1);
        glutSolidSphere(1, 20, 20);
    glPopMatrix();
}

// Função que verifica colisão nos tiros e chama a função que modela a bala
void moveBullets() {
    bullet *aux = NULL;
    monster *monsterHit = NULL;

    for (aux = bulletList; aux != NULL; aux = aux->next) {
        
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

        // Verifica a colisão
        monsterHit = verifyMonsterPosition(aux->coordX, aux->coordZ);
        if (monsterHit != NULL) {
            // Caso colidiu
            // Sinaliza na struct que o monstrou tomou dano
            monsterHit->tookDamage = true;
        } else {
            // Se não colidiu
            // Chamada da função que controi a bala
            buildBullet(aux);
        }
    }
}

#endif