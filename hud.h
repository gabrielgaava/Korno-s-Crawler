// Biblioteca relacionada as funções do HUD

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
   glColor4f(1.0, 0.0, 0.0, 1.0);
   glRectd(mainChar->lifePerc*-1, 0.02, mainChar->lifePerc, -0.02);

   string text = "Vida: "+ to_string(mainChar->pLife) +"%";
   drawText(text.data(), text.size(), 6, 9);

   glColor4f(1.0, 1.0, 1.0, 1.0);
   string text2 = "Balas:  10/10";
   drawText(text2.data(), text2.size(), 6.6, 8.5);
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

   string instTxt = "Para começar aperte <ENTER> ... e Boa Sorte.";
   glColor4f(1.0, 1.0, 1.0, 1.0);
   drawText(instTxt.data(), instTxt.size(), -4.5, -1);

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

   glMatrixMode(GL_PROJECTION);
   glPopMatrix();
   glMatrixMode(GL_MODELVIEW);
   glPopMatrix();
}