#include "Arduino.h"
#include <elapsedMillis.h>
elapsedMillis timeElapsedLed; //declare global if you don't want it reset every time loop runs

int P[8]={0,1,2,3,4,5,6,7};
int ledAdressZone;

// Set i2c address
PCF8574 ledDriver(0x38); //38 ;21
//millis() rollover simulation
extern volatile unsigned long timer0_millis;
// Variables will change:
int ledState = 0;             // ledState used to set the LED


// constants won't change:
const long interval = 400;           // interval at which to blink (milliseconds)
     

 void ledDriverInit(){
  

// Set pinMode to OUTPUT
 for(int i=0; i<8;i++){
ledDriver.pinMode(i, OUTPUT);
ledDriver.digitalWrite(i,HIGH);
delay(50);
 }

  Serial.print("Init ledDriver...");
  if (ledDriver.begin()){
    Serial.println("OK-LED");
  }else{
    Serial.println("KO-LED");
  }
 }

 void ledDriverRun(){
  int intervalBlink =1000;

     for(int i=0; i<4;i++){ 
       while(ledState == 0){
        if((timeElapsedLed>=interval)&&(ledState == 0)){
    ledState = 1;
      ledDriver.digitalWrite(P[i], HIGH); //delay(400);
      timeElapsedLed=0;
     }
    }
    while(ledState == 1){
      if ((timeElapsedLed>= interval)&&(ledState == 1)) {
      ledState = 0;
      ledDriver.digitalWrite(P[i], LOW); //delay(400);
      timeElapsedLed=0;
    }
    }
  }  
 }

 void ledDriverRunVaccum(){
 int intervalBlink =1000;
 if(timeElapsedLed>intervalBlink){
     for(int i=0; i<4;i++){ 
    ledDriver.digitalWrite(P[i], HIGH);
     }
     if(timeElapsedLed>(intervalBlink*2)){
    timeElapsedLed=0;
     }
    }
    else{
       for(int i=0; i<4;i++){
      ledDriver.digitalWrite(P[i], LOW);
       }
    }
 }

