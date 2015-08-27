#include <MaxMatrix.h>
#include <IRremote.h>
#include <avr/pgmspace.h>

const int data = 4;
const int load = 5;
const int clock = 6;
const int btnEsquerda = 8;
const int btnDireita = 7;
const int btnEnter = 10;
const int receptor = 11;
const int emissor = 9;
const int buzina = 2;
const int maxInUse = 2;
const int tamanhoNavio[7] = {1,1,2,2,3,4,5};

const int PASSO_DEFESA = 7;
const int PASSO_ATAQUE = 8;
const int PASSO_FIM_DE_JOGO_VOCE_PERDEU = 9;
const int PASSO_FIM_DE_JOGO_VOCE_GANHOU = 10;

const char *agua_tipo = "F";
const int timeOut = 2 * 1000; //miliseconds
//Ataque seleciona um item dessa matrix e envia
//Defesa percorre essa matrix, pega as posicoes
//Procura na matriz do tabuleiroA e tabulerioC
//Eniva 
unsigned int matrixAtaque[][8] = 
{{0xA00,0xA10,0xA20,0xA30,0xA40,0xA50,0xA60,0xA70},
 {0xA01,0xA11,0xA21,0xA31,0xA41,0xA51,0xA61,0xA71},
 {0xA02,0xA12,0xA22,0xA32,0xA42,0xA52,0xA62,0xA72},
 {0xA03,0xA13,0xA23,0xA33,0xA43,0xA53,0xA63,0xA73},
 {0xA04,0xA14,0xA24,0xA34,0xA44,0xA54,0xA64,0xA74},
 {0xA05,0xA15,0xA25,0xA35,0xA45,0xA55,0xA65,0xA75},
 {0xA06,0xA16,0xA26,0xA36,0xA46,0xA56,0xA66,0xA76},
 {0xA07,0xA17,0xA27,0xA37,0xA47,0xA57,0xA67,0xA77}};
 
 char const *navios_tipo[7] = {"A","A","B","B","C","D","E"};
 char *msg[99] = {"Digite as coordendadas do Hidroaviao 1 (1)",//0
                            "Digite as coordendadas do Hidroaviao 2 (1)",//1
                            "Digite as coordendadas do Submarino 1 (2)",//2
                            "Digite as coordendadas do Submarino 2 (2)",//3
                            "Digite as coordendadas do Cruzador (3)",//4
                            "Digite as coordendadas do Encouracado (4)",//5
                            "Digite as coordendadas do Porta-aviao (5)",//6
                            "Aguardando Adversario...",//7
                            "Ataque!!",//8
                            "Fim de Jogo! Voce Perdeu!! :( ",//9
                            "Fim de Jogo! Voce Ganhou!! :)"};//10
IRrecv irrecv(receptor);
IRsend irsend;
decode_results results;
boolean ataqueRespondido = false;
boolean debugMode = false;
boolean gameStarted = false;
unsigned long last = millis();
int nav_x1 = 0;
int nav_y1 = 0;
int nav_x2 = 0;
int nav_y2 = 0;
int passo = 0;
int navioRestante = 18;
int navioRestanteAtaque = 18;
unsigned int protocolo;
MaxMatrix m(data, load, clock, maxInUse);
int tabuleiroA[][8] =  {{0,0,0,0,0,0,0,0},
                       {0,0,0,0,0,0,0,0},
                       {0,0,0,0,0,0,0,0},
                       {0,0,0,0,0,0,0,0},
                       {0,0,0,0,0,0,0,0},
                       {0,0,0,0,0,0,0,0},
                       {0,0,0,0,0,0,0,0},
                       {0,0,0,0,0,0,0,0}}; 
int tabuleiroB[][8] ={//0,1,2,3,4,5,6,7  // TEM QUE SER ZERADO
                       {0,0,0,0,0,0,0,0},//0
                       {0,0,0,0,0,0,0,0},//1
                       {0,0,0,0,0,0,0,0},//2
                       {0,0,0,0,0,0,0,0},//3
                       {0,0,0,0,0,0,0,0},//4
                       {0,0,0,0,0,0,0,0},//5
                       {0,0,0,0,0,0,0,0},//6
                       {0,0,0,0,0,0,0,0}}; //7
char const *tabuleiroC[][8] ={//0,1,2,3,4,5,6,7  // TEM QUE SER ZERADO
                       {"F","F","F","F","F","F","F","F"},//"F"
                       {"F","F","F","F","F","F","F","F"},//1
                       {"F","F","F","F","F","F","F","F"},//2
                       {"F","F","F","F","F","F","F","F"},//3
                       {"F","F","F","F","F","F","F","F"},//4
                       {"F","F","F","F","F","F","F","F"}, //5
                       {"F","F","F","F","F","F","F","F"},//6
                       {"F","F","F","F","F","F","F","F"}}; //7 
