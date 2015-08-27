#include <MaxMatrix.h>
#include <avr/pgmspace.h>

const int data = 4;
const int load = 5;
const int clock = 6;
const int btnEsquerda = 8;
const int btnDireita = 7;//9
const int btnEnter = 10;
const int receptor = 11;
const int emissor = 9;//3
const int emissorLed = 12;//3
const int buzina = 2;
const int maxInUse = 2;
int arrTiro[] = {0,1};
int tamanhoNavio[7] = {1,1,2,2,3,4,5};
char *msg[99] = {"Digite as coordendadas do Hidroaviao 1 (1)",
                            "Digite as coordendadas do Hidroaviao 2 (1)",
                            "Digite as coordendadas do Submarino 1 (2)",
                            "Digite as coordendadas do Submarino 2 (2)",
                            "Digite as coordendadas do Cruzador (3)",
                            "Digite as coordendadas do Encouracado (4)",
                            "Digite as coordendadas do Porta-aviao (5)",
                            "Aguardando ...",
                            "Aguardando Jogada do adversario",
                            "Envie o tiro",
                            "Fim de Jogo!"};

MaxMatrix m(data, load, clock, maxInUse);
                        //0,1,2,3,4,5,6,7
int tabuleiroA[][8] =  {{0,0,0,0,0,0,0,0},
                       {0,0,0,0,0,0,0,0},
                       {0,0,0,0,0,0,0,0},
                       {0,0,0,0,0,0,0,0},
                       {0,0,0,0,0,0,0,0},
                       {0,0,0,0,0,0,0,0},
                       {0,0,0,0,0,0,0,0},
                       {0,0,0,0,0,0,0,0}}; 

int passo = 0; //TEM QUE SER ZERADO
int tabuleiroB[][8] ={//0,1,2,3,4,5,6,7  // TEM QUE SER ZERADO
                       {0,0,0,0,0,0,0,0},//0
                       {0,0,0,0,0,0,0,0},//1
                       {0,0,0,0,0,0,0,0},//2
                       {0,0,0,0,0,0,0,0},//3
                       {0,0,0,0,0,0,0,0},//4
                       {0,0,0,0,0,0,0,0},//5
                       {0,0,0,0,0,0,0,0},//6
                       {0,0,0,0,0,0,0,0}}; //7

char *tabuleiroC[][8] ={//0,1,2,3,4,5,6,7  // TEM QUE SER ZERADO
                       {"F","F","F","F","F","F","F","F"},//"F"
                       {"F","F","F","F","F","F","F","F"},//1
                       {"F","F","F","F","F","F","F","F"},//2
                       {"F","F","F","F","F","F","F","F"},//3
                       {"F","F","F","F","F","F","F","F"},//4
                       {"F","F","F","F","F","F","F","F"}, //5
                       {"F","F","F","F","F","F","F","F"},//6
                       {"F","F","F","F","F","F","F","F"}}; //7
/*
int passo = 7;

int tabuleiroB[][8] =
{ 
{1,0,1,0,1,0,1,0},
{0,0,0,0,1,0,1,0},
{1,0,0,0,0,0,0,0},
{1,0,0,0,0,0,0,0},
{1,0,0,1,1,1,1,0},
{0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0},
{1,1,1,1,1,0,0,0}};

char *tabuleiroC[][8] = 
{ {"A","F","A","F","B","F","B","F"},
  {"F","F","F","F","B","F","B","F"},
  {"C","F","F","F","F","F","F","F"},
  {"C","F","F","F","F","F","F","F"},
  {"C","F","F","D","D","D","D","F"},
  {"F","F","F","F","F","F","F","F"},
  {"F","F","F","F","F","F","F","F"},
  {"E","E","E","E","E","F","F","F"}};
*/

char *navios_tipo[7] = {"A","A","B","B","C","D","E"};
char *agua_tipo = "F";
String protocolo;

int nav_x1 = 5;
int nav_y1 = 2;
int nav_x2 = 5;
int nav_y2 = 2;
boolean debugMode = false;
int navioRestante = 18;

void initTeste(){

}

void setup()
{
  Serial.begin(9600);
  pinMode(btnEsquerda, INPUT);     
  pinMode(btnDireita, INPUT);     
  pinMode(btnEnter, INPUT);  
  pinMode(emissorLed,OUTPUT);
  pinMode(buzina,OUTPUT);
  pinMode(receptor,INPUT);   
  m.init();
  m.setIntensity(1);
  setDot(nav_x1,nav_y1,HIGH);
  setDot(nav_x2,nav_y2,HIGH);
}

/**
*Funcao que retorna o valor maximo entre dois paramentros
*/
int MAX(int a, int b){
  return ((a >= b) ? a : b);
}

