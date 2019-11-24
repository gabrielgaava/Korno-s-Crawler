//Código fonto do jogo Korno's Crawler

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <GL/glut.h>
#include <iostream>
#include "map.h"
#include "character.h"
#include "monster.h"
#include <iostream>
using namespace std;


/* Declaração de Variáveis Globais */

//Variáveis de definição de ângulo
int angulo1 = 0, angulo2 = 0, angulo3 = 0;
//Variáveis que definem a posição da câmera
int posx, posy, posz;
//Variável que define para onde a câmera olha
int oy, ox, oz;
//Variável que definem qual eixo estará na vertical do monitor
int lx, ly,lz;
double rotation, inclination;
int botao,estado;
//Variável que definee o tipo da câmera
int tipoCam = 1;
double t = 0;

//Vida do jogador
float pLife = 100;
float lifePerc = 1;
bool isDead = false;
int nowHud = 0;


//Variaveis para contagem de FPS
int initial_time = time(NULL), final_time, frame_count = 0;

#ifndef DEG_TO_RAD
#define DEG_TO_RAD 0.017453292519943295769236907684886
#endif

//Protótipos das funções
void Inicializa();
void Display();
void Mouse(int botao, int estado, int x, int y);
void keyboard (unsigned char key, int x, int y);
void TeclasEspeciais (int key, int x, int y);
void HUD();
void gameOver();
void drawHUD(int hud);
void timer(int);
void buildFrame();
void createGame();
void getLife(int x, int z);

//Função que movimenta a câmera
void moveCamera(){
   posx = 30*sin(t);
   posz = 30*cos(t);
}

bool enableMouseMovement = false;
void UpdateCameraCenter(bool CheckValue) {
  if(CheckValue) {
    if(rotation >= 360)
      rotation -= 360;
    if(rotation < 0)
      rotation += 360;
    
    if(inclination <= 45)
      inclination = 45;
    if(inclination >= 135)
      inclination = 135;
  }
  
  ox = (sin(DEG_TO_RAD * inclination) * sin(DEG_TO_RAD * rotation) + posx);
  oy = (sin(DEG_TO_RAD * inclination) * cos(DEG_TO_RAD * rotation) + posy);
  oz = (cos(DEG_TO_RAD * inclination) + posz);
}

//Função que ajusta a câmera
void adjustCamera(){
   //Camera 2D --
   if(tipoCam > 0){ 
      enableMouseMovement = false;
      //Define a posição da câmera
      posx = mainChar->charx; //charx
      posy = 9;
      posz = mainChar->charz; //charz

      //Define para onde a lente da câmera estará apontada
      ox = mainChar->charx; //charx
      oy = 0;
      oz = mainChar->charz; //charz

      //Define eixo que vai estar a horizontal no monitor
      lx = 1;
      ly = 0;
      lz = 0;
   }
   //Camera 3D --
   else if(tipoCam < 0){
      posx = mainChar->charx;
      posz = mainChar->charz;
      posy = 2;
      lx = 0;
      ly = 1;
      lz = 0;
      oy = 2;
   }
}

//Função que cria o Frame
void buildFrame() {
   if(nowHud != 0){
      //Constroi o mapa
      buildPhase();

      //Posiciona o jogador
      buildMainChar();

      //Realiza o movimento dos monstros
      moveMonsters();

      //Posiciona os monstros
      buildMonsters();
   }
}

//Função para coletar orbe de vida
void getLife(int x, int z){
   cout << "Vida coletada!" << endl;
   pLife = pLife + 5;
   //Vida maxima = 100
   if(pLife > 100)
      pLife = 100;
   
   currentPhase->map[x][z] = 1;
}

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
   glRectd(lifePerc*-1, 0.02, lifePerc, -0.02);

   string text = "Vida: "+ to_string(pLife) +"%";
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

//Timer
void timer(int){
   glutPostRedisplay();
   glutTimerFunc(10,timer,0);
}

//Função executada sempre que nenhuma ação é tomada naquele "quadro"
void idle(){
   glutPostRedisplay();
   if(!isDead && nowHud != 0){
      pLife = pLife - 0.150;
      cout << "Life: " << pLife << endl;
      //Atualiza a porcentagem de vida do jogador pra
      //ser mostrada na barra de vida
      lifePerc = pLife/100;

      if(pLife <= 0){
         isDead = true;
         pLife = 0;
         lifePerc = 0;
         nowHud = 2;
         cout << "Voce MORREU!";
      }
   }
}

//Função que cria as variáveis do jogo
void createGame() {
   //Inicializa (ou cria uma nova) fase com salas e paredes
   createPhase();

   //Cria novos monstros
   createMonsters();
}

