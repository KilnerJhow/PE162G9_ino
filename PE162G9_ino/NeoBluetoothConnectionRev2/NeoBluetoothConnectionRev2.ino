//https://sites.google.com/site/toucatronic/arduino/arduino---manipulando-obj-string
//https://www.arduino.cc/en/Tutorial/StringSubstring
//https://www.arduino.cc/en/Tutorial/StringStartsWithEndsWith
//https://www.arduino.cc/en/Tutorial/StringComparisonOperators

/*
1---------1200
2---------2400
3---------4800
4---------9600
5---------19200
6---------38400
7---------57600
8---------115200
*/

#include <SoftwareSerial.h>
#include <Servo.h>

SoftwareSerial serialBt(8, 9); //RX TX

Servo srvMotor1;
Servo srvMotor2;
int posServo;

String data = "";

String sensorValues[3];
int sensorIndex;
int index;
boolean reading;
boolean startValue;

float srvMotor1Value;
float srvMotor2Value;


int azimuthZero = 0;
int rollZero = 0;
int rotateSrvMotor1 = 0;
int newAzimuth, newRoll;

int deltaAzimuth = 0;
int oldDeltaAzimuth = 0;
int deltaRoll = 0;
int oldDeltaRoll = 0;
int range = 3;

boolean isHighAzimuth = true;
boolean isHighRoll = true;

String ang = "";

void setup() {
  srvMotor1.attach(3);
  srvMotor2.attach(4);
  srvMotor1.write(90);
  srvMotor2.write(90);
  Serial.begin(19200);
  serialBt.begin(19200);

  sensorIndex = 0;
  index = 0;
  reading = true;
  startValue = true;
  delay(200);
}

void loop() {

  if (serialBt.available()) {
    while(serialBt.available()){
      
    data += (char)serialBt.read();
    }
    parseReadBuffer();
  }
  
    if(sensorValues [0] != "") {
      
      //Serial.print("String (Azimuth): ");
      //Serial.println(sensorValues[2]);
      /*Serial.print("String (Pitch): ");
      Serial.println(sensorValues[1]);
      Serial.print("String (Roll): ");
      Serial.println(sensorValues[2]);*/
    }
  //data = "";  
  //delay(10);
}

void gotMessage(String message) {

    switch(sensorIndex){
      
        case 0: 
          sensorValues[0] = message;
          sensorIndex++;
          break;
        case 1:
          sensorValues[1] = message;
          sensorIndex++;
          break; 
        case 2:
          sensorValues[2] = message;
          sensorIndex = 0;
          motorsRotation(sensorValues[0].toInt(), sensorValues[2].toInt());
          break;  
      }

      if(sensorValues [0] != "") {
      
      Serial.print("String (Azimuth): ");
      Serial.println(sensorValues[2]);
      /*Serial.print("String (Pitch): ");
      Serial.println(sensorValues[1]);
      Serial.print("String (Roll): ");
      Serial.println(sensorValues[2]);*/
      delay(20);
    }
      if(sensorValues[0] != "") {

        //motorsRotation(sensorValues[0].toInt(), sensorValues[2].toInt());
      }
      
}

void parseReadBuffer() {
  
  int endValue = data.indexOf('&');
  if (endValue == -1) return;
  
  String sensorValue = data.substring(0, endValue);
  
  data = data.substring(endValue + 1);

  gotMessage(sensorValue);
  
  parseReadBuffer();
}

int motorsRotation(int azimuth, int roll){

  if (startValue == true){
    
    azimuthZero = azimuth;
    rollZero = roll;
    startValue = false;
    }else{
      
      newAzimuth = azimuth;
      newRoll = roll;
        
        Serial.print("nA: ");
        Serial.println(newAzimuth);
        Serial.print("aZ: ");
        Serial.println(azimuthZero);
        Serial.print("nR: ");
        Serial.println(newRoll);
        Serial.print("rZ: ");
        Serial.println(rollZero);
        delay(20);

        if(azimuthZero > 90 && isHighAzimuth && newAzimuth < 0){

            newAzimuth = (180 + (-newAzimuth));
            Serial.print("AzimuthModulo: ");
            Serial.println(newAzimuth); 
          }
          
        if(newAzimuth > azimuthZero && newAzimuth < (azimuthZero + 90)){

          deltaAzimuth = newAzimuth - azimuthZero;
          Serial.print("dZ maior: ");
          Serial.println(deltaAzimuth);
          if(((deltaAzimuth - oldDeltaAzimuth) > range) || ((oldDeltaAzimuth - deltaAzimuth) > range)){
            
            srvMotor1.write(90 + deltaAzimuth);
            delay(50);
            oldDeltaAzimuth = deltaAzimuth;
            isHighAzimuth = true;
          }
         }
         
         if(newAzimuth < azimuthZero && newAzimuth > (azimuthZero - 90)){

          deltaAzimuth = azimuthZero - newAzimuth;
          Serial.print("dZ menor: ");
          Serial.println(deltaAzimuth);
           if(((deltaAzimuth - oldDeltaAzimuth) > range) || ((oldDeltaAzimuth - deltaAzimuth) > range)){

              srvMotor1.write(90 - deltaAzimuth);
              delay(50);
              oldDeltaAzimuth = deltaAzimuth;
              isHighAzimuth = false;
            }
         }

// Move Servo 2
         if(rollZero > 90 && isHighRoll && newRoll < 0){

            newRoll = (180 + (-newRoll));
            Serial.print("RowModulo: ");
            Serial.println(newRoll); 
          }
          
        if(newRoll > rollZero && newRoll < (rollZero + 90)){

          deltaRoll = newRoll - rollZero;
          Serial.print("dR maior: ");
          Serial.println(deltaRoll);
          if(((deltaRoll - oldDeltaRoll) > range) || ((oldDeltaRoll - deltaRoll) > range)){
            
            srvMotor2.write(90 + deltaRoll);
            delay(50);
            oldDeltaRoll = deltaRoll;
            isHighRoll = true;
          }
         }
         
         if(newRoll < rollZero && newRoll > (rollZero - 90)){

          deltaRoll = rollZero - newRoll;
          Serial.print("dR menor: ");
          Serial.println(deltaRoll);
           if(((deltaRoll - oldDeltaRoll) > range) || ((oldDeltaRoll - deltaRoll) > range)){

              srvMotor2.write(90 - deltaRoll);
              delay(50);
              oldDeltaAzimuth = deltaRoll;
              isHighRoll = false;
            }
         }
  }
}