/**
*Funcao que retorna o valor minimo entre dois paramentros
*/
int MIN(int a, int b){
  return ((a <= b) ? a : b);
}

/**
*Funcao que escreve no LCD
*/
void escreveLCD(char *msg){
      Serial.println(msg);   
}
/**
* Funcao que liga/desliga um ponto
*/
void setDot(int x, int y, int status){
    m.setDot(x,y,status);
}


/**
* Funcao que liga/desliga uma coluna
*/
void setColumn(int x,int ynot, int status){
    for (int y=0;y<8;y++){
        if (y != ynot){
          m.setDot(x,y,status);
        }
        delay(20);
    }
}
/**
* Funcao que liga/desliga uma linha
*/
void setRow(int y,int xnot, int status){
    for (int x=0;x<8;x++){
        if (x != xnot){
          m.setDot(x,y,status);
        }
        delay(20);
    }
}
/**
* Funcao que pisca uma cruz
*/
void pisqueCross(int x1,int y1){
int count = 0;
  do{
    setColumn(x1,y1,HIGH);
    setRow(y1,x1,HIGH);
    delay(50); 
    setColumn(x1,y1,LOW);
    setRow(y1,x1,LOW);
    delay(50); 
  }while(count++ < 2);
    setColumn(x1,y1,HIGH);
    setRow(y1,x1,HIGH);
    delay(50); 
}
/**
* Funcao que pisca dois pontos
*/
void pisque(int x1,int y1,int x2,int y2){
int count = 0;
  do{
    setDot(x1,y1,HIGH);
    setDot(x2,y2,HIGH);
    delay(50); 
    setDot(x1,y1,LOW);
    setDot(x2,y2,LOW);
    delay(50); 
  }while(count++ < 2);
    setDot(x1,y1,HIGH);
    setDot(x2,y2,HIGH);
    delay(50); 

}
/**
* Funcao que pisca um ponto
*/
void pisque(int x1,int y1){
int count = 0;
  do{
    setDot(x1,y1,HIGH);
    delay(50); 
    setDot(x1,y1,LOW);
    delay(50); 
  }while(count++ < 2);
    setDot(x1,y1,HIGH);
    delay(50); 
}
/**
* Funcao que navega pelo eixo X
*/
void incrementaX(int *x1,int *y1,int x2, int y2){
    if (*x1 != x2 || *y1 != y2){
        setDot(*x1,*y1,LOW);
    }

        *x1 = *x1 + 1;
        if (*x1 > 7){
          *x1 = 0;
          *y1 = *y1 + 1;
        }
        if (*y1 > 7){
          *y1 = 0;
        }

        delay(100);

   
    if (*x1 != x2 || *y1 != y2){
      setDot(*x1,*y1,HIGH);
    }
}

/**
* Funcao que navega pelo eixo X
*/
void incrementaColuna(int *x1, int ynot){
     setColumn(*x1,ynot, LOW);
      *x1 = *x1 + 1;
      if (*x1 > 7){
        *x1 = 0;
      }
      setColumn(*x1,ynot,HIGH);
}

/**
* Funcao que navega pelo eixo X
*/
void incrementaLinha(int *y1, int xnot){
      setRow(*y1,xnot,LOW);
      *y1 = *y1 + 1;
      if (*y1 > 7){
        *y1 = 0;
      }
      setRow(*y1,xnot,HIGH);
}

/**
* Marca o tabuleiroB de acordo com a matrix.
**/
void printTabuleiroB(){
  for (int y=0;y<8;y++){
    for (int x=0;x<8;x++){
        m.setDot(x+8,y,tabuleiroB[y][x]);
  if (debugMode == true){
        Serial.print(tabuleiroB[y][x]);
        Serial.print(tabuleiroC[y][x]);
      }
     
    }
  if (debugMode == true){
    Serial.println("");
  }
  }
}
/**
* Imprime as selecoes antes de marcar-las
*/
void printSelecoes(){
  if (debugMode == true){
        Serial.print("(x1,y1),(x2,y2): ");
        Serial.print(nav_x1,DEC);
        Serial.print(","); 
        Serial.print(nav_y1,DEC);
        Serial.print(","); 
        Serial.print(nav_x2,DEC);
        Serial.print(","); 
        Serial.println(nav_y2,DEC);
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
        incrementaX(&nav_x1, &nav_y1, nav_x2, nav_y2);
        printSelecoes();
      }      
      if (digitalRead(btnEsquerda) == HIGH) {  
        incrementaX(&nav_x2, &nav_y2, nav_x1, nav_y1);
        printSelecoes();
      }
      if (digitalRead(btnEnter) == HIGH) {     
        pisque(nav_x1,nav_y1,nav_x2,nav_y2);
        checkAndMark();
        printTabuleiroB();
      }
     }
}


