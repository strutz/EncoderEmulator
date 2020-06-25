/* Programa de simulaçao de encoder - Thiago Strutz
  Recebe valores e comandos atraves da I2C
  Fases do encoder 00->01->11->10 (avanco), 00->10->11->01 (recuo)
  
*/
#include <Wire.h>
#include <TimerOne.h>

byte i2cBuffer[10] = {0,0,0,0,0,0,0,0,0,0}; // buffer de bytes recebidos da i2c
  // Encoder
  // [0] = comando (0=desligado, 1= horario, 2= anti-horario)
  // [1][2] = valor RPM (INT)
  // [3][4] = resolucao do encoder (INT)

byte cmd = 0; //comando (0=desligado, 1= horario, 2= anti-horario)
int rpm = 0;  // rpm de simulação do encoder, 1 RPM equivale a um ciclo das 4 fases do encoder
int resolucao = 0;  // quantidade de pulsos por volta do encoder
unsigned long period = 0;
int encoderFase = 0;

const int ENC0_A = 2;
const int ENC0_B = 3;
const long TIMER1_PERIOD_MIN = 20;

unsigned long t1 = 0;
unsigned long t2 = 0;

void setup() {
  pinMode(ENC0_A, OUTPUT);
  pinMode(ENC0_B, OUTPUT);
  Timer1.initialize(TIMER1_PERIOD_MIN);  // inicializa Timer 1;
  Timer1.attachInterrupt(Timer1_CallBack); // anexa a função de chamada da interrupcao
  Wire.begin(8);
  Wire.onReceive(i2cReceiveEvent);
  Serial.begin(9600);
}

void loop() {
    Serial.print("Cmd: ");
    Serial.print(cmd);
    Serial.print(" Rpm: ");
    Serial.print(rpm);
    Serial.print(" Resol: ");
    Serial.print(resolucao);
    Serial.print(" PT: ");
    Serial.print(period);
    Serial.print(" Fase: ");
    Serial.println(encoderFase);
    Serial.print(i2cBuffer[0]);
    Serial.print(" ");
    Serial.print(i2cBuffer[1]);
    Serial.print(" ");
    Serial.print(i2cBuffer[2]);
    Serial.print(" ");
    Serial.print(i2cBuffer[3]);
    Serial.print(" ");
    Serial.print(i2cBuffer[4]);
  Serial.println("");
  delay(300);
}

// Leitura da I2C ################################
void i2cReceiveEvent(int howMany){  
  int i=0;
  while (0 < Wire.available()){
    i2cBuffer[i] = Wire.read();
    i ++;
  }
  
  cmd= i2cBuffer[0];
  rpm= i2cBuffer[1]; 
  rpm= rpm<<8;
  rpm|= i2cBuffer[2];
  resolucao= i2cBuffer[3];
  resolucao= resolucao<<8;
  resolucao|= i2cBuffer[4];
  if (rpm != 0 && resolucao != 0)
    period= (60000000 / (long)rpm) / (long)resolucao;
  if (period < TIMER1_PERIOD_MIN)
    period= TIMER1_PERIOD_MIN;
  Timer1.setPeriod(period);
}

// Callback do Timer1 ############################
void Timer1_CallBack(){
  // comando do encoder
  switch (cmd){
  case 1: // avancar
    encoderFase++;
    break;
  case 2: // recuar
    encoderFase--;
    break;
  } 
    
  // limites do ciclo de fases do encoder
  if (encoderFase > 3) encoderFase = 0;
  if (encoderFase < 0) encoderFase = 3;

  // fases do encoder
  switch (encoderFase){
  case 0:
      digitalWrite(ENC0_A, LOW);
      digitalWrite(ENC0_B, LOW);
      break;
  case 1:
      digitalWrite(ENC0_A, HIGH);
      digitalWrite(ENC0_B, LOW);
      break;
  case 2:
      digitalWrite(ENC0_A, HIGH);
      digitalWrite(ENC0_B, HIGH);
      break;
  case 3:
      digitalWrite(ENC0_A, LOW);
      digitalWrite(ENC0_B, HIGH);
      break;
  }
} 
