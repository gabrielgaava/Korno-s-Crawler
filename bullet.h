// Biblioteca relacionada as funções da arma

//Verifica se a biblioteca já foi importada
#ifndef _BULLET_H_
#define _BULLET_H_

// Bibliotecas utilizadas neste arquivo
#include <GL/glut.h>
#include "character.h"

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

#endif