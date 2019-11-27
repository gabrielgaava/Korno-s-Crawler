// Biblioteca relacionada as funções do HUD

//Verifica se a biblioteca já foi importada
#ifndef _HUD_H_
#define _HUD_H_

//Bibliotecas utilizadas nesse arquivo
#include <GL/glut.h>
#include <iostream>
using namespace std;

/* Declaração de variáveis globais */
int nowHud = 0;

/* Declaração dos protótipos das funções */
void drawText(const char *text, int length, float x, float y);
void HUD();
void gameOver();
void drawHUD(int hud);

//Função que rasteriza texto para inserir na HUD
void drawText(const char *text, int length, float x, float y){
   glMatrixMode(GL_PROJECTION);
   double *matrix = new double[16];
   glGetDoublev(GL_PROJECTION_MATRIX, matrix);
   glLoadIdentity();
   glOrtho(-10, 10, -10, 10, -10, 10);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   glPushMatrix();
   glLoadIdentity();
   glRasterPos2f(x,y);
   for(int i = 0; i < length; i++){
      glutBitmapCharacter(GLUT_BITMAP_9_BY_15, (int)text[i]);
   }
   glPopMatrix();
   glMatrixMode(GL_PROJECTION);
   glLoadMatrixd(matrix);
   glMatrixMode(GL_MODELVIEW);
}

//Função de criação dos elementos da HUD
void HUD(){
   //glBindTexture(GL_TEXTURE_2D, HUDtex);
   glColor4ub(245,86,112, 255);
   glRectd(mainChar->lifePerc*-1, 0.02, mainChar->lifePerc, -0.02);

   int life = ceil(mainChar->pLife);
   string text = "Vida: "+ to_string(life) +"%";
   drawText(text.data(), text.size(), -9.5, 9);

   glColor4f(1.0, 1.0, 1.0, 1.0);
   string text3 = "Fase atual:  " + to_string(idPhase);
   drawText(text3.data(), text3.size(), 6.6, 9);

   glColor4f(1.0, 1.0, 1.0, 1.0);
   string text2 = "Balas:  " + to_string(mainChar->currentAmmo);
   drawText(text2.data(), text2.size(), 6.6, 8.5);

   glColor4f(1.0, 1.0, 1.0, 1.0);
   string text4 = "Traps: " + to_string(mainChar->currentTraps);
   drawText(text4.data(), text4.size(), 6.6, 8);

   if(idPhase >= 10){
      glColor4f(1.0, 1.0, 1.0, 1.0);
      string text5 = "PARABÉNS, VOCÊ PLATINOU O JOGO";
      drawText(text4.data(), text4.size(), 6.6, 8);
   }
   
}

//"Tela" de Game over
void gameOverHUD(){
   string gameOverTxt = "G A M E - O V E R";
   glColor4f(1.0, 0.0, 0.0, 1.0);
   drawText(gameOverTxt.data(), gameOverTxt.size(), -1.5, 0);

   string instTxt = "Aperte <R> para comecar novamente";
   glColor4f(1.0, 1.0, 1.0, 1.0);
   drawText(instTxt.data(), instTxt.size(), -3.2, -1);

   glColor4f(0.0, 0.0, 0.0,0.8);
   glRectd(-1,1,1,-1);
}

void welcomeHUD(){
   string gameOverTxt = "Bem vindo ao KORNO'S CRAWLER!";
   glColor4f(1.0, 0.0, 0.0, 1.0);
   drawText(gameOverTxt.data(), gameOverTxt.size(), -3, 0);

   string instTxt = "Para comecar aperte <ENTER> ... e Boa Sorte.";
   glColor4f(1.0, 1.0, 1.0, 1.0);
   drawText(instTxt.data(), instTxt.size(), -4.5, -1);

   string instTxt2 = "Pressione <H> para ajuda";
   glColor4f(1.0, 1.0, 1.0, 0.8);
   drawText(instTxt2.data(), instTxt2.size(), -2.5, -1.7);

   glColor4f(0.0, 0.0, 0.0,0.8);
   glRectd(-1,1,1,-1); 
}

void helpHUD(){
   string titleTxt = "Guia para o Noob:";
   glColor4f(1.0, 0.0, 0.0, 1.0);
   drawText(titleTxt.data(), titleTxt.size(), -9, 9);

   string instTxt = "Para voltar aperte <H>";
   glColor4f(1.0, 1.0, 1.0, 1.0);
   drawText(instTxt.data(), instTxt.size(), -9, 8.5);

   string cmTxt =  "<W A S D> - Movimentacao";
   string cmTxt2 = "<SPACE> - Atirar";
   string cmTxt3 = "<P> - Troca de Perspectiva";
   string cmTxt4 = "<I/O> - Rotacao da Camera";
   string cmTxt5 = "<T> - Colocar Armadilha";

   glColor4f(1.0, 1.0, 1.0, 0.7);
   drawText(cmTxt.data(), cmTxt.size(), -9, 7);
   drawText(cmTxt2.data(), cmTxt2.size(), -9, 6.5);
   drawText(cmTxt3.data(), cmTxt3.size(), -9, 6);
   drawText(cmTxt4.data(), cmTxt4.size(), -9, 5.5);
   drawText(cmTxt5.data(), cmTxt5.size(), -9, 5);


   glColor4f(0.0, 0.0, 0.0,0.8);
   glRectd(-1,1,1,-1); 
}

//Função responsavel por contruir a HUD
void drawHUD(int hud){

   glMatrixMode(GL_PROJECTION);
   glPushMatrix();
   glLoadIdentity();
   gluOrtho2D(0.0, 1.0, 1.0, 0.0);

   glMatrixMode(GL_MODELVIEW);
   glPushMatrix();
   glLoadIdentity();

   if(hud == 0)
      welcomeHUD();
   if(hud == 1)
      HUD();
   if(hud == 2)
      gameOverHUD();
   if(hud == 3)
      helpHUD();

   glMatrixMode(GL_PROJECTION);
   glPopMatrix();
   glMatrixMode(GL_MODELVIEW);
   glPopMatrix();
}

#endif