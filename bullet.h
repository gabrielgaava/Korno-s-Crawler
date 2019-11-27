// Biblioteca relacionada as funções da arma

//Verifica se a biblioteca já foi importada
#ifndef _BULLET_H_
#define _BULLET_H_

// Bibliotecas utilizadas neste arquivo
#include <GL/glut.h>
#include "character.h"
#include "monster.h"

int rot1,tipoCam1;
typedef struct bullet {
    float coordX, coordY, coordZ;
    float direcaoX, direcaoY, direcaoZ;

    struct bullet *next;
} bullet;

/* Declaração de variáveis globais */
bullet * bulletList = NULL;

/* Protótipos das funções */
void createBullet(int tipoCam,int rot,ISoundEngine *engine);
void buildBullet(bullet *);
void moveBullets();
void getAmmo(ISoundEngine* engine);
void clearBullets();

/* Funções */

int verificaQuadrante(int rot){
    if(tipoCam1 > 0){
        if(rot > 315)
            return 1;
        else if(rot <= 45)
            return 1;
        else if(rot > 45 && rot <= 135)
            return 2;
        else if(rot > 135 && rot <= 225 )
            return 3;
        else if(rot >225 && rot <= 315)
            return 4;
    }
    else{
        if(mainChar->direcaox == 0 && mainChar->direcaoz == 1)
            return 1;
        if(mainChar->direcaox == 1 && mainChar->direcaoz == 0)
            return 2;
        if(mainChar->direcaox == 0 && mainChar->direcaoz == -1)
            return 3;
        if(mainChar->direcaox == -1 && mainChar->direcaoz == 0)
            return 4;
        
    }
        
}

bullet *quadrante_to_direcao(bullet *nova,int rot){
      int quadrante;
      quadrante = verificaQuadrante(rot);

      switch (quadrante)
      {
      case 1:
         nova->direcaoX = 1;
         nova->direcaoZ = 0;
         break;
      
      case 2:
         nova->direcaoZ = 1;
         nova->direcaoX = 0;
         break;
      
      case 3:
         nova->direcaoX = -1;
         nova->direcaoZ = 0;
         break;

      case 4:
         nova->direcaoX = 0;
         nova->direcaoZ = -1;
         break;

      
      default:
         break;
      }
      
      nova->direcaoY = mainChar->direcaoy;
      return nova;

}


// Função que cria uma bala
void createBullet(int tipoCam,int rot,ISoundEngine *engine){
    bullet *aux = NULL, *newBullet = NULL;
    rot1 = rot;
    tipoCam1 = tipoCam;
    
    // Verifica se o jogador possui tiros
    if (mainChar->currentAmmo > 0) {
        newBullet = new bullet();

        newBullet->next = NULL;

        // Definindo a posição e direção da bala
        newBullet->coordX = mainChar->charx;
        newBullet->coordY = mainChar->chary;
        newBullet->coordZ = mainChar->charz;
        if(tipoCam >0 ){
            newBullet->direcaoX = mainChar->direcaox;
            newBullet->direcaoY = mainChar->direcaoy;
            newBullet->direcaoZ = mainChar->direcaoz;
        }
        else
        {
            newBullet = quadrante_to_direcao(newBullet,rot);
        }
        

        //Reproduz o som
        ISound* music = engine->play2D("assets/shoot.mp3", false);

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
    float valor = 0.78;
    int quadrante = verificaQuadrante(rot1);
    glPushMatrix();
        glColor3ub(0, 255, 0);
        if(rot1 > 2)
            glTranslatef(b->coordX + 0.5, 1.4, b->coordZ -valor);
        else
        {
             glTranslatef(b->coordX + 0.5, 1.4, b->coordZ +valor);
        }
        
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

// Função que pega munição do cenário
void getAmmo(ISoundEngine* engine) {
    //Reproduz o som
    ISound* music = engine->play2D("assets/reload.mp3", false);
    mainChar->currentAmmo = mainChar->currentAmmo + 2;
    currentPhase->map[mainChar->charx][mainChar->charz] = 0;
}

void clearBullets(){
    bullet *aux = NULL, *previous = NULL;

    if (bulletList != NULL) {
        previous = bulletList;
        aux = bulletList->next;
        do {
            delete(previous);
            previous = aux;
            aux = previous->next;
        } while (aux->next != NULL);
        delete(aux);
    }

    bulletList = NULL;
}

#endif