//Função qeu configura o display
void Display() {
   
   glEnable(GL_DEPTH_TEST);
   glEnable(GL_LINE_SMOOTH);
   glEnable(GL_POLYGON_SMOOTH); 
   glEnable(GL_SMOOTH);
   glEnable(GL_BLEND);
   
   // Inicializa parâmetros de rendering
   // Define a cor de fundo da janela de visualização como preta
   glClearColor(0.0, 0.0, 0.0, 0.0); 
   
   //glMatrixMode() - define qual matriz será alterada
	//SEMPRE defina o tipo de apresentação na matriz PROJECTION
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

   if (tipoCam > 0) {
		//Define a projeção como ortogonal
      glOrtho(-10, 10, -10, 10, -10, 10);
	} else {
      //Define a projeção como perspectiva
		gluPerspective(45,1,1,150);

	}
   
   //glMatrixMode() define qual matriz será alterada
	//SEMPRE defina a câmera (ortogonal ou perspectiva) na matriz MODELVIEW (onde o desenho ocorrerá)
	glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   
   //Chamada para ajustar a câmera de acordo com a visão atual
   adjustCamera();    

   //Define a pos da câmera, para onde olha e qual eixo está na vertical
   gluLookAt(posx, posy, posz, ox, oy, oz, lx, ly, lz);

   //Limpa um buffer particular ou combinações de buffers, onde buffer é uma área de armazenamento para informações da imagem
	//Nesse caso está limpando os buffers para suportarem animações
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   
   //Chamada para Função que desenha o objeto/cena...
   //----------------------------------------------------------------
   buildFrame();
   //----------------------------------------------------------------   
   //Executa a cena
	//SwapBuffers dá suporte para mais de um buffer, permitindo execução de animações sem cintilações
   drawHUD(nowHud);
	glutSwapBuffers(); 

   frame_count++;
   final_time = time(NULL);
   if(final_time - initial_time > 0){
      cout << "FPS: " << frame_count / (final_time - initial_time)  << endl;
      frame_count = 0;
      initial_time = final_time;
   }
}

void Mouse(int botao, int estado, int rotation, int inclination) {
   //botao - recebe o código do botão pressionado
   //estado - recebe se está pressionado ou não
   //x, y - recebem respectivamente as posições do mouse na tela
   switch (botao) {
      case GLUT_LEFT_BUTTON:
      break;

      case GLUT_RIGHT_BUTTON:
      break;
   }

}

