//Código fonto do jogo Korno's Crawler

#include <stdlib.h>
#include <stdio.h>
#include <GL/glut.h>
#include <math.h>
#include <time.h>
#include "map.h"
#include "character.h"

/* Declaração de Variáveis Globais */

//Variáveis de definição de ângulo
int angulo1 = 0, angulo2 = 0, angulo3 = 0;
//Variável lógica para definir o tipo de projeção (perspectuva ou ortogonal)
int projecao=1;
//Variáveis que definem a posição da câmera
int posx, posy, posz;
//Variável que define para onde a câmera olha
int oy, ox, oz;
//Variável que definem qual eixo estará na vertical do monitor
int lx, ly,lz;

//Variável que definee o tipo da câmera
int tipoCam = 1;
double t = 0;
float increm = 0.017453293;

//Protótipos das funções
void Inicializa();
void Display();
void Mouse(int botao, int estado, int x, int y);
void keyboard (unsigned char key, int x, int y);
void TeclasEspeciais (int key, int x, int y);

//Função que movimenta a câmera
void moveCamera(){
   posx = 30*sin(t);
   posz = 30*cos(t);
}

//Função que ajusta a câmera
void adjustCamera(){
   //Camera 2D --
   if(tipoCam > 0){ 
      projecao = 1;
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
      projecao = 0;
      //Define a posição da câmera
      posx = 0;
      posy = 5;
      posz = -5;

      //Define para onde a lente da câmera estará apontada
      ox = 0;
      oy = 0;
      oz = 0;

      //Define eixo que vai estar a vertical no monitor
      lx = 0;
      ly = 1;
      lz = 0;
   }
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

   if (projecao==1) {
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
   
   //Chamada para Função  ou funções para desenhar o objeto/cena...
   //----------------------------------------------------------------
   buildPhase();
   buildMainChar();
   buildMonsters();
   //----------------------------------------------------------------   
   
   //Executa a cena
	//SwapBuffers dá suporte para mais de um buffer, permitindo execução de animações sem cintilações
	glutSwapBuffers(); 
}

void Mouse(int botao, int estado, int x, int y) {
   //botao - recebe o código do botão pressionado
   //estado - recebe se está pressionado ou não
   //x, y - recebem respectivamente as posições do mouse na tela
   switch (botao) {
      case GLUT_LEFT_BUTTON:
      break;

      case GLUT_RIGHT_BUTTON:
         if(estado == GLUT_DOWN) {
            projecao=0;
            posx=0; posy=10; posz=20;
            oy=0; ox=0;  oz=0;
            lx=0, ly=1, lz=0;
            glutPostRedisplay();
         }
      break;
   }
}

void keyboard (unsigned char key, int x, int y) {
   //Key - recebe o código ASCII da tecla
   //x, y - recebem as posições do mouse na tela (permite tratar os dois dispositivos)
   
   if (projecao == 1) {
      //Caso seja 2D
      switch (key) {
         case 'w':
            if (currentPhase->map[mainChar->charx + 1][mainChar->charz] == 1) {
               if (mainChar->direcaox != 1) {
                  mainChar->direcaox = 1;
                  mainChar->direcaoz = 0;
               } else {
                  mainChar->charx++;   
               }
            }
            break;
         case 's':
            if (currentPhase->map[mainChar->charx - 1][mainChar->charz] == 1) {
               
               if (mainChar->direcaox != -1) {
                  mainChar->direcaox = -1;
                  mainChar->direcaoz = 0;
               } else {
                  mainChar->charx--;
               }
            }
            break;
         case 'a':
            if (currentPhase->map[mainChar->charx][mainChar->charz - 1] == 1) {
               if (mainChar->direcaoz != -1) {
                  mainChar->direcaox = 0;
                  mainChar->direcaoz = -1;
               } else {
                  mainChar->charz--;
               }
            }
            break;
         case 'd':
            if (currentPhase->map[mainChar->charx][mainChar->charz + 1] == 1) {
               if (mainChar->direcaoz != 1) {
                  mainChar->direcaox = 0;
                  mainChar->direcaoz = 1;
               } else {
                  mainChar->charz++;
               }
            }
            break;
         case 'p':
            tipoCam *= -1;
            adjustCamera();
            break;
         default:
            printf("%c ", key);
            break;
      }
   } else {
      //Caso a projeção seja 3D
      switch(key) {
          case 'q':
            t = (t + increm);
            moveCamera();
            break;
         case 'p':
            tipoCam *= -1;
            adjustCamera();
            break;
      }
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
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

   //Define a janela
   glutInitWindowSize(800, 800);
   glutInitWindowPosition(100, 100);
   glutCreateWindow("Korno's Crawler");

   //Inicializa as variáveis da Phase 1 do jogo
   createNewPhase();
   createMap();
   createMainChar();

   glutDisplayFunc(Display);
   glutMouseFunc(Mouse);
   glutKeyboardFunc(keyboard);
   glutSpecialFunc(TeclasEspeciais);
   glutMainLoop();
   
   return 0; 
}
