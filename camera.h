#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>
#include "character.h"

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

int tipoCam = 1;
double t = 0;
float increm = 0.017453293;


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
      posy = 20;
      posz = -5;

      //Define para onde a lente da câmera estará apontada
      ox = mainChar->charx;
      oy = mainChar->chary;
      oz = 0;

      //Define eixo que vai estar a vertical no monitor
      lx = 0;
      ly = 1;
      lz = 0;
   }
}