void keyboard (unsigned char key, int x, int y) {
   //Key - recebe o código ASCII da tecla
   //x, y - recebem as posições do mouse na tela (permite tratar os dois dispositivos)
      //Caso seja 2D

      //Passa para int a posição do personagem
      int posX = (int) mainChar->charx;
      int posZ = (int) mainChar->charz;
      switch (key) {
         case 'w':
            //Proxima direção tenha chão
            if (currentPhase->map[posX + 1][posZ] == 1) {
               if (mainChar->direcaox != 1) {
                  mainChar->direcaox = 1;
                  mainChar->direcaoz = 0;
               } else {
                  mainChar->charx++;   
               }
               if (tipoCam == -1)
               {
                  ox = mainChar->direcaox + mainChar->charx;
                  oz = mainChar->charz + mainChar->direcaoz;
               }
            }

            //Proxima direção tenha uma vida
            if(currentPhase->map[posX + 1][posZ] == 9){
               if (mainChar->direcaox != 1) {
                  mainChar->direcaox = 1;
                  mainChar->direcaoz = 0;
               } else {
                  mainChar->charx++;   
               }
               if (tipoCam == -1)
               {
                  ox = mainChar->direcaox + mainChar->charx;
                  oz = mainChar->charz + mainChar->direcaoz;
               }
               getLife(posX+1, posZ);
            }
            break;
         case 's':
            if (currentPhase->map[posX - 1][posZ] == 1) {
               
               if (mainChar->direcaox != -1) {
                  mainChar->direcaox = -1;
                  mainChar->direcaoz = 0;
               } else {
                  mainChar->charx--;
               }
               if (tipoCam == -1)
               {
                  ox = mainChar->direcaox + mainChar->charx;
                  oz = mainChar->charz + mainChar->direcaoz;
               }
            }

             //Proxima direção tenha uma vida
            if(currentPhase->map[posX - 1][posZ] == 9){
               if (mainChar->direcaox != 1) {
                  mainChar->direcaox = -1;
                  mainChar->direcaoz = 0;
               } else {
                  mainChar->charx--;   
               }
               if (tipoCam == -1)
               {
                  ox = mainChar->direcaox + mainChar->charx;
                  oz = mainChar->charz + mainChar->direcaoz;
               }
               getLife(posX-1, posZ);
            }
            break;
         case 'a':
            //Chão
            if (currentPhase->map[posX][posZ - 1] == 1) {
               if (mainChar->direcaoz != -1) {
                  mainChar->direcaox = 0;
                  mainChar->direcaoz = -1;
               } else {
                  mainChar->charz--;
               }
               if (tipoCam == -1)
               {
                  ox = mainChar->direcaox + mainChar->charx;
                  oz = mainChar->charz + mainChar->direcaoz;
               }
            }
            //Vida
            if (currentPhase->map[posX][posZ - 1] == 9) {
               if (mainChar->direcaoz != -1) {
                  mainChar->direcaox = 0;
                  mainChar->direcaoz = -1;
               } else {
                  mainChar->charz--;
               }
               if (tipoCam == -1)
               {
                  ox = mainChar->direcaox + mainChar->charx;
                  oz = mainChar->charz + mainChar->direcaoz;
               }
               getLife(posX,posZ-1);
            }
            
            break;
         case 'd':
            //Chão
            if (currentPhase->map[posX][posZ + 1] == 1) {
               if (mainChar->direcaoz != 1) {
                  mainChar->direcaox = 0;
                  mainChar->direcaoz = 1;
               } else {
                  mainChar->charz++;
               }
               if (tipoCam == -1)
               {
                  ox = mainChar->direcaox + mainChar->charx;
                  oz = mainChar->charz + mainChar->direcaoz;
               }
            }
            //Vida
            if (currentPhase->map[posX][posZ + 1] == 9) {
               if (mainChar->direcaoz != 1) {
                  mainChar->direcaox = 0;
                  mainChar->direcaoz = 1;
               } else {
                  mainChar->charz++;
               }
               if (tipoCam == -1)
               {
                  ox = mainChar->direcaox + mainChar->charx;
                  oz = mainChar->charz + mainChar->direcaoz;
               }
               getLife(posX, posZ+1);
            }
            break;
         case 'p':
            tipoCam *= -1;
            adjustCamera();
            break;
         
         case 'r':
            if(nowHud == 2){
               //Inicializa as variáveis da Phase 1 do jogo
               createGame();
               //Inicializa as variaveis do personagem principal
               createMainChar();
               pLife = 100;
               isDead = false;
               nowHud = 1;
            }
            break;
         
         case 13:
            if(nowHud == 0)
               nowHud = 1;
            break;

         default:
            break;
      }
   glutPostRedisplay();
}

void TeclasEspeciais (int key, int x, int y) {
   //Key - recebe o código ASCII da tecla
   //x, y - recebem respectivamente as posições mouse na tela (permite tratar os dois dispositivos)
   if (key==GLUT_KEY_RIGHT) {
      posx+=5; ox+=5;
   }
   else if (key==GLUT_KEY_PAGE_UP) {
      posy+=5; oy+=5;
   }
   else if (key==GLUT_KEY_UP) {
      posz-=5; oz-=5;
   }
   else if (key==GLUT_KEY_LEFT) { 
      posx-=5; ox-=5;
   }
   else if (key==GLUT_KEY_PAGE_DOWN) {
      posy-=5; oy-=5;
   }
   else if (key==GLUT_KEY_DOWN) {      
      posz+=5; oz+=5;
   }
   glutPostRedisplay();
}

int main(int argc,char **argv) {
   //Iniatizes glut
	glutInit(&argc, argv);

   //Seed da srand
   srand(time(NULL));
   
   //Define as características do espaço vetorial
	//Nesse caso, permite animações (sem cintilações), cores compostas por (R, G, B)
	//Buffer que permite trabalhar com profundidade e elimina faces escondidas
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);

   //Define a janela
   glutInitWindowSize(800, 800);
   glutInitWindowPosition(100, 100);
   glutCreateWindow("Korno's Crawler");

   //Inicializa as variáveis da Phase 1 do jogo
   createGame();

   //Inicializa as variaveis do personagem principal
   createMainChar();

   //Mostra o Mapa
	printMap();

   //Opacidade nas cores
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

   glutDisplayFunc(Display);
   glutMouseFunc(Mouse);
   glutKeyboardFunc(keyboard);
   glutSpecialFunc(TeclasEspeciais);
   glutIdleFunc(idle);
   //glutTimerFunc(0,timer,0);

   glutMainLoop();
   
   return 0; 
}
