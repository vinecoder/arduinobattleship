
int led = 13;

int tabuleiroA [8][8] = {{0,0,0,0,0,0,0,0},
                         {0,0,0,0,0,0,0,0},
                         {0,0,0,0,0,0,0,0},
                         {0,0,0,0,0,0,0,0},
                         {0,0,0,0,0,0,0,0},
                         {0,0,0,0,0,0,0,0},
                         {0,0,0,0,0,0,0,0},
                         {0,0,0,0,0,0,0,0}};
                         
                         
int tabuleiroB [8][8] = {{0,0,0,0,0,0,0,0},
                         {0,0,0,0,0,0,0,0},
                         {0,0,0,0,0,0,0,0},
                         {0,0,0,0,0,0,0,0},
                         {0,0,0,0,0,0,0,0},
                         {0,0,0,0,0,0,0,0},
                         {0,0,0,0,0,0,0,0},
                         {0,0,0,0,0,0,0,0}};
                         
int marcacao [][4] = { {0,0,0,1}, //XX000000
                      {0,3,0,7}, //00XXXXX0
                      {0,2,0,2}}; //NAO PODE POR ESTAR COLADO
                         
int incomingByte = 0;
     
char * msg[] = {"Digite as coordendadas do Hidroaviao 1",
              "Digite as coordendadas do Hidroaviao 2",
              "Digite as coordendadas do Submarino 1",
              "Digite as coordendadas do Submarino 2",
              "Digite as coordendadas do Cruzadores",
              "Digite as coordendadas do Encouracados",
              "Digite as coordendadas do Porta-avioes"
            };
void setup() {  
    Serial.begin (9600);
}

int count = 0;
int foimarcado = false;

void loop() {

  A_lerMarcacao();
  
        
}
/**
 * Função que lê uma string da Serial
 * e retorna-a
 */
String leStringSerial(){
  String conteudo = "";
  char caractere;
  
  // Enquanto receber algo pela serial
  while(Serial.available() > 0) {
    // Lê byte da serial
    caractere = Serial.read();
    // Ignora caractere de quebra de linha
    if (caractere != '\n'){
      // Concatena valores
      conteudo.concat(caractere);
    }
    // Aguarda buffer serial ler próximo caractere
    delay(10);
  }
    
  return conteudo;
}

void A_lerMarcacao(){
  String recebido="";
    
  if (foimarcado == false){
  Serial.println("BATALHA NAVAL");
  Serial.println(":: Digite as 4 coordenadas Exemplo 0101");
  Serial.println(msg[count++]); 

   do { 
      // Lê toda string recebida
      String recebido = leStringSerial();
   }while(count < 7);
    Serial.print("Recebi: ");
    Serial.println(recebido);

   foimarcado = true;
  Serial.println(":: Marcacao Finalizada");
  }
  
}  
/*

//Funcao que checar se a posicao e valida e nao esta ocupada
int checarPosicao(int x1,int y1, int x2, int y2){
    if (tabuleiroA[x1,y1] == HIGH || tabuleiroA[x2,y2] == HIGH) {
                Serial.print("POSICAO OCUPADA");
                return 1;
      }else{
                return 0;
      }
    
}


void escreveNavio(int x1,int y1, int x2, int y2,int TIPO){
int delta_x =  x2-x1;
    int delta_y =  x2-x1;
      
      if (delta_x == 0 && delta_y == 0 && TIPO == 0) {
                Serial.print("HIDROAVIAO");
        if (tabuleiroA[x1,y1] == HIGH || tabuleiroA[x2,y2] == HIGH) {
                Serial.print("POSICAO OCUPADA");
        }
      }else if(delta_x == 0){ //VERTICAL
        if (tabuleiroA[x1,y1] == HIGH || tabuleiroA[x2,y2] == HIGH) {
                Serial.print("POSICAO OCUPADA");
        }
      
      }
}*/
