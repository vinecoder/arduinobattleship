#include <MaxMatrix.h>
#include <avr/pgmspace.h>

const int data = 4;
const int load = 5;
const int clock = 6;
const int btnEsquerda = 8;
const int btnDireita = 9;
const int btnEnter = 10;
const int maxInUse = 2;
int tamanhoNavio[7] = {1,1,2,2,3,4,5};
char *msg[7] = {"Digite as coordendadas do Hidroaviao 1",
                            "Digite as coordendadas do Hidroaviao 2",
                            "Digite as coordendadas do Submarino 1",
                            "Digite as coordendadas do Submarino 2",
                            "Digite as coordendadas do Cruzadores",
                            "Digite as coordendadas do Encouracados",
                            "Digite as coordendadas do Porta-avioes"};
MaxMatrix m(data, load, clock, maxInUse);

int tabuleiroA[][8] =  {{0,0,0,0,0,0,0,0},
                       {0,0,0,0,0,0,0,0},
                       {0,0,0,0,0,0,0,0},
                       {0,0,0,0,0,0,0,0},
                       {0,0,0,0,0,0,0,0},
                       {0,0,0,0,0,0,0,0},
                       {0,0,0,0,0,0,0,0},
                       {0,0,0,0,0,0,0,0}}; 
int tabuleiroB[][8] =  {{0,0,0,0,0,0,0,0},
                       {0,0,0,0,0,0,0,0},
                       {0,0,0,0,0,0,0,0},
                       {0,0,0,0,0,0,0,0},
                       {0,0,0,0,0,0,0,0},
                       {0,0,0,0,0,0,0,0}, 
                       {0,0,0,0,0,0,0,0},
                       {0,0,0,0,0,0,0,0}}; 

int countMarcacao=0;
int passo = 0;
int nav_x1 = 0;
int nav_y1 = 0;
int nav_x2 = 2;
int nav_y2 = 0;

void setup()
{
  Serial.begin(9600);
  pinMode(btnEsquerda, INPUT);     
  pinMode(btnDireita, INPUT);     
  pinMode(btnEnter, INPUT);     
  m.init();
  m.setIntensity(1);
  liga(nav_x1,nav_y1);
  liga(nav_x2,nav_y2);
}

int MAX(int a, int b){
  return ((a >= b) ? a : b);
}


int MIN(int a, int b){
  return ((a <= b) ? a : b);
}



//Funcoes Utilitarias

void escreveLCD(char *msg){
      Serial.println(msg);   
}
/**
* Funcao que liga um ponto
*/
void liga(int x, int y){
    m.setDot(x,y,HIGH);
}
/**
* Funcao que desliga um ponto
*/
void desliga(int x, int y){
    m.setDot(x,y,LOW);
}

/**
* Funcao que pisca dois pontos
*/
void pisque(int x1,int y1,int x2,int y2){
int count = 0;
  do{
        liga(x1,y1);
        liga(x2,y2);
        delay(50); 
        desliga(x1,y1);
        desliga(x2,y2);
        delay(50); 
  }while(count++ < 2);
  
        liga(x1,y1);
        liga(x2,y2);
  delay(50); 

}
/**
* Funcao que pisca um ponto
*/
void pisque(int x1,int y1){
int count = 0;
  do{
        liga(x1,y1);
        delay(50); 
        desliga(x1,y1);
        delay(50); 
  }while(count++ < 2);
  
  liga(x1,y1);
  delay(50); 

}



void marcacaoInicial(){
 if (digitalRead(btnDireita) == HIGH || 
     digitalRead(btnEsquerda) == HIGH || 
     digitalRead(btnEnter) == HIGH){
      if (digitalRead(btnDireita) == HIGH) {  
        if (nav_x1 != nav_x2 || nav_y1 != nav_y2){
            desliga(nav_x1,nav_y1);
        }
        nav_x1++;
        if (nav_x1 > 7){
          nav_x1 = 0;
          nav_y1++;
        }
        if (nav_y1 > 7){
          nav_y1 = 0;
        }
     
        if (nav_x1 != nav_x2 || nav_y1 != nav_y2){
          liga(nav_x1,nav_y1);
        }
      }
      
     if (digitalRead(btnEsquerda) == HIGH) {  
        
        if (nav_x1 != nav_x2 || nav_y1 != nav_y2){
          desliga(nav_x2,nav_y2);   
        }
        
        nav_x2++;
        if (nav_x2 > 7){
          nav_x2 = 0;
          nav_y2++;
        }
        if (nav_y2 > 7){
          nav_y2 = 0;
        };
        
        if (nav_x1 != nav_x2 || nav_y1 != nav_y2){
          liga(nav_x2,nav_y2);
        }
      }
      
        Serial.print("(x1,y1),(x2,y2): ");
        Serial.print(nav_x1,DEC);
        Serial.print(","); 
        Serial.print(nav_y1,DEC);
        Serial.print(","); 
        Serial.print(nav_x2,DEC);
        Serial.print(","); 
        Serial.println(nav_y2,DEC);
   
      
      if (digitalRead(btnEnter) == HIGH) {     
        // adicionar nas marcacoes
        passo++; 
        pisque(nav_x1,nav_y1,nav_x2,nav_y2);
        if (nav_x1 == nav_x2){
          for (int i = MIN(nav_y1,nav_y2);i<=MAX(nav_y1,nav_y2);i++){
            tabuleiroB[i][nav_x1] = 1;
          }
        }else if (nav_y1 == nav_y2){
          for (int i = MIN(nav_x1,nav_x2);i<=MAX(nav_x1,nav_x2);i++){
            tabuleiroB[nav_y1][i] = 1;
          }
        }else{
          ///alertar posicao invalida;
        }
        
        printTabuleiroB();
      }
        delay(100);
     }
}

void printTabuleiroB(){
  for (int y=0;y<8;y++){
    for (int x=0;x<8;x++){
        m.setDot(x+8,y,tabuleiroB[y][x]);
        Serial.print(tabuleiroB[y][x]);
     
    }
    Serial.println("");
  
  }

}

void loop()
{  
  if (passo <7) {   
    escreveLCD(msg[passo]);
    marcacaoInicial();
  } else if (passo == 7) {
    //jogar();
  } else {
     //imprimirResultado();
  }
}
