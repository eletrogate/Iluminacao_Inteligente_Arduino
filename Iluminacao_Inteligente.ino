/* DEFINIÇÃO DE PINOS */
#define pinLamp 2 //PORTA LED OU RELÉ
#define pinSenE 3 // PORTA SENSOR DE ENTRADA
#define pinSenS 4 // PORTA SENSOR DE SAÍDA
#define pinBotao 5 // PORTA INTERRUPTOR OU PUSHBUTTON
#define pinControle 6 // PORTA PARA CHAVEAMENTO DO TRANSISTOR
#define pinLDR A0 // PORTA DE SINAL DO LDR
#define pinPot A1 // PORTA DE AJUSTE DO POTENCIÔMETRO

/* VARIÁVEIS GLOBAIS */
int luz = 0; // VARIÁVEL QUE CONTROLA A SENSIBILIDADE
int valorLDR = 0; // VARIÁVEL QUE CONTROLA O VALOR DO LDR
int pessoas = 0; // CONTROLA A QUANTIDADE DE PESSOAS NO AMBIENTE
boolean botaoAcionado = false;
boolean estadoBotao = false;
boolean estAntBotao = false;
boolean estadoE = false;
boolean estAntE = false;
boolean estadoS = false;
boolean estAntS = false;
boolean estadoLamp = false;

void setup() {
  /* DEFINIÇÃO DE ENTRADAS E SAÍDAS */
  pinMode(pinSenE, INPUT);
  pinMode(pinSenS, INPUT);
  pinMode(pinBotao, INPUT);
  pinMode(pinLamp, OUTPUT);
  pinMode(pinControle, OUTPUT);

  /* INICIA O MONITOR SERIAL */
  Serial.begin(9600);
}

// FUNÇÃO QUE REALIZA A LEITURA DOS SENSORES
void leituraSensores() {
  Serial.print("LDR ");
  Serial.print (valorLDR);
  Serial.print("   LUZ ");
  Serial.print (luz);
  Serial.print("   Sensor saída ");
  Serial.print(estadoS);
  Serial.print("   Sensor entrada ");
  Serial.print(estadoE);
  estadoE = digitalRead(pinSenE);
  estadoS = digitalRead(pinSenS);
  if (estadoE == false && estAntE == true) {
    if (estadoS == false) {
      pessoas++;
    }
  }
  estAntE = estadoE; // ESTADO ANTERIOR É IGUALADO AO ESTADO ATUAL
  if (estadoS == false && estAntS == true) {
    if (estadoE == false) {
      pessoas--;
      if (pessoas < 0) {
        pessoas = 0;
      }
    }
  }
  estAntS = estadoS; // ESTADO ANTERIOR É IGUALADO AO ESTADO ATUAL
  Serial.print("   pessoas ");
  Serial.println(pessoas);
}

// FUNÇÃO PARA ACIONAMENTO DA LÂMPADA
void lampada() {
  if ( pessoas >= 1) {
    estadoLamp = true;
  } else if ( pessoas == 0) {
    estadoLamp = false;
  }
  // digitalWrite(pinLamp, !estadoLamp);
  digitalWrite(pinLamp, estadoLamp);
}

// REALIZA LEITURA DO LDR
void leituraLDR() {
  valorLDR = analogRead(pinLDR);
}

void loop() { // INÍCIO DO LOOP
  // LÊ ESTADOS INICIAIS DOS SENSORES
  luz = analogRead(pinPot);
  // FUNÇÕES DOS SENSORES
  leituraLDR();
  leituraSensores();
  lampada();

  digitalWrite(pinControle, HIGH);

  // VERIFICA AJUSTE PARA DIA OU NOITE
  while (valorLDR > luz && estadoLamp == false) {
    digitalWrite(pinControle, LOW);
    Serial.println(valorLDR);
    estadoBotao = digitalRead(pinBotao);
    // SE O BOTÃO É PRESSIONADO INVERTE ESTADO DA LÂMPADA
    if ( estadoBotao && !estAntBotao) {
      delay(50);
      if ( pessoas >= 1) {
        pessoas = 0;
      } else {
        pessoas = 1;
      }
    }
    estAntBotao = estadoBotao;
    lampada();
    delay(100);
    luz = analogRead(pinPot);
    leituraLDR();
  }

  // LÊ ESTADO DO BOTÃO CASO ESTEJA A NOITE
  estadoBotao = digitalRead(pinBotao);
  if ( estadoBotao && !estAntBotao) {
    delay(50);
    if ( pessoas >= 1) {
      pessoas = 0;
    } else {
      pessoas = 1;
    }
  }
  estAntBotao = estadoBotao;
  delay(10);
} // FINAL DO LOOP