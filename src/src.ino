#include <MaxMatrix.h>
#include <avr/pgmspace.h>

const int data = 4;
const int load = 5;
const int clock = 6;
const int btnEsquerda = 8;
const int btnDireita = 9;
const int btnEnter = 10;
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
                            "Aguardando Adversario...",
                            "Aguardando Jogada do adversario",
                            "Envie o tiro",
                            "Fim de Jogo! Voce Perdeu!!"};

MaxMatrix m(data, load, clock, maxInUse);

int tabuleiroA[][8] =  {{0,0,0,0,0,0,0,0},
                       {0,0,0,0,0,0,0,0},
                       {0,0,0,0,0,0,0,0},
                       {0,0,0,0,0,0,0,0},
                       {0,0,0,0,0,0,0,0},
                       {0,0,0,0,0,0,0,0},
                       {0,0,0,0,0,0,0,0},
                       {0,0,0,0,0,0,0,0}}; 
int tabuleiroB[][8] =  {
                       {1,0,1,0,1,0,1,0},
                       {0,0,0,0,1,0,1,0},
                       {1,0,0,0,0,0,0,0},
                       {1,0,0,0,0,0,0,0},
                       {1,0,0,1,1,1,1,0},
                       {0,0,0,0,0,0,0,0}, 
                       {0,0,0,0,0,0,0,0},
                       {1,1,1,1,1,0,0,0}}; 

char *navios[7] = {"Hidroaviao"," Hidroaviao"," Submarino ", "Submarino"," Cruzador"," Encouracado"," Porta-aviao"};
char *navios_tipo[7] = {"A"," A"," B ", "B"," C"," D","E"};
char *protocolo;
int marcacao[6][4];
int countMarcacao=0;
int passo = 7;
int nav_x1 = 0;
int nav_y1 = 0;
int nav_x2 = 0;
int nav_y2 = 0;
boolean debugMode = false;
int navioRestante = 18;