/***
* Funcao que checa as posicoes:
* 1 - Se a marcacao esta em uma posicao invalida (diagonoal, colada)
  2 - Se a posicao ja esta ocupada
  3 - Se a posicao nao corresponde ao tamanho do navio
***/
void checkAndMark(){  
         if (nav_x1 == nav_x2 &&  nav_y1 == nav_y2){

          if (tamanhoNavio[passo] != (MAX(nav_y1,nav_y2)-MIN(nav_y1,nav_y2))+1){
            Serial.println("Tamanho do navio invalido.");
            return;
          }

          //verifica os vizinho em cima e em baixo
          for (int i = MIN(nav_y1,nav_y2)-1;i<=MAX(nav_y1,nav_y2)+1;i++){
              //Verificacao no mesmo eixo
            if (tabuleiroB[i][nav_x1] == 1){
              Serial.println("Posicao muito proxima a outra embarcaçao.");
              return;
            }
          }

          //verifica os vizinho em esquerda para direita
          for (int i = MIN(nav_x1,nav_x2)-1;i<=MAX(nav_x1,nav_x2)+1;i++){

           //Verificacao no mesmo eixo
            if ( tabuleiroB[nav_y1][i] == 1 ){
              Serial.println("Posicao muito proxima a outra embarcaçao.");
              return;
            }
          }

            for (int i = MIN(nav_y1,nav_y2);i<=MAX(nav_y1,nav_y2);i++){
              tabuleiroB[i][nav_x1] = 1;
              tabuleiroC[i][nav_x1] = navios_tipo[passo];
            } 
  
          passo++; 
          escreveLCD(msg[passo]);
         }else  if (nav_x1 == nav_x2){

         if (tamanhoNavio[passo] != (MAX(nav_y1,nav_y2)-MIN(nav_y1,nav_y2))+1){
            Serial.println("Tamanho do navio invalido.");
            return;
          }

          //verifica os vizinho em cima e em baixo
          for (int i = MIN(nav_y1,nav_y2)-1;i<=MAX(nav_y1,nav_y2)+1;i++){
              //Verificacao no mesmo eixo
            if ( tabuleiroB[i][nav_x1] == 1 ){
              Serial.println("Posicao muito proxima a outra embarcaçao.");
              return;
            }
            //Verificacao dos extremos
            if (i != MIN(nav_y1,nav_y2)-1 && i != MAX(nav_y1,nav_y2)+1){
              int extremoMenor = nav_x1-1 <= 7 && nav_x1-1 >=0 ?nav_x1-1 : nav_x1; 
              int extremoMaior = nav_x1+1 <= 7 && nav_x1+1 >=0 ?nav_x1+1 : nav_x1; 
              if ( tabuleiroB[extremoMenor][i] == 1 || tabuleiroB[extremoMaior][i] == 1 ){
                Serial.println("Posicao muito proxima a outra embarcaçao.");
                return;
              }
            }
        
          }

          for (int i = MIN(nav_y1,nav_y2);i<=MAX(nav_y1,nav_y2);i++){
            tabuleiroB[i][nav_x1] = 1;
            tabuleiroC[i][nav_x1] = navios_tipo[passo];
          } 
  
          passo++; 
          escreveLCD(msg[passo]);
        }else if (nav_y1 == nav_y2){
          
         if (tamanhoNavio[passo] != (MAX(nav_x1,nav_x2)-MIN(nav_x1,nav_x2))+1){
            Serial.println("Tamanho do navio invalido.");
            return;
          }
          //verifica os vizinho em esquerda para direita
          for (int i = MIN(nav_x1,nav_x2)-1;i<=MAX(nav_x1,nav_x2)+1;i++){

           //Verificacao no mesmo eixo
            if ( tabuleiroB[nav_y1][i] == 1 ){
              Serial.println("Posicao muito proxima a outra embarcaçao.");
              return;
            }
            //Verificacao dos extremos
            if (i != MIN(nav_x1,nav_x2)-1 && i != MAX(nav_x1,nav_x2)+1){
              int extremoMenor = nav_y1-1 <= 7 && nav_y1-1 >=0 ?nav_y1-1 : nav_y1; 
              int extremoMaior = nav_y1+1 <= 7 && nav_y1+1 >=0 ?nav_y1+1 : nav_y1; 
            
              if ( tabuleiroB[extremoMenor][i] == 1 || tabuleiroB[extremoMaior][i] == 1 ){
                Serial.println("Posicao muito proxima a outra embarcaçao.");
                return;
              }
            }
        
          }
          
          for (int i = MIN(nav_x1,nav_x2);i<=MAX(nav_x1,nav_x2);i++){
            tabuleiroB[nav_y1][i] = 1;
            tabuleiroC[nav_y1][i] = navios_tipo[passo];
          }


          passo++; 
          escreveLCD(msg[passo]);
        }else{
         Serial.println("Posicao invalida.");
         return;
        }
  }

