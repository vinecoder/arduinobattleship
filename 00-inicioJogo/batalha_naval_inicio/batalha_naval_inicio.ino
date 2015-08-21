#include <MaxMatrix.h>
#include <avr/pgmspace.h>

int data = 4;
int load = 5;
int clock = 6;
int maxInUse = 2;    //change this variable to set how many MAX7219's you'll use
MaxMatrix m(data, load, clock, maxInUse);
byte buffer[10];
char msg[] = "";
int tabuleiroA[][8] =  {{0,0,0,0,0,0,0,0},
                    {0,0,0,0,0,0,0,0},
                    {0,0,0,0,0,0,0,0},
                    {0,0,0,0,0,0,0,0},
                    {0,0,0,0,0,0,0,0},
                    {0,0,0,0,0,0,0,0},
                    {0,0,0,0,0,0,0,0},
                    {0,0,0,0,0,0,0,0}} ; 
int tabuleiroB[][8] =  {{0,0,0,0,0,0,0,0},
                    {0,0,0,0,0,0,0,0},
                    {0,0,0,0,0,0,0,0},
                    {0,0,0,0,0,0,0,0},
                    {0,0,0,0,0,0,0,0},
                    {0,0,0,0,0,0,0,0}, 
                    {0,0,0,0,0,0,0,0},
                    {0,0,0,0,0,0,0,0}} ; 

int marcacao[][4] = {{0,0,0,3},//0,0  0,1 0,2 0,3
                       {0,0,0,3},
                       {0,0,0,3},
                       {0,0,0,3}};
                       
int passo = 0;

const int btnEsquerda = 8;
const int btnDireita = 9;
const int btnEnter = 10;
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
}
/*
void escrever(int[2] origem, int[2] destino){
  for (origem ate destino ){
    m.setDot(orgem,destino);
  
  }
 
} */




void resetTabuleiroA(){
  for (int linha=0;linha<8;linha++){
    for (int coluna=0;coluna<8;coluna++){
        for (int i=0;i<sizeof(marcacao);i++){
          // x1 <= linha <= x2
           int x1 = marcacao[i][0]; 
           int y1 = marcacao[i][1];
           int x2 = marcacao[i][2];
           int y2 = marcacao[i][3];
           
           if (x1 != linha && x2 != linha){
             tabuleiroA[linha][coluna] = LOW;
           }
          
        }
      
    }
  }

}

void pisque(int x1,int y1,int x2,int y2){
int count = 0;
  do{
        liga(x1,y1);
        liga(x2,y2);
        delay(500); 
        desliga(x1,y1);
        desliga(x2,y2);
        delay(500); 
  }while(count++ < 2);
  
        liga(x1,y1);
        liga(x2,y2);
  delay(500); 

}


void pisque(int x1,int y1){
int count = 0;
  do{
        liga(x1,y1);
        delay(500); 
        desliga(x1,y1);
        delay(500); 
  }while(count++ < 2);
  
  liga(x1,y1);
  delay(500); 

}

void liga(int x, int y){
    m.setDot(x,y,HIGH);
}
void desliga(int x, int y){
    m.setDot(x,y,LOW);
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
          for (int i = nav_y1;i<=nav_y2;i++){
            tabuleiroB[i][nav_x1] = 1;
          }
        }else if (nav_y1 == nav_y2){
          for (int i = nav_x1;i<=nav_x2;i++){
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
     
    }
    Serial.println("");
  
  }

}

void loop()
{  
  if (passo <8) {   
    marcacaoInicial();
  } else if (passo ==8) {
    //jogar();
  } else {
     //imprimirResultado();
  }
}
