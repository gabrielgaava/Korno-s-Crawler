//Programa exemplo para representa��es de elementos bidimensionais e tridimensionais. A estrutura permite manipular diferentes proje��es.

#include <stdlib.h>
#include <stdio.h>
#include <GL/glut.h>
#include <math.h>



//Declara��o de Vari�veis Globais
int angulo1 = 0, angulo2 = 0, angulo3 = 0;
int projecao=0; //Vari�vel L�gica para Definir o Tipo de Proje��o (Perspectiva ou Ortogonal)
int posx, posy, posz; //Vari�veis que definem a posi��o da c�mera
int oy,ox,oz;         //Vari�veis que definem para onde a c�mera olha
int lx, ly,lz;         //Vari�veis que definem qual eixo estar� na vertical do monitor.
float charx = 0, chary = 0, charz = 0;
double t = 0;
float increm = 0.017453293;
int tipoCam = 1;

//Prot�tipos das Fun��es
void Inicializa();
void Display();
void Mouse(int botao, int estado, int x, int y);
void keyboard (unsigned char key, int x, int y);
void TeclasEspeciais (int key, int x, int y);


void createMainChar(){
     
      int armsize =4.7,bodysize = 6,headsize = 1;
      float armposx = 1.7,armposy = 0.5,armposz = 0,headposx = 0,headposy = 5,headposz = 0;
      float bodyposx = 0,bodyposy = 0,bodyposz = 0;

      glTranslatef(charz,chary,charx);
      glPushMatrix();
      glColor3f(1.5,0,0);
      glScalef(1, bodysize, 1);
      glutWireCube(1.5);
      glTranslatef(bodyposx, bodyposy, bodyposz);
      glPopMatrix();

      glPushMatrix();
      glTranslatef(0.0, 5, 0);
      glutSolidSphere(1, 50, 50);
      glPopMatrix();

      glPushMatrix();
      glScalef(0.6,armsize,0.6);
      glTranslatef(armposx,armposy,armposz);
      glRotatef(angulo1,1,0,0);
      glutWireCube(1);
      glPopMatrix();

      glPushMatrix();
      glScalef(0.6,armsize,0.6);
      glTranslatef(-armposx,armposy,armposz);
      glRotatef(angulo1,1,0,0);
      glutWireCube(1);
      glPopMatrix();

}

void createGround(){
   glPushMatrix();
   glTranslatef(0, -0.2, 0);
   glScalef(30, 0.1, 30);
   glColor3ub(255, 255, 255);
   glutSolidCube(1);
   glPopMatrix();
}

void moveCamera(){
   
   
   posx = 30*sin(t);
   posz = 30*cos(t);


}

void adjustCamera(){
   //Camera 2D --
   if(tipoCam > 0){ 

      posx = charx;
      posy = 30;
      posz = charz;

      ly = 0;
      lx = charx;
      lz = charz;

      oy = 0;
      ox = 1;
      oz = 0;
   }
   //Camera 3D --
   else if(tipoCam < 0){

      posx = charx;
      posy = chary +2;
      posz = charz;

      oy = 1;
      ox = 0;
      oz = 0;

      ly = 5;
      lx = charx;
      lz = charz;

   }


}


