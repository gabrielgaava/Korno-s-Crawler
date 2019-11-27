//Código fonte do jogo Korno's Crawler

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <GL/glut.h>
#include <iostream>

//Para a Engine de Som
#include "assets/soundEngine/irrKlang.h"
#pragma comment(lib, "irrKlang.lib")
using namespace irrklang;
using namespace std;

#include "map.h"
#include "character.h"
#include "monster.h"
#include "hud.h"
#include "bullet.h"

/* Declaração de Variáveis Globais */

//Variáveis que definem a posição da câmera
float posx, posy, posz;

//Variável que define para onde a câmera olha
float oy, ox, oz;

//Variável que definem qual eixo estará na vertical do monitor
int lx, ly,lz;

//Variável que define o tipo da câmera
int tipoCam = 1;

//Variavel que define a rotação da câmera
float rot = 0;

// Variável que define o número da fase
int numberPhase = 1;

//Device de Som
ISoundEngine* engine = createIrrKlangDevice();

//Variaveis para contagem de FPS
int initial_time = time(NULL), final_time, frame_count = 0;

#ifndef DEG_TO_RAD
#define DEG_TO_RAD 0.017453292519943295769236907684886
#endif

/* Declaração dos Protótipos das funções */
void Inicializa();
void Display();
void Mouse(int botao, int estado, int x, int y);
void keyboard (unsigned char key, int x, int y);
void TeclasEspeciais (int key, int x, int y);
void timer(int);
void buildFrame();
void getLife();
void clearVariables();
void startGame();

//Função para coletar orbe de vida
void getLife(){
   mainChar->pLife = mainChar->pLife + 20;
    
   //Vida maxima = 100
   if(mainChar->pLife > 100) {
     mainChar->pLife = 100;
   }
    
   currentPhase->map[mainChar->charx][mainChar->charz] = 0;

   // play some sound stream, looped, in 3D space
   ISound* music = engine->play2D("assets/life.wav", false);
}

//Função para criar um TRAP
void putTrap(int x, int z){
   if(mainChar->currentTraps >= 1){
      mainChar->currentTraps -= 1;
      currentPhase->map[x][z] = 5;
      ISound* music = engine->play2D("assets/trap.wav", false);
   }
}

//Função que ajusta a câmera
void adjustCamera(){
   //Camera 2D --
   if(tipoCam > 0){ 
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
      // Define a posição da câmera 
      posx = mainChar->charx;
      posy = 2;
      posz = mainChar->charz + 0.5;

      //Define onde a lente da câmera estará apontada
      ox = cos(DEG_TO_RAD*rot) + posx ;
      oy = 2;
      oz = sin(DEG_TO_RAD*rot) + posz ;

      // Define que eixo vai estar a horizontal
      lx = 0;
      ly = 1;
      lz = 0;
   }
}

//Função que cria o Frame
void buildFrame() {

   if(nowHud != 0){
      // Constroi o mapa
      buildPhase();

      // Posiciona o jogador
      buildMainChar();

      // Realiza as movimentações relacionadas as balas, e as coloca no mapa
      moveBullets();

      //Realiza o movimento dos monstros e posiciona os monstros
      moveMonsters(engine);

      // Posiciona os monstros
      buildMonsters();
   }
}

//Timer
void timer(int){
   glutPostRedisplay();
   glutTimerFunc(10,timer,0);
}

//Função executada sempre que nenhuma ação é tomada naquele "quadro"
void idle(){
   glutPostRedisplay();
   if(!mainChar->isDead && nowHud != 0){
      mainChar->pLife = mainChar->pLife - 0.05;
      //cout << "Life: " << mainChar->pLife << endl;
      //Atualiza a porcentagem de vida do jogador pra
      //ser mostrada na barra de vida
      mainChar->lifePerc = mainChar->pLife/100;

      if(mainChar->pLife <= 0){
         mainChar->isDead = true;
         mainChar->pLife = 0;
         mainChar->lifePerc = 0;
         nowHud = 2;
         engine->stopAllSounds();
         ISound* music = engine->play2D("assets/gameOver.mp3", false);
         cout << "Voce MORREU!";
         
      }
   }
}

// Função que inicia o jogo
void startGame() {
   //Inicializa (ou cria uma nova) fase com salas e paredes
   createPhase();

   //Cria novos monstros
   createMonsters();

   //Inicializa as variaveis do personagem principal
   createMainChar();
}