void setup()
{
  Serial.begin(9600);
  pinMode(btnEsquerda, INPUT);     
  pinMode(btnDireita, INPUT);     
  pinMode(btnEnter, INPUT);     
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
void setColumn(int x, int status){
    for (int y=0;y<8;y++){
        m.setDot(x,y,status);
        delay(20);
    }
}
/**
* Funcao que liga/desliga uma linha
*/
void setRow(int y, int status){
    for (int x=0;x<8;x++){
        m.setDot(x,y,status);
        delay(20);
    }
}
/**
* Funcao que pisca uma cruz
*/
void pisqueCross(int x1,int y1){
int count = 0;
  do{
    setColumn(x1,HIGH);
    setRow(y1,HIGH);
    delay(50); 
    setColumn(x1,LOW);
    setRow(y1,LOW);
    delay(50); 
  }while(count++ < 2);
    setColumn(x1,HIGH);
    setRow(y1,HIGH);
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
void incrementaColuna(int *x1){
     setColumn(*x1,LOW);
     *x1 = *x1 > 7 ? 0 : *x1 + 1;
     setColumn(*x1,HIGH);
}

/**
* Funcao que navega pelo eixo X
*/
void incrementaLinha(int *y1){
     setRow(*y1,LOW);
     *y1 = *y1 > 7 ? 0 : *y1 + 1;
     setRow(*y1,HIGH);
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
        if (nav_x1 == nav_x2){

         if (tamanhoNavio[passo] != (MAX(nav_y1,nav_y2)-MIN(nav_y1,nav_y2))+1){
            Serial.println("Tamanho do navio invalido.");
            return;
          }
            //verifica os vizinho em cima e em baixo
            for (int i = MIN(nav_y1,nav_y2)-1;i<=MAX(nav_y1,nav_y2)+1;i++){
            //verifica os vizinho da direita e da esquerda e o propio
              if (
                     tabuleiroB[i][nav_x1-1] == 1 
                  || tabuleiroB[i][nav_x1] == 1 
                  || tabuleiroB[i][nav_x1+1] == 1){
                Serial.println("Posicao muito proxima a outra embarcaçao.");
                return;
              }
            }

          for (int i = MIN(nav_y1,nav_y2);i<=MAX(nav_y1,nav_y2);i++){
            tabuleiroB[i][nav_x1] = 1;
          } 

          marcacao[countMarcacao][0] = nav_x1;
          marcacao[countMarcacao][1] = nav_y1;
          marcacao[countMarcacao][2] = nav_x2;
          marcacao[countMarcacao][3] = nav_y2;
          countMarcacao++;

          passo++; 
        }else if (nav_y1 == nav_y2){
          
         if (tamanhoNavio[passo] != (MAX(nav_x1,nav_x2)-MIN(nav_x1,nav_x2))+1){
            Serial.println("Tamanho do navio invalido.");
            return;
          }

          //verifica os vizinho em cima e em baixo
          for (int i = MIN(nav_x1,nav_x2)-1;i<=MAX(nav_x1,nav_x2)+1;i++){
          //verifica os vizinho da direita e da esquerda e o propio
            //TODO verficiar as extremidades.
            if (
                   tabuleiroB[i][nav_y1-1] == 1 
                || tabuleiroB[i][nav_y1] == 1 
                || tabuleiroB[i][nav_y1+1] == 1){
              Serial.println("Posicao muito proxima a outra embarcaçao.");
              return;
            }
          }

          
          for (int i = MIN(nav_x1,nav_x2);i<=MAX(nav_x1,nav_x2);i++){
            tabuleiroB[nav_y1][i] = 1;
          }

          marcacao[countMarcacao][0] = nav_x1;
          marcacao[countMarcacao][1] = nav_y1;
          marcacao[countMarcacao][2] = nav_x2;
          marcacao[countMarcacao][3] = nav_y2;
          countMarcacao++;

          passo++; 
          
        }else{
         Serial.println("Posicao invalida.");
         return;
        }
  }

void recebeTiro(){
  passo++;

}
///////////////////////// JOGO
void jogar(){
  if (passo == 7){
    recebeTiro();
  }
  marcacaoJogada();

}

void marcacaoJogada(){
 if (digitalRead(btnDireita) == HIGH || 
     digitalRead(btnEsquerda) == HIGH || 
     digitalRead(btnEnter) == HIGH){
      if (digitalRead(btnDireita) == HIGH) {  
        incrementaColuna(&nav_x1);  
        printSelecoes();
      }      
      if (digitalRead(btnEsquerda) == HIGH) {  
        incrementaLinha(&nav_y1);  
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
String marcaJogada() {
  char* result;
//Percorre o tabuleiroB e verifica se a coord. esta ligada
    for (int x=0; x<=7; x++) {
      for(int y=0; y<=7; y++) {
        Serial.print(',');
        //Se a coordenada estiver ligada, verifica se o tiro acertou algum navio
        if ((x == nav_x1) && (y == nav_y1)) {
//          //Se o tiro acertou - Mostra msg "Acertou", senão, mostra "Agua"
          if (tabuleiroB[y][x] == 1 ) {
            tabuleiroB[y][x] = 0; 
            navioRestante--;
            for (int i=0; i<6; i++){
              int x1 = navios[i][0];
              int y1 = navios[i][1];
              int x2 = navios[i][2];
              int y2 = navios[i][3];
              result = " = Acertou";

              if ( nav_x1 >= MIN(x1,x2) && nav_x2 <= MAX(x1,x2) 
                  && nav_y1 >= MIN(y1,y2) && nav_y2 <= MAX(y1,y2))
              {
                  protocolo = nav_x1 + nav_y1 + navios_tipo[i];
                  result = result + "" +  navios[i];
              }
            }
            //Substituir por som do Buzzer
           
          } else {
            tabuleiroB[y][x] = 0;
            //Substituir por som do Buzzer
            result = " = Agua";
            protocolo = nav_x1 + nav_y1 + "X";
          }
        }
      }
      sendProtocolo();
      Serial.println( result );
      result = "";
    }
    Serial.println();
    Serial.println('-');
    Serial.println();
    delay(6000);
}

void sendProtocolo(){
    //enviar protocolo via ir
      Serial.println("Enviando protocolo: " + protocolo );
}


void loop()
{  

  if (passo <7) {   
    escreveLCD(msg[passo]);
    while (digitalRead(btnDireita) == LOW &&
           digitalRead(btnEsquerda) == LOW &&
           digitalRead(btnEnter) == LOW){

    }
    marcacaoInicial();
  } else if (passo >= 7 && passo != 10) {
    if (navioRestante == 0){
      passo = 10;
    }
    escreveLCD(msg[passo]);
    printTabuleiroB();
    // Aguardando jogador.
     while (digitalRead(btnDireita) == LOW &&
           digitalRead(btnEsquerda) == LOW &&
           digitalRead(btnEnter) == LOW){

    }
    jogar();
  } else {
     //imprimirResultado();
  }
}
