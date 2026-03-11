#include <math.h>

// Entrada

int btnLed = 8;
int btnVoltaje = 9;
int btnCalibracion = 2;

// Salida

int ledR = 3;
int ledG = 5;
int ledB = 6;

// Lectura

int LDRIn = A0;
float valueLDR = 0;
float voltajeLDR = 0;
float RLDR = 0;
float absorbancia = 0;

// Variable de calibración
float calibracion = 0;

// Estado de los botones

int stdBtnLed = 0;
int stdBtnVoltaje = 1;

// Variables de estado

int estadoColor = 0;
int estadoVoltaje = 1;

// Arreglo de voltajes/led

int vRed[3] = {240, 25, 2};
int vGreen[3] = {225, 23, 1};
int vBlue[3] = {195, 20, 1};

float cRed[3] = {};
float cGreen[3] = {};
float cBlue[3] = {};

// 

void setup() {
  // Iniciando el monitor serial
  Serial.begin(9600);

  // Pines de entrada
  pinMode(btnLed, INPUT);
  pinMode(btnVoltaje, INPUT);

  // Pines de salida
  pinMode(ledR, OUTPUT);
  pinMode(ledG, OUTPUT);
  pinMode(ledB, OUTPUT);

  //attachInterrupt(digitalPinToInterrupt(btnCalibracion), calibrarSensor, RISING);

  Serial.println("FOTOCOLORIMETRO - ESPROBREM");
}

void loop() {
  stdBtnLed = digitalRead(btnLed);
  stdBtnVoltaje = digitalRead(btnVoltaje);

  if (stdBtnLed == HIGH) {
    while(digitalRead(btnLed) == HIGH){}
    estadoColor++;
  }

  if (stdBtnVoltaje == HIGH){
    while(digitalRead(btnVoltaje) == HIGH){}
    estadoVoltaje++;
  }

  if(estadoVoltaje == 4){
    estadoVoltaje = 1;
  }
  
  switch(estadoColor){
    case 0 :
      apagarLed(ledR);
      apagarLed(ledG);
      apagarLed(ledB);
    case 1 :
      cambiarIntensidad(ledR, 1, vRed);

      apagarLed(ledG);
      apagarLed(ledB);
    break;

    case 2 :
      cambiarIntensidad(ledG, 2, vGreen);

      apagarLed(ledR);
      apagarLed(ledB);
    break;
    
    case 3 :
      cambiarIntensidad(ledB, 3, vBlue);

      apagarLed(ledR);
      apagarLed(ledG);
    break;

    default :
      estadoColor = 0;
    break;
  }

  // Lectura del voltaje en el LDR
  valueLDR = analogRead(LDRIn);
  voltajeLDR = valueLDR*5/1023;

  RLDR = (11865*voltajeLDR)/(5 - voltajeLDR);

  // absorbancia = log10(0.00501/(5/(11865 + RLDR)));
  absorbancia = log10(voltajeLDR/0.2);

  Serial.print("LDR V: ");
  Serial.println(voltajeLDR);

  Serial.print("LDR R: ");
  Serial.println(RLDR);

  Serial.print("Abs: ");
  Serial.println(absorbancia);

  

  delay(500);
}

void cambiarIntensidad(int led, int estado, int voltajeLed[]){

  if (estadoColor == estado && estadoVoltaje == 1){
    analogWrite(led, voltajeLed[0]);
  }else if (estadoColor == estado && estadoVoltaje == 2){
    analogWrite(led, voltajeLed[1]);
  }else if (estadoColor == estado && estadoVoltaje == 3){
    analogWrite(led, voltajeLed[2]);
  }
}

void apagarLed(int led) {
  analogWrite(led, 0);
}

void calibrarSensor(){

  int promedio = 0;
  calibracion = 0;
  Serial.println("Calibrando ...");
  for(int i = 0; i < 10; i++){
    valueLDR = analogRead(LDRIn);
    voltajeLDR = valueLDR*5/1023;
    promedio += voltajeLDR;
    Serial.print("Calibrando ...");
    Serial.println(i);
  }
  calibracion = promedio / 10;
  Serial.print("Voltaje Luz Ambiental: ");
  Serial.println(calibracion);
  Serial.println("Calibración finalizada");
}