//Defesa, verifica se o tiro acertou uma dessas linhas.
 //Essa matrix deve ser preenchida no momento da marcacao.
 //Dica: 

unsigned int matrixDefesa[][8] = 
{{0xA00A,0xA10F,0xA20A,0xA30F,0xA40B,0xA50F,0xA60B,0xA70F},
 {0xA01F,0xA11F,0xA21F,0xA31F,0xA41B,0xA51F,0xA61B,0xA71F},
 {0xA02C,0xA12F,0xA22F,0xA32F,0xA42F,0xA52F,0xA62F,0xA72F},
 {0xA03C,0xA13F,0xA23F,0xA33F,0xA43F,0xA53F,0xA63F,0xA73F},
 {0xA04C,0xA14F,0xA24F,0xA34D,0xA44D,0xA54D,0xA64D,0xA74F},
 {0xA05F,0xA15F,0xA25F,0xA35F,0xA45F,0xA55F,0xA65F,0xA75F},
 {0xA06F,0xA16F,0xA26F,0xA36F,0xA46F,0xA56F,0xA66F,0xA76F},
 {0xA07E,0xA17E,0xA27E,0xA37E,0xA47E,0xA57F,0xA67F,0xA77F}};

/************************************************** INICIO TESTE ***************************************************/
/*int tabuleiroB[][8] =
{ 
{1,0,1,0,1,0,1,0},
{0,0,0,0,1,0,1,0},
{1,0,0,0,0,0,0,0},
{1,0,0,0,0,0,0,0},
{1,0,0,1,1,1,1,0},
{0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0},
{1,1,1,1,1,0,0,0}};

char const *tabuleiroC[][8] = 
{ {"A","F","A","F","B","F","B","F"},
  {"F","F","F","F","B","F","B","F"},
  {"C","F","F","F","F","F","F","F"},
  {"C","F","F","F","F","F","F","F"},
  {"C","F","F","D","D","D","D","F"},
  {"F","F","F","F","F","F","F","F"},
  {"F","F","F","F","F","F","F","F"},
  {"E","E","E","E","E","F","F","F"}};

void initTeste(){
  debugMode = true;
  passo = PASSO_DEFESA;
  nav_x1 = 3;
  nav_y1 = 4;
  nav_x2 = 5;
  nav_y2 = 2;
}*/
/*************************************************** FIM TESTE ****************************************************/
/**************************************************** INICIO *****************************************************/
void setup()
{


 // initTeste();
  Serial.begin(9600);
  irrecv.enableIRIn(); 
  pinMode(btnEsquerda, INPUT);     
  pinMode(btnDireita, INPUT);     
  pinMode(btnEnter, INPUT);  
  pinMode(emissor,OUTPUT);
  pinMode(buzina,OUTPUT);
  m.init();
  m.setIntensity(1);
}
void loop()
{  


//  clearMatrixA();
 // clearMatrixB();

  if (passo < PASSO_DEFESA) { 
    aguardaUmClick();

    while(passo < PASSO_DEFESA){ 
        iformaPasso();
        startModoConfiguracao();  
    }

  } else if (passo == PASSO_DEFESA) {
    iformaPasso();
    checaInicioGame();

    while (passo == PASSO_DEFESA){
      startModoDefesa();
      checaFimDeJogo();
    }

  }  else if (passo == PASSO_ATAQUE) {
      iformaPasso();

     while (passo == PASSO_ATAQUE){
       startModoAtaque();
       checaFimDeJogo();
     }

  }else{
      Serial.println("Fim de jogo");
  }
}
/**************************************************** ################################ *****************************************************/

/**************************************************** INICIO CONFIGURACAO DO TABULEIRO *****************************************************/
/* Funcao que faz toda marcacao inicial */