///////////////////////// JOGO


void marcacaoJogada(){
 if (digitalRead(btnDireita) == HIGH || 
     digitalRead(btnEsquerda) == HIGH || 
     digitalRead(btnEnter) == HIGH){
      if (digitalRead(btnDireita) == HIGH) {  
        incrementaColuna(&nav_x1,nav_y1);  
        printSelecoes();
      }      
      if (digitalRead(btnEsquerda) == HIGH) {  
        incrementaLinha(&nav_y1,nav_x1);  
        printSelecoes();
      }
      if (digitalRead(btnEnter) == HIGH) {     
        pisque(nav_x1,nav_y1);
        marcaJogada();
        printTabuleiroB();
      }
     }
}

//Função checa se o tiro acertou algum navio
void marcaJogada() {
  String result;
  protocolo = "";
  Serial.println("");
  Serial.println("Tiro recebido " + String(nav_x1)+String(",")+String(nav_y1));
//Percorre o tabuleiroB e verifica se a coord. esta ligada
    for (int x=0; x<=7; x++) {
      for(int y=0; y<=7; y++) {
        //Se a coordenada estiver ligada, verifica se o tiro acertou algum navio
        if ((x == nav_x1) && (y == nav_y1)) {
          //Se o tiro acertou - Mostra msg "Acertou", senão, mostra "Agua"
          if (tabuleiroB[y][x] == 1 ) {
            tabuleiroB[y][x] = 0; 
            navioRestante--;

            String nome = tabuleiroC[y][x] == "A" ? "Hidroaviao":
                          tabuleiroC[y][x] == "B" ? "Submarino":
                          tabuleiroC[y][x] == "C" ? "Cruzador":
                          tabuleiroC[y][x] == "D" ? "Encouracado":
                          tabuleiroC[y][x] == "E" ? "Porta-aviao": "Agua";
            result = "Acertou " + nome;

            protocolo = String(nav_x1) + String(nav_y1) + String(tabuleiroC[y][x]);
            buzinarAcerto();
            sendProtocolo();
            Serial.println(result);
          //Substituir por som do Buzzer
          } else {
            tabuleiroB[y][x] = 0;
            buzinarErro();
            protocolo = String(nav_x1) + String(nav_y1) + String(agua_tipo);
            sendProtocolo();
            Serial.println("Agua ");
          }
        }
      }
    }
}

void buzinarErro(){
    digitalWrite(buzina,HIGH);
    delay(2000);
    digitalWrite(buzina,LOW);
}

void buzinarAcerto(){
    digitalWrite(buzina,HIGH);
    delay(500);
    digitalWrite(buzina,LOW);
    delay(500);
    digitalWrite(buzina,HIGH);
    delay(1000);
    digitalWrite(buzina,LOW);
    delay(500);
}


void sendProtocolo(){
      digitalWrite(emissorLed,HIGH);
      delay(200);
      digitalWrite(emissorLed,LOW);
      delay(200);
      digitalWrite(emissorLed,HIGH);
      delay(200);
      digitalWrite(emissorLed,LOW);
      delay(200);
      //enviar protocolo via ir
      Serial.print("Enviando protocolo via ir: ");
      Serial.println(protocolo);
}


void loop()
{  
  if (passo <7) {   
    escreveLCD(msg[passo]);

    while(passo <7){
       marcacaoInicial();
       delay(100);
    }
  } else if (passo == 7) {
      
    escreveLCD(msg[passo]);
      //Inicializa
      setColumn(nav_x1,nav_y1,HIGH);
      setRow(nav_y1,nav_x1,HIGH);
      printTabuleiroB();
     
     while (passo == 7){
      marcacaoJogada();
      if (navioRestante == 0){
        passo = 10;
      }
       delay(100);
    }
  } else if (passo == 10){ 
    escreveLCD(msg[passo]);
         // Aguardando jogador.
      while (digitalRead(btnDireita) == LOW &&
             digitalRead(btnEsquerda) == LOW &&
             digitalRead(btnEnter) == LOW){

      }
  }else{
    Serial.println("Fim de Jogo!");
        // Aguardando jogador.
      while (digitalRead(btnDireita) == LOW &&
             digitalRead(btnEsquerda) == LOW &&
             digitalRead(btnEnter) == LOW){

      }
  }
}