#define som 3 //Buzzer
#define pot 5 //Potenciometro
#define l1 8 //LED
#define l2 9
#define l3 10
#define b1 2 //Botão
#define b2 1
#define b3 0
#define r 4 //Aleatório

#define tempo 200

int seq[50];
int fases = 50;
int modo;

void setup() {
  pinMode(som, OUTPUT);
  pinMode(pot, INPUT);
  pinMode(l1, OUTPUT);
  pinMode(l2, OUTPUT);
  pinMode(l3, OUTPUT);
  pinMode(b1, INPUT);
  pinMode(b2, INPUT);
  pinMode(b3, INPUT);  
  Serial.begin(9600);
  randomSeed(analogRead(r));
  modo = 5;
}

void emitirSom(int s){
  if (s==l1)tone(som, 526, 50); //Dó
  else if (s==l2) tone(som, 663, 50); //Mi
  else if (s==l3) tone(som, 792, 50); //Sol
  delay(10);
  digitalWrite(som, LOW);
}

void acender (int l){  
  digitalWrite(l, HIGH); 
  delay(tempo);
  digitalWrite(l, LOW);
  delay(tempo/2);
}

void apagar(int l){
  digitalWrite(l, LOW);
  delay(tempo);
  digitalWrite(l, HIGH);
  delay(tempo/2);
}

int botao(int x, int j){
  //Acertou
  if (x==seq[j]){
    return 1;
  }
  //Errou
  else{
    Serial.println("game over");
    Serial.print("Ordem correta: ");
    for (int i = 0; i<50; i++){
      if (seq[i]!=0) Serial.print(String(seq[i]));
      else break;
      if (seq[i+2]==0) Serial.print(" e ");
      else if (seq[i+1]!=0) Serial.print(", ");
      else Serial.println(".");
    }
    return 2;
  }
}

void modoNormal(int j){
  if (seq[j]==1) {        
    emitirSom(l1);
    acender(l1);
  }
  else if (seq[j]==2) {
    emitirSom(l2);
    acender(l2);
  }
  else {
    emitirSom(l3);
    acender(l3);
  }
}

void jogo(){
  int j, rs[3];
  if (modo == 6){
    for (j=0; j<3; j++){
      rs[j] = random(1000);
    }
    for (j=0; j<3; j++){
      tone(som, rs[0], 50);
      acender(l1);        
      delay(200);
    }
    delay(200);
    for (j=0; j<3; j++){
      tone(som, rs[2], 50);
      acender(l2);
      delay(200);
    }
    delay(200);
    for (j=0; j<3; j++){
      tone(som, rs[2], 50);
      acender(l3);
      delay(200);
    }
    delay(200);
  }

  int i=0;
  for (int i=0; i<fases; i++){
    if (i!=0) Serial.println("concluída!");
    
    //Gerar valores
    bool ok;
    do {
      ok = false;
      seq[i] = random(1, 4);
      if (modo == 0 || modo >= 4) ok = true;
      else if (modo == 1 && seq[i]!=3) ok = true;
      else if (modo == 2 && seq[i]!=1) ok = true;
      else if (modo == 3 && seq[i]!=2) ok = true;
    } while(ok==false);

    Serial.print("Fase " + String(i + 1) + ": ");
    
    // LEDs    
    for (j=0; j<=i; j++){      
      if (modo == 4){
        digitalWrite(l1, HIGH);
        digitalWrite(l2, HIGH);
        digitalWrite(l3, HIGH);
        delay(500);
        if (seq[j]==1) {
          emitirSom(l1);
          apagar(l1);
        }
        else if (seq[j]==2){
          emitirSom(l2);
          apagar(l2);
        }
        else{
          emitirSom(l3);
          apagar(l3);
        }
      }
      else if (modo < 6){
        if (modo == 5) modoNormal(j);
        modoNormal(j);
      }
      else if (modo == 6){
        tone(som, rs[j], 50);
        delay(tempo + tempo);
      }
    }
    digitalWrite(l1, LOW);
    digitalWrite(l2, LOW);
    digitalWrite(l3, LOW);

    //Pressionar botões
    j=0;
    int z = 1;
    do{
      int x=0;
      if (analogRead(b1)==1023) {
        emitirSom(l1);
        acender(l1);
        x = 1;
      }
      else if (analogRead(b2)==1023){
        emitirSom(l2);
        acender(l2);
        x = 2;
      }
      else if (analogRead(b3)==1023) {
        emitirSom(l3);
        acender(l3);
        x = 3;
      }
      if (x>0) {
        z = botao(x, j);
        if (z!=1) break;
      }
      else j--;
      j++;
    } while (j<=i);    
    delay(2000);
    if (z!=1)break;
  }
}

void loop() {
  //Jogo
  acender(l1);
  acender(l2);
  acender(l3);
  Serial.println("\nMENU");
  Serial.println("1 - JOGO");
  Serial.println("2 - MODO DE JOGO");
  Serial.println("3 - VELOCIDADE DE JOGO\n");
  while (1==1){
    if (analogRead(b1)==1023) {
      Serial.println("JOGO");
      acender(l1);
      delay (2000);
      jogo();
      break;
    }
    //Modo
    else if (analogRead(b2)==1023){
      Serial.println("MODO DE JOGO");
      acender(l2);
      delay (500);
      while (analogRead(b1)!=1023){
        modo = ceil(analogRead(pot)/(1023/7));
        digitalWrite(l1, LOW);
        digitalWrite(l2, LOW);
        digitalWrite(l3, LOW);
        //0 = 3 LEDs
        if (modo == 0){
          digitalWrite(l1, HIGH);
          digitalWrite(l2, HIGH);
          digitalWrite(l3, HIGH);
        }
        //1 = Verde e amarelo
        else if (modo == 1){
          digitalWrite(l1, HIGH);
          digitalWrite(l2, HIGH);
        }
        //2 = Amarelo e vermelho
        else if (modo == 2){
          digitalWrite(l2, HIGH);
          digitalWrite(l3, HIGH);
        }
        //3 = Verde e vermelho
        else if (modo == 3){
          digitalWrite(l1, HIGH);
          digitalWrite(l3, HIGH);
        }
        //4 = 3 LED apagando 
        else if (modo == 4){
          ;
        }
        //5 = Repetição
        else if (modo == 5){
          delay(200);
          if (analogRead(b1)==1023) break;
          digitalWrite(l1, HIGH);
          digitalWrite(l2, HIGH);
          digitalWrite(l3, HIGH);          
          delay(200);
        }        
        //6 = Som
        else {
          modo = 6;
          delay(100);
          digitalWrite(l3, HIGH);          
          delay(100);
        }
      }
      Serial.println("Modo " + String(modo + 1) + " selecionado");
      digitalWrite(l1, LOW);
      digitalWrite(l2, LOW);
      digitalWrite(l3, LOW);
      delay(100);
      acender(l1);
      break;
    }
    //Velocidade
    else if (analogRead(b3)==1023) {
      Serial.println("VELOCIDADE (TESTE)");
      acender(l3);
      delay (500);
      break;
    }
  }
}
