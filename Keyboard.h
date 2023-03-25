#include "Arduino.h"
#include "PCF8574.h"

//For arduino uno only pin 1 and 2 are interrupted
#define ARDUINO_UNO_INTERRUPTED_PIN 2
int keyAdressVar ;
// Function interrupt
void keyChangedOnPCF8574();

// Set i2c address
PCF8574 pcf8574(0x39, ARDUINO_UNO_INTERRUPTED_PIN, keyChangedOnPCF8574);
PCF8574::DigitalInput di;
bool keyChanged = false;
String pressedCharacter ="*";
int pin[8];
//PCF8574 pcf8574(0x);0x39 ;20
 void pcf8574InitKeyboard(){
  pcf8574.pinMode(0, INPUT);
  pcf8574.pinMode(1, INPUT);
  pcf8574.pinMode(2, INPUT);
  pcf8574.pinMode(3, INPUT);

  pcf8574.pinMode(4, INPUT);
  pcf8574.pinMode(5, INPUT);
  pcf8574.pinMode(6, INPUT);
  pcf8574.pinMode(7, INPUT);
  pcf8574.digitalWrite(0, HIGH);
  pcf8574.digitalWrite(1, HIGH);
  pcf8574.digitalWrite(2, HIGH);
  pcf8574.digitalWrite(3, HIGH);
  pcf8574.digitalWrite(4, HIGH);
  pcf8574.digitalWrite(5, HIGH);
  pcf8574.digitalWrite(6, HIGH);
  pcf8574.digitalWrite(7, HIGH);
  
  Serial.print("Init pcf8574...");
  if (pcf8574.begin()){
    Serial.println("OK");
  }else{
    Serial.println("KO");
  }
 }

 String  pcf8574Run(){
//Serial.println("OK");

   bool ledOff = true;
 // di = pcf8574.digitalReadAll();
    for(int i =0; i<8;i++){
 pin[i] = pcf8574.digitalRead(i);
  //Serial.print("Di = ");
  // Serial.println(pin[i]);
  }
  
    if(pin[0]==0){
      pressedCharacter = "StartProg1";
      ledOff = false;
     // Serial.print(" - ");
   //Serial.print(pressedCharacter);
    }
  // Serial.print(" - ");
   // Serial.print(di.p1);
     if(pin[1]==0){
      pressedCharacter = "ParfSt1";
      ledOff = false;
    //  Serial.print(" - ");
   // Serial.print(pressedCharacter);
    }
  // Serial.print(" - ");
   // Serial.print(di.p2);
    if(pin[2]==0){
      pressedCharacter = "CauciucSt1";
     // Serial.print(" - ");
    //Serial.print(pressedCharacter);
      ledOff = false;
    }
    if(pin[3]==0){
      pressedCharacter = "AerCompSt1";
    // Serial.print(" - ");
    //Serial.print(pressedCharacter);
      ledOff = false;
    }
   keyChanged= false;
 
  // delay(650);
   
  //return ledOff ;
  return pressedCharacter;
  
 }

 void keyChangedOnPCF8574(){
 // Interrupt called (No Serial no read no wire in this function, and DEBUG disabled on PCF library)
   keyChanged = true;
}
