#include <MaxMatrix.h>
#include <IRremote.h>
#include <avr/pgmspace.h>

const int data = 4;
const int load = 5;
const int clock = 6;
const int btnEsquerda = 8;
const int btnDireita = 9;
const int btnEnter = 10;
const int receptor = 11;
const int emissor = 3;
const int buzina = 2;
const int maxInUse = 2;

IRrecv irrecv(receptor);
IRsend irsend;
decode_results results;

int tamanhoNavio[7] = {1,1,2,2,3,4,5};
char *msg[99] = {"Digite as coordendadas do Hidroaviao 1 (1)",//0
                            "Digite as coordendadas do Hidroaviao 2 (1)",//1
                            "Digite as coordendadas do Submarino 1 (2)",//2
                            "Digite as coordendadas do Submarino 2 (2)",//3
                            "Digite as coordendadas do Cruzador (3)",//4
                            "Digite as coordendadas do Encouracado (4)",//5
                            "Digite as coordendadas do Porta-aviao (5)",//6
                            "Aguardando Adversario...",//7
                            "Ataque!!",//8
                            "Fim de Jogo! Voce Perdeu!!"};//9

const int PASSO_DEFESA = 7;
const int PASSO_ATAQUE = 8;
const int PASSO_FIM_DE_JOGO = 9;
boolean ataqueRespondido = false;

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

/*int passo = 7; //TEM QUE SER ZERADO
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
*/
int passo = PASSO_DEFESA;

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

//Ataque seleciona um item dessa matrix e envia
//Defesa percorre essa matrix, pega as posicoes
//Procura na matriz do tabuleiroA e tabulerioC
//Eniva 
const int matrixAtaque[][8] = 
{{0x00,0x10,0x20,0x30,0x40,0x50,0x60,0x70},
 {0x01,0x11,0x21,0x31,0x41,0x51,0x61,0x71},
 {0x02,0x12,0x22,0x32,0x42,0x52,0x62,0x72},
 {0x03,0x13,0x23,0x33,0x43,0x53,0x63,0x73},
 {0x04,0x14,0x24,0x34,0x44,0x54,0x64,0x74},
 {0x05,0x15,0x25,0x35,0x45,0x55,0x65,0x75},
 {0x06,0x16,0x26,0x36,0x46,0x56,0x66,0x76},
 {0x07,0x17,0x27,0x37,0x47,0x57,0x67,0x77}};

//Defesa, verifica se o tiro acertou uma dessas linhas.
 //Essa matrix deve ser preenchida no momento da marcacao.
 //Dica: 
 /*
int matrixDefesa[][8] = 
{{0x00A,0x10F,0x20A,0x30F,0x40B,0x50F,0x60B,0x70F},
 {0x01F,0x11F,0x21F,0x31F,0x41B,0x51F,0x61B,0x71F},
 {0x02C,0x12F,0x22F,0x32F,0x42F,0x52F,0x62F,0x72F},
 {0x03C,0x13F,0x23F,0x33F,0x43F,0x53F,0x63F,0x73F},
 {0x04C,0x14F,0x24F,0x34D,0x44D,0x54D,0x64D,0x74F},
 {0x05F,0x15F,0x25F,0x35F,0x45F,0x55F,0x65F,0x75F},
 {0x06F,0x16F,0x26F,0x36F,0x46F,0x56F,0x66F,0x76F},
 {0x07E,0x17E,0x27E,0x37E,0x47E,0x57F,0x67F,0x77F}};*/


char *navios_tipo[7] = {"A","A","B","B","C","D","E"};
char *agua_tipo = "F";
String protocolo;

int nav_x1 = 3;
int nav_y1 = 4;
int nav_x2 = 5;
int nav_y2 = 2;
boolean debugMode = false;
int navioRestante = 18;

void initTeste(){

}

void setup()
{
  Serial.begin(9600);
  irrecv.enableIRIn(); 
  pinMode(btnEsquerda, INPUT);     
  pinMode(btnDireita, INPUT);     
  pinMode(btnEnter, INPUT);  
  pinMode(emissor,OUTPUT);
  pinMode(buzina,OUTPUT);
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
void clearMatrixA(){
  for (int y=0;y<8;y++){
    for (int x=0;x<8;x++){
        m.setDot(x,y,LOW);
    }
  }
}
/**
* Marca o tabuleiroB de acordo com a matrix.
**/
void clearMatrixB(){
  for (int y=0;y<8;y++){
    for (int x=0;x<8;x++){
        m.setDot(x+8,y,LOW);
    }
  }
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
            tabuleiroC[i][nav_x1] = navios_tipo[passo];
          } 

  
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
            tabuleiroC[i][nav_x1] = navios_tipo[passo];
          }


          passo++; 
          
        }else{
         Serial.println("Posicao invalida.");
         return;
        }
  }


///////////////////////// JOGO


void ataque(){
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
        //envia para o adversario
        pisque(nav_x1,nav_y1);
        protocolo = String(nav_x1) + String(nav_y1);
        ataqueRespondido = false;
        while(ataqueRespondido == false){
          sendProtocolo();
          resultadoAtaque();
        }
      }
     }
}

