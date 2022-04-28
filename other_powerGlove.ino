/*
 * Autor Bruno A Patrocinio.
 *                          
 *   V. 1.2.0
 */
#include <MPU6050_tockn.h>
#include <Wire.h>

//pino do flex sensor 1
const int flexPin = A0;

//var globais
unsigned long timer = 0;
float printResOHMS = 0;
float printAngle = 0;
int barCtrlFlex = 0;
boolean ligado = false;

//const globais
const float vcc = 4.98;
const float r_div = 47500.0;
//resistencia quando flex sensor está reto e torcido:
const float straight_res = 25000.0;
const float bend_res = 125000.0;

//prototipos de função
void updateAcell();
void printaSerial();
void updateFlexSensor();
void acionaImpressao();
void escreveAcellSerial();

MPU6050 acell(Wire); //obj do tipo MPU6050

void setup() {
  Serial.begin(38400);  
  pinMode(flexPin, INPUT); //seta o pino como entrada de dados
  Wire.begin();
  acell.begin();
  acell.calcGyroOffsets(true); //calcula os val de offset do MPU6050
  
}

void loop() {
  updateAcell();
  updateFlexSensor();
  acionaImpressao();
  
}//fim de loop

void updateAcell(){
  acell.update(); //atualiza os dados do sensor acel.
  escreveAcellSerial(); //escreve dados na interface serial
  
}//fim de updateAcell

void updateFlexSensor(){
  int ct;
  int flexADC = analogRead(flexPin);
  float flexV = flexADC * vcc / 1023.0;
  float flexR = r_div * (vcc / flexV - 1.0);
  int mapeado = map(flexR, bend_res, straight_res, 0, 255);                
  Serial.write(byte(mapeado)); //envia os dados do flexSensor pela serial
  //Serial.print("Dobra Dedo: ");Serial.println(dobra);
  Serial.flush();
   
  printResOHMS = flexR; //pra printar na função printSerial;
  
}//fim de updateFlexSensor

void escreveAcellSerial(){
  float valorX = map(acell.getAngleX(), -90.0, 90.0, 0.0, 255.0);
  float valorY = map(acell.getAngleY(), -90.0, 90.0, 0.0, 255.0);
  int valX = (int)valorX;
  int valY = (int)valorY;
  Serial.write(byte(valX)); //envia pela serial o angulo eixo X
  Serial.write(byte(valY)); //envia pela serial o angulo eixo Y
  Serial.flush();
  
}//fim de updateAcell

//aciona a impressão no monitor serial quando pressionado a tecla l para ligar e d para desligar
void acionaImpressao(){
  if(Serial.available()){
    char c = Serial.read();
    if(c){
     switch(c){
      case 'l':
        ligado = true;
        break;
      case 'd':
        ligado = false;
        break;
      default:
        break;
     }
      c = NULL;
    }
    
  }

  if(ligado == true){ 
     printaSerial();
  }else{
    //nada
  }

}//fim de acionaImpressao

//printa os dados na serial, quando chamado na função acionaImpressao()
void printaSerial(){
  if(millis() - timer > 1000){
    
    Serial.println("=======================================================");
    Serial.print("temp : ");Serial.println(acell.getTemp());
    Serial.print("accX : ");Serial.print(acell.getAccX());
    Serial.print("\taccY : ");Serial.print(acell.getAccY());
    Serial.print("\taccZ : ");Serial.println(acell.getAccZ());
  
    Serial.print("gyroX : ");Serial.print(acell.getGyroX());
    Serial.print("\tgyroY : ");Serial.print(acell.getGyroY());
    Serial.print("\tgyroZ : ");Serial.println(acell.getGyroZ());
  
    Serial.print("accAngleX : ");Serial.print(acell.getAccAngleX());
    Serial.print("\taccAngleY : ");Serial.println(acell.getAccAngleY());
  
    Serial.print("gyroAngleX : ");Serial.print(acell.getGyroAngleX());
    Serial.print("\tgyroAngleY : ");Serial.print(acell.getGyroAngleY());
    Serial.print("\tgyroAngleZ : ");Serial.println(acell.getGyroAngleZ());
    
    Serial.print("angleX : ");Serial.print(acell.getAngleX());
    Serial.print("\tangleY : ");Serial.print(acell.getAngleY());
    Serial.print("\tangleZ : ");Serial.println(acell.getAngleZ());
    Serial.print("\tresistance: ");Serial.println(String(printResOHMS) + " ohms");
    Serial.print("\tcontrole Incremento: ");Serial.println(String(barCtrlFlex) + " Incr");
    
    
    Serial.println("=======================================================\n");
    timer = millis();
    
  }
}//fim de printaSerial