void Display()
{
   
   glEnable(GL_DEPTH_TEST);
   
   glEnable(GL_LINE_SMOOTH);
   glEnable(GL_POLYGON_SMOOTH); 

   glEnable(GL_SMOOTH);
   glEnable(GL_BLEND);
   
   // Inicializa par�metros de rendering
    // Define a cor de fundo da janela de visualiza��o como preta
   glClearColor(0.0, 0.0, 0.0, 0.0); 
   
   
   glMatrixMode(GL_PROJECTION);/*glMatrixMode()- define qual matriz ser� alterada. SEMPRE defina o tipo de apresenta��o 
                              (Ortogonal ou Perspectiva) na matriz PROJECTION.*/
   glLoadIdentity();//"Limpa" ou "transforma" a matriz em identidade, reduzindo poss�veis erros.

   if (projecao==1)
      glOrtho(-150, 150, -150, 150, -150, 150); //Define a proje��o como ortogonal
   else
      gluPerspective(45,1,1,50); //Define a proje��o como perspectiva
   
   glMatrixMode(GL_MODELVIEW);/*glMatrixMode()- define qual matriz ser� alterada. SEMPRE defina a c�mera 
                              (Ortogonal ou Perspectiva) na matriz MODELVIEW (onde o desenho ocorrer�).*/
   glLoadIdentity(); ////"Limpa" ou "transforma" a matriz em identidade, reduzindo poss�veis erros.
         adjustCamera();    

   gluLookAt(posx,posy,posz,ox,oy,oz,lx,ly,lz); //Define a pos da c�mera, para onde olha e qual eixo est� na vertical.
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); /* "limpa" um buffer particular ou combina��es de buffers, 
                                                         onde buffer � uma �rea de armazenamento para informa��es da imagem. 
                                                        Nesse caso, est� "limpando os buffers para suportarem anima��es */
   //Chamada para Fun��o  ou fun��es para desenhar o objeto/cena...
   //----------------------------------------------------------------
   //glPushMatrix(); //Salva o estado atual da cena. O que for desenhado ap�s o Push n�o influenciar� o j� representado
      createGround();
      glColor3ub(100, 255, 40);
      createMainChar();
      


  // glPopMatrix();  //Retorna ao estado anterior da cena. O que for desenhado ap�s o Push n�o influenciou o j� representado
   //----------------------------------------------------------------   
   
   glutSwapBuffers(); //Executa a Cena. SwapBuffers d� suporte para mais de um buffer, permitindo execu��o de anima��es sem cintila��es. 
}

void Mouse(int botao, int estado, int x, int y)
{  //bot�o - recebe o c�digo do bot�o pressionado
   //estado - recebe se est� pressionado ou n�o
   //x, y - recebem respectivamente as posi��es do mouse na tela
   switch (botao)
   {
      case GLUT_LEFT_BUTTON:

      break;

      case GLUT_RIGHT_BUTTON:
      if(estado == GLUT_DOWN)
      {
         projecao=0;
         posx=0; posy=10; posz=20;
         oy=0; ox=0;  oz=0;
         lx=0, ly=1, lz=0;
         glutPostRedisplay();
      }
      break;
   }
}

void keyboard (unsigned char key, int x, int y)
{//Key - recebe o c�digo ASCII da tecla
 //x, y - recebem as posi��es do mouse na tela (permite tratar os dois dispositivos)
    if(key == 'w')
      charx ++;    
    else if(key == 's')
        charx --;
    else if(key == 'a')
         charz --;
    else if(key == 'd')
         charz ++;
    else if(key == 'q'){
         t = (t + increm);
      moveCamera();
    }
    else if(key == 'p'){
         tipoCam *= -1;
         adjustCamera();
    }

      glutPostRedisplay();
}

void TeclasEspeciais (int key, int x, int y)
{//Key - recebe o c�digo ASCII da tecla
 //x, y - recebem respectivamente as posi��es mouse na tela (permite tratar os dois dispositivos)
      if (key==GLUT_KEY_RIGHT)
      {
         posx+=5; ox+=5;
      }
      else if (key==GLUT_KEY_PAGE_UP)
      {
         posy+=5; oy+=5;
      }
      else if (key==GLUT_KEY_UP)
      {
         posz-=5; oz-=5;
      }
      else if (key==GLUT_KEY_LEFT)    
      { 
         posx-=5; ox-=5;
      }
      else if (key==GLUT_KEY_PAGE_DOWN)
      {
         posy-=5; oy-=5;
      }
      else if (key==GLUT_KEY_DOWN)
      {      
         posz+=5; oz+=5;
      }
      glutPostRedisplay();
}


int main(int argc,char **argv)
{
   glutInit(&argc, argv); // Initializes glut
    
   
   glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); /*Define as caracter�sticas do espa�o vetorial. 
                                                                           //  Nesse caso, permite anima��es (sem cintila��es), cores compostas por Verm. Verde e Azul,
                                                                           //  Buffer que permite trablhar com profundidade e elimina faces escondidas.*/           
  
   glutInitWindowSize(800, 800);
   glutInitWindowPosition(100, 100);
   glutCreateWindow("Arms Simulation");
   glutDisplayFunc(Display);
   glutMouseFunc(Mouse);
   glutKeyboardFunc(keyboard);
   glutSpecialFunc(TeclasEspeciais);
   glutMainLoop();
   return 0; 
}