void startModoConfiguracao(){
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
/**
* Funcao que checa as posicoes:
* 1 - Se a marcacao esta em uma posicao invalida (diagonoal, colada)
  2 - Se a posicao ja esta ocupada
  3 - Se a posicao nao corresponde ao tamanho do navio
**/
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
/**************************************************** FIM CONFIGURACAO DO TABULEIRO *****************************************************/

/**************************************************** INICO DO MODO DEFESA *****************************************************/
/* 
  Escuta por comandos 
  
*/  
void startModoDefesa(){
  printTabuleiroB();
  clearMatrixA();
  escuta();
  

    for (int x=0; x<=7; x++) {
      for(int y=0; y<=7; y++) {
        if (matrixAtaque[y][x] == results.value){
          nav_x1 = x;
          nav_y1 = y;
          setColumn(nav_x1,nav_y1,HIGH);
          setRow(nav_y1,nav_x1,HIGH);
          delay(50);
          pisque(nav_x1,nav_y1);
          checkAndMarkDefesa();
          printTabuleiroB();
          clearMatrixA();
        }
      }
    }


}
//Função checa se o tiro acertou algum navio
void checkAndMarkDefesa() {
  String result;
  protocolo = NULL;
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

            protocolo = matrixDefesa[nav_y1][nav_x1];
            buzinarAcerto();
            sendProtocolo();
            Serial.println(result);
            passo = PASSO_DEFESA;
          } else {
            buzinarErro();
            protocolo = matrixDefesa[nav_y1][nav_x1];
            sendProtocolo();
            Serial.println("Agua ");
            passo = PASSO_ATAQUE;
          }
        }
      }
    }
}
/**************************************************** FIM DO MODO DEFESA *****************************************************/

/**************************************************** INICO DO MODO ATAQUE *****************************************************/
/* Inicia o modo de ataque */
void startModoAtaque(){
  
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
        protocolo = matrixAtaque[nav_y1][nav_x1];
        ataqueRespondido = false;
        while(ataqueRespondido == false){
          sendProtocolo();
          checkAndMarqueAtaque();
        }
      }
     }
}

void checkAndMarqueAtaque(){
 if (irrecv.decode(&results)) {
    Serial.println(results.value, HEX);

    for (int x=0; x<=7; x++) {
      for(int y=0; y<=7; y++) {
        if (matrixDefesa[y][x] == results.value){
        nav_x1 = x;
        nav_y1 = y;
        setColumn(nav_x1,nav_y1,HIGH);
        setRow(nav_y1,nav_x1,HIGH);
        delay(50);
        pisque(nav_x1,nav_y1);
        char posicao[5];
        sprintf(posicao, "%x", matrixDefesa[y][x]);

        
        if (posicao[5] == 'F'){
          Serial.println("Agua");
          passo = PASSO_DEFESA;
          ataqueRespondido = true;
        }else{
            String result;

            String nome = String(posicao[5]) == "A" ? "Hidroaviao":
                          String(posicao[5]) == "B" ? "Submarino":
                          String(posicao[5]) == "C" ? "Cruzador":
                          String(posicao[5]) == "D" ? "Encouracado":
                          String(posicao[5]) == "E" ? "Porta-aviao": "";
            result = "Acertou " + nome;

          Serial.println(result);
          passo = PASSO_ATAQUE;
          ataqueRespondido = true;

        }
        clearMatrixA();

        }
      }
    }
    irrecv.resume(); 
  }

}

/**************************************************** FIM DO MODO ATAQUE *****************************************************/



/*Funcao que retorna o valor maximo entre dois paramentros */
int MAX(int a, int b){
  return ((a >= b) ? a : b);
}

/*Funcao que retorna o valor minimo entre dois paramentros */
int MIN(int a, int b){
  return ((a <= b) ? a : b);
}

/*Funcao que escreve no LCD */
void escreveLCD(char *msg){
      Serial.println(msg);   
}
/* Funcao que liga/desliga um ponto */
void setDot(int x, int y, int status){
    m.setDot(x,y,status);
}


/* Funcao que liga/desliga uma coluna */
void setColumn(int x,int ynot, int status){
    for (int y=0;y<8;y++){
        if (y != ynot){
          m.setDot(x,y,status);
        }
        delay(20);
    }
}
/* Funcao que liga/desliga uma linha */
void setRow(int y,int xnot, int status){
    for (int x=0;x<8;x++){
        if (x != xnot){
          m.setDot(x,y,status);
        }
        delay(20);
    }
}
/* Funcao que pisca uma cruz */
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
/* Funcao que pisca dois pontos */
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
/* Funcao que pisca um ponto */
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
/* Funcao que navega pelo eixo X */
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

/* Funcao que navega pelo eixo X */
void incrementaColuna(int *x1, int ynot){
     setColumn(*x1,ynot, LOW);
      *x1 = *x1 + 1;
      if (*x1 > 7){
        *x1 = 0;
      }
      setColumn(*x1,ynot,HIGH);
}