//Função que limpa as variáveis do jogo
void clearVariables() {
   ISound* music = engine->play2D("assets/sucess.mp3", false);
   clearBullets();
   clearMonsters();
   clearMainChar();
   clearCurrentPhase();
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
		gluPerspective(
      (45.0 * glutGet(GLUT_WINDOW_WIDTH)/glutGet(GLUT_WINDOW_HEIGHT)),
      glutGet(GLUT_WINDOW_WIDTH)/glutGet(GLUT_WINDOW_HEIGHT),
      glutGet(GLUT_WINDOW_WIDTH)/glutGet(GLUT_WINDOW_HEIGHT),
      //(100.0 * Window.CurrentRatio)
      (200.0 * glutGet(GLUT_WINDOW_WIDTH)/glutGet(GLUT_WINDOW_HEIGHT))
    );

	}

   //Chamada para ajustar a câmera de acordo com a visão atual
   adjustCamera();

   //glMatrixMode() define qual matriz será alterada
	//SEMPRE defina a câmera (ortogonal ou perspectiva) na matriz MODELVIEW (onde o desenho ocorrerá)
	glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();

   //Define a pos da câmera, para onde olha e qual eixo está na vertical
   gluLookAt(posx, posy, posz, ox, oy, oz, lx, ly, lz);

   //Limpa um buffer particular ou combinações de buffers, onde buffer é uma área de armazenamento para informações da imagem
	//Nesse caso está limpando os buffers para suportarem animações
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   
   //Chamada para Função que desenha o objeto/cena...
   //----------------------------------------------------------------
   buildFrame();
   drawHUD(nowHud);
   //----------------------------------------------------------------   
	//SwapBuffers dá suporte para mais de um buffer, permitindo execução de animações sem cintilações
	glutSwapBuffers();

   //Contagem de frames
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

void keyboard2d(unsigned char key) {
   //Key - recebe o código ASCII da tecla
   fflush(stdin);

   //Passa para int a posição do personagem
   short posX = mainChar->charx;
   short posZ = mainChar->charz;

   switch (key) {
      case 'w':
         //Verifica se é possível caminhar
         if (currentPhase->map[posX + 1][posZ] >= 0 && verifyMonsterPosition(posX + 1, posZ) == NULL) {
            //Verifica se o personagem está virado para a direção 
            if (mainChar->direcaox != 1) {
               //Se não estiver, vira o personagem
               mainChar->direcaox = 1;
               mainChar->direcaoz = 0;
            } else {
               // Se estiver, caminha e executa outras ações (se houver)
               mainChar->charx++;
            }
         }
         break;

      case 's':
         //Lógica análoga a W
         if (currentPhase->map[posX - 1][posZ] >= 0 && verifyMonsterPosition(posX - 1, posZ) == NULL) {
            if (mainChar->direcaox != -1) {
               mainChar->direcaox = -1;
               mainChar->direcaoz = 0;
            } else {
               mainChar->charx--;
            }
         }
         break;

      case 'a':
         //Lógica análoga a W
         if (currentPhase->map[posX][posZ - 1] >= 0 && verifyMonsterPosition(posX, posZ - 1) == NULL) {
            if (mainChar->direcaoz != -1) {
               mainChar->direcaox = 0;
               mainChar->direcaoz = -1;
            } else {
               mainChar->charz--;
            }
         }
         break;

      case 'd':
         //Lógica análoga a W
         if (currentPhase->map[posX][posZ + 1] >= 0  && verifyMonsterPosition(posX, posZ + 1) == NULL) {
            if (mainChar->direcaoz != 1) {
               mainChar->direcaox = 0;
               mainChar->direcaoz = 1;
            } else {
               mainChar->charz++;
            }
         }
         break;

      default:
         break;
   }
}


void walk3d(int valor){
   int quadrante;
   quadrante = verificaQuadrante(rot);

   switch (quadrante)
   {
   case 1:
      if(currentPhase->map[mainChar->charx-valor][mainChar->charz] >= 0)
         mainChar->charx -= valor;
      break;
   
   case 2:
      if(currentPhase->map[mainChar->charx][mainChar->charz-valor] >= 0)
         mainChar->charz -= valor;
      break;
   
   case 3:
      if(currentPhase->map[mainChar->charx+valor][mainChar->charz] >= 0)
         mainChar->charx += valor;
      break;
   
   
   case 4:
      if(currentPhase->map[mainChar->charx][mainChar->charz+valor] >= 0)
         mainChar->charz += valor;
      break;

   default:
      break;
   }
}

void walk3d2(int valor){

   int quadrante;
   quadrante = verificaQuadrante(rot);

   switch (quadrante)
   {
   case 4:
      if(currentPhase->map[mainChar->charx+valor][mainChar->charz] >= 0)
         mainChar->charx += valor;
      break;
   
   case 3:
      if(currentPhase->map[mainChar->charx][mainChar->charz-valor] >= 0)
         mainChar->charz -= valor;
      break;
   
   case 2:
      if(currentPhase->map[mainChar->charx-valor][mainChar->charz] >= 0)
         mainChar->charx -= valor;
      break;
   
   case 1:
      if(currentPhase->map[mainChar->charx][mainChar->charz+valor] >= 0)
         mainChar->charz += valor;
      break;

   default:
      break;
   }

}

