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
const char *msg[7] = {"Digite as coordendadas do Hidroaviao 1",
                            "Digite as coordendadas do Hidroaviao 2",
                            "Digite as coordendadas do Submarino 1",
                            "Digite as coordendadas do Submarino 2",
                            "Digite as coordendadas do Cruzadores",
                            "Digite as coordendadas do Encouracados",
                            "Digite as coordendadas do Porta-avioes"}
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

int marcacao[][4];
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

//Funcoes Utilitarias

void escreveLCD(*char msg){
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
    delay(100); 
    desliga(x1,y1);
    desliga(x2,y2);
    delay(100); 
  }while(count++ < 2);
    liga(x1,y1);
    liga(x2,y2);
    delay(100); 
}
/**
* Funcao que pisca um ponto
*/
void pisque(int x1,int y1){
  int count = 0;
  do{
        liga(x1,y1);
        delay(100); 
        desliga(x1,y1);
        delay(100); 
  }while(count++ < 2);
  liga(x1,y1);
  delay(100);
}
/**
* Funcao que navega pelo eixo X
*/
void incrementaX(int *x){

    if (nav_x1 != nav_x2 || nav_y1 != nav_y2){
        desliga(nav_x1,nav_y1);
    }
    *x++;
    if (*x > 7){
      *x = 0;
      nav_y1++;
    }
    if (nav_y1 > 7){
      nav_y1 = 0;
    }
 
    if (nav_x1 != nav_x2 || nav_y1 != nav_y2){
      liga(nav_x1,nav_y1);
    }
}


/***
* Funcao que checa as posicoes:
* 1 - Se a marcacao esta em uma posicao invalida (diagonoal, colada)
  2 - Se a posicao ja esta ocupada
  3 - Se a posicao nao corresponde ao tamanho do navio
***/
boolean checkAndMark(){
  boolean invalido = false;
  int x1,x2,y1,y2;
  boolean horizontal;

 if (nav_x1 == nav_x2){
    x1 = &nav_x1;
    x2= &nav_x2;
    y1 = &nav_y1;
    y2= &nav_y2;
    horizontal = false;
  }else if (nav_y1 == nav_y2){
    x1 = &nav_y1;
    x2= &nav_y2;
    y1 = &nav_x1;
    y2= &nav_x2;
    horizontal = true;
  }else{
       Serial.println("Posicao invalida.");
       return false;
  }

    if (tamanhoNavio[passo] != (y2-y1)+1){
      invalido = true;
      Serial.println("Tamanho do navio invalido.");
      return false;
    }

    for (int i = y1-1;i<=y2+1;i++){
      
      if (tabuleiroB[i][x1-1] == 1 
          || tabuleiroB[i][x1] == 1 
          || tabuleiroB[i][x1+1] == 1 ||){
        invalido = true;
        Serial.println("Posicao muito proxima a outra embarcaçao.");
        return false;
      }
    }
    //VALIDO
    if (invalido == false){
      marcacao[countMarcacao++] = {x1,y1,x2,y2};
      for (int i = y1;i<=y2;i++){
        if (horizontal == true){
          tabuleiroB[i][x1] = 1;
        }else{
          tabuleiroB[x1][i] = 1;
        }
      }
        passo++; 
    }
  }

}

/**
* Imprime as selecoes antes de marcar-las
*/
void printSelecoes(){
        Serial.print("(x1,y1),(x2,y2): ");
        Serial.print(nav_x1,DEC);
        Serial.print(","); 
        Serial.print(nav_y1,DEC);
        Serial.print(","); 
        Serial.print(nav_x2,DEC);
        Serial.print(","); 
        Serial.println(nav_y2,DEC);
}
/**
* Marca o tabuleiroB de acordo com a matrix.
**/
void printTabuleiroB(){
  for (int y=0;y<8;y++){
    for (int x=0;x<8;x++){
        m.setDot(x+8,y,tabuleiroB[y][x]);
    }
    Serial.println("");
  }
}


/***
* Funcao que faz toda marcacao inicial
**/
void marcacaoInicial(){
 if (digitalRead(btnDireita) == HIGH || 
     digitalRead(btnEsquerda) == HIGH || 
     digitalRead(btnEnter) == HIGH){

    if (digitalRead(btnDireita) == HIGH) {  
      incrementaX(&nav_x1);
      printSelecoes();
    }else if (digitalRead(btnEsquerda) == HIGH) {  
      incrementaX(&nav_x2);
      printSelecoes();
    }else   
    if (digitalRead(btnEnter) == HIGH) {     
      pisque(nav_x1,nav_y1,nav_x2,nav_y2);
      checkAndMark();
      printTabuleiroB();
    }
 }
}



