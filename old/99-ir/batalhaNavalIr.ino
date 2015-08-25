// Este codigo precisa de uma IR LED conectado aos pinos interruptores 3 e 5 ligados aos pinos 4 -8

#include <IRremote.h> // IR remote control library

const int botaoPressionado = 1;
const int botao1 = 4; // O 1Âº pino de uma sequencia de 5 pinos connectado ao botÃ£o
boolean estadoBotao[botaoPressionado];
boolean ultimoEstadoBotao[botaoPressionado];
long codigoIrBotao[botaoPressionado] = {0x18E758A7, }; // Colocar a matriz de led nesta linha

IRsend irsend;

void setup(){
  for (int i = 0; i < botaoPressionado; i++){
    estadoBotao[i]=true;
    ultimoEstadoBotao[i]=true;
    int pinoFisico=i + botao1;
    pinMode(pinoFisico, INPUT);
    digitalWrite(pinoFisico, HIGH); // turn on pull-ups
  }
  Serial.begin(9600);
}
void loop() {
  for (int numBotao=0; numBotao<botaoPressionado; numBotao++){
    int proximoPino = numBotao + 4;
    estadoBotao[numBotao] = digitalRead(proximoPino);
    if (estadoBotao[numBotao] != ultimoEstadoBotao[numBotao]){
      if (estadoBotao[numBotao] == LOW){
      irsend.sendSony(codigoIrBotao[numBotao], 32); // Trocar a funÃ§Ã£o sendSony pelas coordenadas X,Y do tiro
      Serial.println("Enviando tiro");
      }
      ultimoEstadoBotao[numBotao] = estadoBotao[numBotao];
    }
  }
}