void keyboard3d(unsigned char key, int x, int y) {
   //Key - recebe o código ASCII da tecla
   //x, y - recebem as posições do mouse na tela (permite tratar os dois dispositivos)

   switch(key) {

      case 's':
         walk3d(1);
         break;
      
      case 'w':
         walk3d(-1);
         break;

      case 'd':
         printf("rot %f quad %d\n",rot,verificaQuadrante(rot));
         walk3d2(1);
         break;
      
      case 'a':
          printf("rot %f quad %d\n",rot,verificaQuadrante(rot));
         walk3d2(-1);
         break;
      
      case 'i':
         rot -= 10;
         if (rot < 0) {
            rot = 355;
         }
         // Atualiza as direções do personagem
         updateCharRotDirection(rot);
         break;

      case 'o':
         rot += 10;
         if(rot == 360)
            rot = 0;
         // Atualiza as direções do personagem
         updateCharRotDirection(rot);   
         break;


   }
   adjustCamera();
}

void keyboard(unsigned char key, int x, int y) {
   //Key - recebe o código ASCII da tecla
   //x, y - recebem as posições do mouse na tela (permite tratar os dois dispositivos)

   //Mapeia teclas especiais que devem ser tratadas diferente dependendo do tipo da câmera
   char special_keys[] = {'w', 's', 'a', 'd','i','o'};
   bool isSpecialKey = false;
   for (int i=0; i < 6; i++) {
      if (key == special_keys[i]){
         isSpecialKey = true;
      }
   }

   // Verifica se é uma tecla especial
   if (isSpecialKey && !mainChar->isDead) {
      if (tipoCam > 0) {
         // Se a câmera for 2D
         keyboard2d(key);
      } else {
         // Se a câmera for 3D
         keyboard3d(key, x, y);
      }
   } else {
      // Se não é uma tecla especial, faz a lógica
      switch (key) {  
         case 'p':
            // Troca a câmera
            tipoCam *= -1;
            
            if (tipoCam > 0) {
               if (rot > 315 || rot <= 45) {
                  mainChar->direcaox = 1;
                  mainChar->direcaoz = 0;
               } else if (rot <= 135) {
                  mainChar->direcaox = 0;
                  mainChar->direcaoz = 1;
               } else if (rot <= 225) {
                  mainChar->direcaox = -1;
                  mainChar->direcaoz = 0;
               } else {
                  mainChar->direcaox = 0;
                  mainChar->direcaoz = -1;
               }
            } else {
               if (mainChar->direcaox != 0) {
                  if (mainChar->direcaox == 1) {
                     rot = 0;
                  } else {
                     rot = 180;
                  }
               } else {
                  if (mainChar->direcaoz == 1) {
                     rot = 90;
                  } else {
                     rot = 270;
                  }
               }
            }
            adjustCamera();
            break;
            
         case 'r':
            // Começa o jogo
            if(nowHud == 2){
               //Inicializa as variáveis
               ISound* music = engine->play2D("assets/theme.mp3", true);
               startGame();
               idPhase = 1;
               nowHud = 1;
            }
            break;
         
         case 13: //ENTER
            if(nowHud == 0)
               nowHud = 1;
            break;

         case 32:
            // Atira com ESPAÇO
            createBullet(tipoCam,rot,engine);
            break;
            
         default:
            break;
      }
   }

   // Após executar as ações enviadas pelo teclado, verifica se a posição não possui um item
   short valueMap = verifyMapContent(mainChar->charx, mainChar->charz);
   switch (valueMap) {
      case LIFE_SPHERE:
         getLife();
         break;

      case AMMO_DROP:
         getAmmo(engine);
         break;

      case EXIT:
         clearVariables();
         startGame();
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

   //Sound Engine
   if (!engine)
      return 0; // error starting up the engine 

   // play some sound stream, looped, in 3D space
   ISound* music = engine->play2D("assets/theme.mp3", true);

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

   // Chama a função para inicializar o jogo
   startGame();

   //Mostra o Mapa
	printMap();

   //Opacidade nas cores
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

   glutDisplayFunc(Display);
   glutMouseFunc(Mouse);
   fflush(stdin);
   glutKeyboardFunc(keyboard);
   glutSpecialFunc(TeclasEspeciais);
   glutIdleFunc(idle);
   //glutTimerFunc(0,timer,0);

   glutMainLoop();
   
   return 0; 
}