/* Funcao que navega pelo eixo X */
void incrementaLinha(int *y1, int xnot){
      setRow(*y1,xnot,LOW);
      *y1 = *y1 + 1;
      if (*y1 > 7){
        *y1 = 0;
      }
      setRow(*y1,xnot,HIGH);
}

/* Marca o tabuleiroB de acordo com a matrix. */
void clearMatrixA(){
  for (int y=0;y<8;y++){
    for (int x=0;x<8;x++){
        m.setDot(x,y,LOW);
    }
  }
}
/* Marca o tabuleiroB de acordo com a matrix. */
void clearMatrixB(){
  for (int y=0;y<8;y++){
    for (int x=0;x<8;x++){
        m.setDot(x+8,y,LOW);
    }
  }
}
/* Marca o tabuleiroB de acordo com a matrix. */
void printTabuleiroB(){
  for (int y=0;y<8;y++){
    for (int x=0;x<8;x++){
        m.setDot(x+8,y,tabuleiroB[y][x]);
     
    }
  }
}
/* Imprime as selecoes antes de marcar-las */
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
/* Emite um som de erro
**/
void buzinarErro(){
    digitalWrite(buzina,HIGH);
    delay(1000);
    digitalWrite(buzina,LOW);
}

/* Emite um som de acerto
**/
void buzinarAcerto(){
    digitalWrite(buzina,HIGH);
    delay(300);
    digitalWrite(buzina,LOW);
    delay(300);
    digitalWrite(buzina,HIGH);
    delay(300);
    digitalWrite(buzina,LOW);
}

/* Envia o protocolo via Inravermelho */
void sendProtocolo(){
      sendIr(protocolo);
      //enviar protocolo via ir
      Serial.print("Enviando protocolo: ");
      Serial.println(protocolo);
}

/* Envia um codigo via Infravermelho */
void sendIr(unsigned int codigo){
    for (int i=0;i< 3;i++){
      irsend.sendSony(codigo,12);
      delay(100);
    }


    if (debugMode == true){
      Serial.println("Enviando Mensagem: ");
      Serial.print("HEX: ");
      Serial.println(codigo,HEX);
    }
}

/* Aguardar o usuario pressionar algum botao */
void aguardaUmClick(){
   while (digitalRead(btnDireita) == LOW &&
           digitalRead(btnEsquerda) == LOW &&
           digitalRead(btnEnter) == LOW){
      //Aguarda pressionar algum botao
    }
}
/* Checa se jogo nao iniciou e os 2 botoes foram pressionados */
void checaInicioGame(){
  if (gameStarted == false){
       escuta();
  
       if ( results.value == 0x910){
            gameStarted = true;
            passo = PASSO_ATAQUE;
              Serial.println("PASSO_ATAQUE");
      }

       if ( results.value == 0x5CE9){
            gameStarted = true;
            passo = PASSO_DEFESA;
            if (debugMode == true){
              Serial.println("PASSO_DEFESA");
            }
      }
  }
}
/* Checa se o jogo acabou */
void checaFimDeJogo(){
    if (navioRestante == 0){
      passo = PASSO_FIM_DE_JOGO_VOCE_PERDEU;
    }
    if (navioRestanteAtaque == 0){
      passo = PASSO_FIM_DE_JOGO_VOCE_GANHOU;
    }
}
/* Informa ao usuario o passo corrente */
void iformaPasso(){
    escreveLCD(msg[passo]);  
}

/* Envia e aguarda resposta dentro do timeout */
void sendRelay(unsigned int sendCod){
    results.value = NULL;
    last = millis(); 
    do{
      sendIr(sendCod);
      delay(1000);

     if (debugMode == true){
        Serial.println("Aguardando Mensagem: ");
      }

     if (irrecv.decode(&results)) {
        irrecv.resume(); 
     }

    } while(results.value == NULL || (millis() - last) < timeOut);
    
   if (debugMode == true){
      if (results.value != NULL){
        Serial.println("Recebendo (sendRelay) Mensagem: ");
        Serial.print("HEX: ");
        Serial.println(results.value,HEX);
      }
    }
}
/* Funcao que escuta o ir */
void escuta(){
  // results.value = NULL;
   last = millis(); 
   do{
     if (irrecv.decode(&results)) {
        irrecv.resume(); 
      }
    }while(results.value == NULL || (millis() - last) < timeOut);
  
    if (debugMode == true){
      if (results.value != NULL){
        Serial.println("Recebendo (escuta) Mensagem: ");
        Serial.print("HEX: ");
        Serial.println(results.value,HEX);
      }
    }
  
}
