
//Cria as cooredendas do Tiro.
int arrTiro[] = {0,1};

void setup()
{
  Serial.begin(9600);

}

void loop() {

int marcaJogada(int Tiro[]); 

}


//Função checa se o tiro acertou algum navio
String marcaJogada() {
  char* result;
  
  int tabuleiroB[8][8] = {{1,1,0,0,1,0,1,0},
                          {0,0,0,0,1,0,1,0},
                          {0,0,1,0,1,0,1,0},
                          {0,0,0,0,0,0,1,0},
                          {0,1,0,0,1,0,1,0},
                          {0,1,0,0,0,0,0,0}, 
                          {0,1,0,0,0,0,1,1},
                          {0,1,0,0,0,0,0,0}};   
  int x=0;
  int y=0;  
  
//Percorre o tabuleiroB e verifica se a coord. esta ligada
    for (x=0; x<=7; x++) {
      for(y=0; y<=7; y++) {
        Serial.print(tabuleiroB[x][y]);
        Serial.print(',');
        //Se a coordenada estiver ligada, verifica se o tiro acertou algum navio
        if ((x == arrTiro[0]) && (y == arrTiro[1])) {
//          //Se o tiro acertou - Mostra msg "Acertou", senão, mostra "Agua"
          if (tabuleiroB[x][y] == 1 ) {
            //Substituir por som do Buzzer
            result = " = Acertou";
          } else {
            //Substituir por som do Buzzer
            result = " = Agua";
          }
        }
      }
      Serial.println( result );
      result = "";
    }
    Serial.println();
    Serial.println('-');
    Serial.println();
    delay(6000);
}