void resultadoAtaque(){
 if (irrecv.decode(&results)) {
    Serial.println(results.value, HEX);

    //Pega as coordenadas e se errou ou acertou
    if (results.value == 0x910){
        int nav_x1 = 7;
        int nav_y1 = 5;

        setColumn(nav_x1,nav_y1,HIGH);
        setRow(nav_y1,nav_x1,HIGH);
        delay(50);
        pisque(nav_x1,nav_y1);
        if (results.value == 0x00F){
          Serial.println("Agua");
          passo = PASSO_DEFESA;
          ataqueRespondido = true;
        }else{
            String result;

            String nome = String(results.value) == "A" ? "Hidroaviao":
                          String(results.value) == "B" ? "Submarino":
                          String(results.value) == "C" ? "Cruzador":
                          String(results.value) == "D" ? "Encouracado":
                          String(results.value) == "E" ? "Porta-aviao": "";
            result = "Acertou " + nome;

          Serial.println(result);
          passo = PASSO_ATAQUE;
          ataqueRespondido = true;

        }
        clearMatrixA();
    }

    irrecv.resume(); 
  }

}



//Função checa se o tiro acertou algum navio
void markDefesa() {
  String result;
  protocolo = "";
  Serial.println("Recebido o Tiro " + String(nav_x1)+String(",")+String(nav_y1));
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
                          tabuleiroC[y][x] == "E" ? "Porta-aviao": "";
            result = "Acertou " + nome;

            protocolo = String(nav_x1) + String(nav_y1) + String(tabuleiroC[y][x]);
            buzinarAcerto();
            sendProtocolo();
            Serial.println(result);
            passo = PASSO_DEFESA;
          } else {
            buzinarErro();
            protocolo = String(nav_x1) + String(nav_y1) + String(agua_tipo);
            sendProtocolo();
            Serial.println("Agua ");
            passo = PASSO_ATAQUE;
          }
        }
      }
    }
}

void buzinarErro(){
    digitalWrite(buzina,HIGH);
    delay(1000);
    digitalWrite(buzina,LOW);
}

void buzinarAcerto(){
    digitalWrite(buzina,HIGH);
    delay(300);
    digitalWrite(buzina,LOW);
    delay(300);
    digitalWrite(buzina,HIGH);
    delay(300);
    digitalWrite(buzina,LOW);

}

void sendProtocolo(){
      //sendIr(protocolo);
      //enviar protocolo via ir
      Serial.print("Enviando protocolo: ");
      Serial.println(protocolo);
}

void sendIr(int msg){
    irsend.sendSony(msg,12);
    Serial.print("Enviando Mensagem: ");
    Serial.println(msg,HEX);

}
void defesa(){
  
  escuta();
  //Se receber 0x000 entao fica na defesa e o adversario ataca
  if (results.value == 0x000){
    sendIr(0x001);
    passo=PASSO_DEFESA;
  }

  if (results.value == 0x910){
      // Tratar o tiro, se as coordenadas do tiro estao do tamanho do tabuleiro.
        setColumn(nav_x1,nav_y1,HIGH);
        setRow(nav_y1,nav_x1,HIGH);
        delay(50);
        pisque(nav_x1,nav_y1);
        markDefesa();
        printTabuleiroB();
        clearMatrixA();
        irrecv.resume();
    }
}

/**
Quem inicia o jogo e quem ataca
**/
void startGame(){
  results.value = NULL;
  //Enquanto adversario nao confirmar fico escutando
  do{
    sendIr(0x000); // Sony TV power code
    delay(1000);
    escuta();  
  }while (results.value != 0x001);

  passo = PASSO_ATAQUE;
}

void  escuta(){
    results.value = NULL;

   do{
     if (irrecv.decode(&results)) {
        irrecv.resume(); 
      }
    }while(results.value == NULL);
    
    Serial.println("Recebido");
    Serial.println(results.value, HEX);
  
}
void loop()
{  

  if (passo < PASSO_DEFESA) {   
    escreveLCD(msg[passo]);
    while (digitalRead(btnDireita) == LOW &&
           digitalRead(btnEsquerda) == LOW &&
           digitalRead(btnEnter) == LOW){

    }
    marcacaoInicial();
  } else if (passo == PASSO_DEFESA) {

  
    escreveLCD(msg[passo]);
    printTabuleiroB();
    clearMatrixA();

    if ( digitalRead(btnEnter) == HIGH){
            startGame();
            delay(2000);
    }

    while (PASSO_DEFESA){
      defesa();
    }
    if (navioRestante == 0){
      passo = PASSO_FIM_DE_JOGO;
    }

  }  else if (passo == PASSO_ATAQUE) {
     escreveLCD(msg[passo]);
     ataque();
     clearMatrixB();
  }else if  (passo == PASSO_FIM_DE_JOGO){
     //imprimirResultado();
  }else{

  }
}
