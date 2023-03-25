#include <OneWire.h>
#include <Wire.h>
#include<EEPROM.h>
#include "Service.h"
//#include "MemoryZone.h"
#include "Display.h"
//#include "OutputReel.h"
#include "Coin.h"
#include "Keyboard.h"
#include "Led.h"
#include "Scanner.h"
//#include "Contabilitate.h"
//#include "SetingMenu.h"
#include <elapsedMillis.h>
 
// Variabila total bani incasati
int totalCoin;
//variable for price time/Ron
  int timeCoin1 = 0,timeCoin2 = 0,timeCoin3 = 0,timeCoin4 = 0;
// rezervare zone de memorie in eeprom
int totalCoins = 10; // zona memorare total bani incasati
//.........zona minute/RON.................
int timeCoinsEeprom1 = 20; 
int timeCoinsEeprom2 = 30;
int timeCoinsEeprom3 = 40;
int timeCoinsEeprom4 = 50;
//Variabile timp VaccumClean
// new statia1
int programSelector = 0;
unsigned long totalTimeCoin = 0;
int minuteRamase = 0;
int secundeRamase = 0;
 float newCoin = 0;
 float newTimeCoin = 0;
 int  pinReel[] = {12,9,10,11}; //(asp,aerComp,negruCauciuc,Parf)

   //--------
  

void setup() {
Serial.begin(115200);   
  Wire.begin();
  Wire.setClock(400000L);
  pinMode(coinPin, INPUT);
  pinMode(inhibitCoin, OUTPUT);
  //Set val Pin Arduino nano
 digitalWrite(coinPin,HIGH);
  digitalWrite(inhibitCoin,LOW);
  pcf8574InitKeyboard();
  ledDriverInit();
  //initReel();
  attachInterrupt(digitalPinToInterrupt(3), coinInterrupt, LOW); //SetCoinPin
//set LCD instance
  lcd.begin(20, 4);
  lcd.setBacklight(HIGH); 
  lcd.setCursor(0, 0);
  lcd.print("   Aspirator4Comenzi");
  lcd.setCursor(6, 1);
  lcd.print("Vers1.0EE ");
  lcd.setCursor(2, 2);
  lcd.print(" ZAN-Horduino ");
  lcd.setCursor(2, 3);
  lcd.print("  1Keyboard IC2");
  
   lcd1.begin(16, 2);
  lcd1.setBacklight(HIGH); 
  lcd1.setCursor(0, 0);
  lcd1.print("1KbSerialWithScannerV2");
  delay(3000); lcd.clear();lcd1.clear();
   timeCoin1 = EEPROM.get(timeCoinsEeprom1,timeCoin1);
   timeCoin2 = EEPROM.get(timeCoinsEeprom2,timeCoin2);
    timeCoin3 = EEPROM.get(timeCoinsEeprom3,timeCoin3);
     timeCoin4 = EEPROM.get(timeCoinsEeprom4,timeCoin4);
     for(int i=0;i<4;i++){
    pinMode(pinReel[i],OUTPUT);
    digitalWrite(pinReel[i],LOW);
   
  }
}


void loop() {
  timeCoin1 = EEPROM.get(timeCoinsEeprom1,timeCoin1);
   timeCoin2 = EEPROM.get(timeCoinsEeprom2,timeCoin2);
    timeCoin3 = EEPROM.get(timeCoinsEeprom3,timeCoin3);
     timeCoin4 = EEPROM.get(timeCoinsEeprom4,timeCoin4);
  // pressedCharacter = pcf8574Run();
       //  Serial.println(pressedCharacter);
if(coin>0){
  //start program ReelComand
vacuumCleaner();
}
  //activare meniu reglaje
  if (digitalRead(meniuButton[0]) ==LOW)
  {
   //pressedCharacter = "*";
    meniuprog();
  }
  if(digitalRead(meniuButton[2]) ==LOW){
      lcd.clear();
      //pressedCharacter = "*";
      setConta();
  }

  //Afisare lcd Secundar
lcd1.setCursor(0, 0);
  lcd1.print("Aspirat4Functii");
  lcd1.setCursor(0, 1);
  lcd1.print(" V2 Set Meniu");
  
  // Afisare Lista Preturi
   lcd.setCursor(0,0);
  lcd.print("Aspirat:1Ron=");
  lcd.setCursor(13,0);
   lcd.print(timeCoin1/60);
   lcd.print(",");
   lcd.setCursor(15,0);
   if((timeCoin1%60)>=10){
    lcd.print(timeCoin1%60);
   }
   else{
    lcd.print("0");
    lcd.print(timeCoin1%60);
   }
   lcd.setCursor(17,0);
   lcd.print("Mi");
  
   lcd.setCursor(0,1);
  lcd.print("Parfum :1Ron=");
  lcd.setCursor(13,1);
   lcd.print(timeCoin2/60);
   lcd.print(",");
   lcd.setCursor(15,1);
   if((timeCoin2%60)>=10){
    lcd.print(timeCoin2%60);
   }
   else{
    lcd.print("0");
    lcd.print(timeCoin2%60);
   }
  lcd.setCursor(17,1);
   lcd.print("Mi");
   
    lcd.setCursor(0,2);
  lcd.print("Negru C:1Ron=");
  lcd.setCursor(13,2);
   lcd.print(timeCoin3/60);
   lcd.print(",");
   lcd.setCursor(15,2);
   if((timeCoin3%60)>=10){
    lcd.print(timeCoin3%60);
   }
   else{
    lcd.print("0");
    lcd.print(timeCoin3%60);
   }
    lcd.setCursor(17,2);
   lcd.print("Mi");

    lcd.setCursor(0,3);
  lcd.print("A.Comp :1Ron=");
  lcd.setCursor(13,3);
   lcd.print(timeCoin4/60);
    lcd.print(",");
   lcd.setCursor(15,3);
   if((timeCoin4%60)>=10){
    lcd.print(timeCoin4%60);
   }
   else{
    lcd.print("0");
    lcd.print(timeCoin4%60);
   }
   lcd.setCursor(17,3);
   lcd.print("Mi");
 ledDriverRun();
   

}



// setare conabilitate
void setConta(){
  lcd1.clear();
  digitalWrite(inhibitCoin,HIGH);
  delay(350);
  boolean set1 = true;
  totalCoin = EEPROM.get(totalCoins,totalCoin);
  while(set1){
   // pcf8574Run(); //read the keypad
    lcd1.setCursor(1,0);
    lcd1.print("Total : ");
    lcd1.print(totalCoin);
     lcd1.setCursor(0,1);
    
   if ((digitalRead(meniuButton[0]) ==LOW)&&(totalCoin>=100))
  {
  // pressedCharacter = " ";
      totalCoin = totalCoin-50;
      delay(250);
      if(totalCoin<=50){
         totalCoin=0;
          }
      }
      lcd1.print("Exit=>Set");
   if (digitalRead(meniuButton[1]) ==LOW)
  {
   pressedCharacter = "*";
      EEPROM.put(totalCoins,totalCoin);
      set1=false;
      digitalWrite(inhibitCoin,LOW);
      lcd1.clear();
      delay(300);
     }
    }
    
  }

// setare timp/coin
void coin_time(){

  int isOk = 0;
  boolean isOk1 = true;
  int coin = 0 ;
   timeCoin1 =  EEPROM.get(timeCoinsEeprom1,timeCoin1);
   timeCoin2 = EEPROM.get(timeCoinsEeprom2,timeCoin2);
   timeCoin3 = EEPROM.get(timeCoinsEeprom3,timeCoin3);
   timeCoin4 = EEPROM.get(timeCoinsEeprom4,timeCoin4);
  
   lcd1.clear();
    delay(250);
    do{

       while(isOk==0){
  //pcf8574Run(); //read the keypad
   lcd1.setCursor(0,0);
  lcd1.print("Pt.Iesire = *Meniu");
  delay(250);
   if(digitalRead(meniuButton[0]) ==LOW){
    // pressedCharacter = " ";
     delay(250);
      isOk=9;
      isOk1=false;
      lcd1.clear();
    }
    
    lcd1.setCursor(0,1);
    lcd1.print("Pt.Setare = *Set");
    delay(850);
    lcd1.clear();
     lcd1.setCursor(0,0);
    lcd1.print("Pt. Next = *Conta");
    lcd1.setCursor(0,1);
  lcd1.print("Set Pret1 Timp/Ron");
  
     if(digitalRead(meniuButton[1]) ==LOW){
      delay(250);
      pressedCharacter = "* ";
      isOk=1;
      lcd1.clear();
    }
     if(digitalRead(meniuButton[2]) ==LOW){
      delay(250);
      pressedCharacter = " ";
      isOk=2;
      lcd1.clear();
    }
    delay(850);
    lcd1.clear();
}
 //SetPret1   
     
      if(isOk==1){
   lcd1.setCursor(0,0);
  lcd1.print("Set Pret1 Timp/Ron");
      }
  while(isOk==1){
    delay(50);
 
    if(digitalRead(meniuButton[0]) ==LOW){
      
      delay(250);  
     timeCoin1 = timeCoin1 +1;
      if(timeCoin1 > 600){
        timeCoin1 = 0;
      }
    }
    if(timeCoin1/60<10){
    lcd1.setCursor(0,1);
    lcd1.print("1Ron=");
    lcd1.print("0");
    lcd1.print(timeCoin1/60);
    lcd1.print("M");
    }else{
       lcd1.setCursor(0,1);
    lcd1.print("1Ron=");
    lcd1.print(timeCoin1/60);
    lcd1.print("M");
      }
      lcd1.setCursor(8,1);
    lcd1.print("+");
    lcd1.setCursor(10,1);
    lcd1.print("Sec:");
    if((timeCoin1%60)>9){
      lcd1.setCursor(13,1);
   lcd1.print(timeCoin1%60);
    }else{
      lcd1.setCursor(13,1);
      lcd1.print("0");
      lcd1.print(timeCoin1%60);   
    }
     if(digitalRead(meniuButton[2]) ==LOW){ 
    
        timeCoin1 = 0;
         lcd1.clear();
          lcd1.setCursor(0,0);
  lcd1.print("SetP1Timp/Ron");
     }
    if(digitalRead(meniuButton[1]) ==LOW){
    
      EEPROM.put(timeCoinsEeprom1,timeCoin1);
      isOk=2;
      lcd1.clear();
    }
  }
    delay(250);
while(isOk==2){
   delay(50);
 
   lcd1.setCursor(0,0);
  lcd1.print("Pt.Iesire=Meniu");
  delay(250);
   if(digitalRead(meniuButton[0]) ==LOW){
   //  pressedCharacter = " ";
     delay(250);
      isOk=9;
      isOk1=false;
      lcd1.clear();
    }
    
    lcd1.setCursor(0,1);
    lcd1.print("Pt.Setare=*Set");
    delay(850);
    lcd1.clear();
     lcd1.setCursor(0,0);
    lcd1.print("Pt.Next=Conta");
    lcd1.setCursor(0,1);
  lcd1.print("Set Pret2 Timp/Ron");
     if(digitalRead(meniuButton[1]) ==LOW){
      delay(250);
    
      isOk=3;
      lcd1.clear();
    }
     if(digitalRead(meniuButton[2]) ==LOW){
      delay(250);
      //pressedCharacter = " ";
      isOk=4;
      lcd1.clear();
    }
    delay(850);
    lcd1.clear();
}

  //SetPret2
  lcd1.clear();
  lcd1.setCursor(0,0);
  lcd1.print("SetPret2 Timp/Ron");
   delay(250);
 
  while(isOk==3){
    delay(50);
    
     
    if(digitalRead(meniuButton[0]) ==LOW){ 
     delay(250);
     timeCoin2 = timeCoin2 +1;
      if(timeCoin2 > 600){
        timeCoin2 = 0;
      }
    
    }
    if(timeCoin2/60<10){
    lcd1.setCursor(0,1);
    lcd1.print("1Ron=");
    lcd1.print("0");
    lcd1.print(timeCoin2/60);
    lcd1.print("M");
    }else{
       lcd.setCursor(0,1);
    lcd1.print("1Ron=");
    lcd1.print(timeCoin2/60);
    lcd1.print("M");
      }
      lcd1.setCursor(8,1);
    lcd1.print("+");
    lcd1.setCursor(10,1);
    lcd1.print("Sec");
  if((timeCoin2%60)>9){
    lcd1.setCursor(13,1);
   lcd1.print(timeCoin2%60);
    }else{
       lcd1.setCursor(13,1);
      lcd1.print("0");
      lcd1.print(timeCoin2%60);   
    }
    if(digitalRead(meniuButton[2]) ==LOW){ 
     // pressedCharacter = " ";
        timeCoin2 = 0;
         lcd1.clear();
          lcd1.setCursor(0,0);
  lcd1.print("SetP2Timp/Ron");
     }
    if(digitalRead(meniuButton[1]) ==LOW){
      //pressedCharacter = " ";
      EEPROM.put(timeCoinsEeprom2,timeCoin2);
      isOk=4;
      lcd1.clear();
    }
  }
  
   delay(250);
while(isOk==4){
   delay(50);
  //pcf8574Run(); //read the keypad
   lcd1.setCursor(0,0);
  lcd1.print("Pt.Iesire=Meniu");
  delay(250);
   if(digitalRead(meniuButton[0]) ==LOW){
    pressedCharacter = " ";
     delay(250);
      isOk1=false;
      isOk=9;
      lcd1.clear();
    }
    
   lcd1.setCursor(0,1);
    lcd1.print("Pt.Setare=*Set");
    delay(850);
    lcd1.clear();
     lcd1.setCursor(0,0);
    lcd1.print("Pt.Next=*Conta");
    lcd1.setCursor(0,1);
  lcd1.print("SetPret3Timp/Ron");
     if(digitalRead(meniuButton[1]) ==LOW){
      delay(250);
      pressedCharacter = " ";
      isOk=5;
      lcd1.clear();
    }
     if(digitalRead(meniuButton[2]) ==LOW){
      delay(250);
      pressedCharacter = " ";
      isOk=6;
      lcd1.clear();
    }
    delay(850);
    lcd1.clear();
}

  //Set Pret3
  lcd1.clear();
  lcd1.setCursor(0,0);
  lcd1.print("SetP3Timp/Ron");
   delay(250);
 while(isOk==5){
  delay(50);
  //pcf8574Run(); //read the keypad
     
    if(digitalRead(meniuButton[0]) ==LOW){ 
      delay(250);
      pressedCharacter = " ";
     timeCoin3 = timeCoin3 +1;
      if(timeCoin3 > 600){
        timeCoin3 = 0;
      }
    }
    if(timeCoin3/60<10){
    lcd1.setCursor(0,1);
    lcd1.print("1Ron=");
    lcd1.print("0");
    lcd1.print(timeCoin3/60);
    lcd1.print("M");
    }else{
       lcd.setCursor(0,1);
    lcd1.print("1Ron=");
    lcd1.print(timeCoin3/60);
    lcd1.print("M");
      }
      lcd1.setCursor(8,1);
    lcd1.print("+");
    lcd1.setCursor(10,1);
    lcd1.print("Sec:");
   if((timeCoin3%60)>9){
     lcd1.setCursor(13,1);
   lcd1.print(timeCoin3%60);
    }else{
       lcd1.setCursor(13,1);
      lcd1.print("0");
      lcd1.print(timeCoin3%60);   
    }
    if(digitalRead(meniuButton[2]) ==LOW){ 
      pressedCharacter = " ";
        timeCoin3 = 0;
         lcd1.clear();
          lcd1.setCursor(0,0);
  lcd1.print("SetP3Timp/Ron");
     }
    if(digitalRead(meniuButton[1]) ==LOW){
      pressedCharacter = " ";
      EEPROM.put(timeCoinsEeprom3,timeCoin3);
      isOk=6;
      lcd1.clear();
    }
  }
   delay(250);
while(isOk==6){
  delay(50);
 
   lcd1.setCursor(0,0);
  lcd1.print("Pt.Iesire=*Meniu");
  delay(250);
   if(digitalRead(meniuButton[0]) ==LOW){
   
     delay(250);
      isOk1=false;
      isOk=9;
      lcd1.clear();
    }
    
  lcd1.setCursor(0,1);
    lcd1.print("Pt.Setare=*Set");
     lcd.setCursor(0,1);
    lcd.print("Pt.Next=*Conta");
     delay(850);
    lcd1.clear();
    lcd1.setCursor(0,1);
  lcd1.print("SetPret4Timp/Ron");
     if(digitalRead(meniuButton[1]) ==LOW){
      delay(250);
     
      isOk=7;
      lcd1.clear();
    }
     if(digitalRead(meniuButton[2]) ==LOW){
      delay(250);
    
      isOk=0;
      lcd1.clear();
    }
    delay(850);
    lcd1.clear();
}

  //Set Pret4
  lcd1.clear();
  lcd1.setCursor(0,0);
  lcd1.print("SetP4Timp/Ron");
   delay(250);
  while(isOk==7){
    delay(50);
   
     
    if(digitalRead(meniuButton[0]) == LOW){ 
      delay(250);
     
     timeCoin4 = timeCoin4 + 1;
      if(timeCoin4 > 600){
        timeCoin4 = 0;
      }
    }
    if(timeCoin4 / 60 < 10){
    lcd1.setCursor(0,1);
    lcd1.print("1Ron=");
    lcd1.print("0");
    lcd1.print(timeCoin4 / 60);
    lcd1.print("M");
    }else{
       lcd1.setCursor(0,1);
    lcd1.print("1Ron=");
    lcd1.print(timeCoin4 / 60);
    lcd1.print("M");
      }
      lcd1.setCursor(8,1);
    lcd1.print("+");
    lcd1.setCursor(10,1);
    lcd1.print("Sec");
   if((timeCoin4 % 60 ) > 9){
     lcd1.setCursor(13,1);
   lcd1.print(timeCoin4 % 60 );
    }else{
       lcd1.setCursor(13,1);
      lcd1.print("0");
      lcd1.print(timeCoin4 % 60 );   
    }
    if(digitalRead(meniuButton[2]) ==LOW){ 
     
        timeCoin4 = 0;
         lcd1.clear();
          lcd1.setCursor(0,0);
  lcd1.print("SetP4Timp/Ron");
     }
    if(digitalRead(meniuButton[1]) ==LOW){
      pressedCharacter = " ";
      EEPROM.put(timeCoinsEeprom4 , timeCoin4  );
      isOk=8;
      lcd1.clear();
    }
  }
   delay(250);
while(isOk==8){
  
   lcd1.setCursor(0,0);
  lcd1.print("Pt.Next=*MENIU");
  delay(250);
   if(digitalRead(meniuButton[0]) ==LOW){ 
      pressedCharacter = " ";
     delay(250);
     isOk1=false;
      isOk=9;
      lcd1.clear();
    }
    
    lcd1.setCursor(0,1);
    lcd1.print("Pt.Setare=*Set");
     if(digitalRead(meniuButton[1]) ==LOW){ 
      pressedCharacter = " ";
      delay(250);
      isOk=0;
      isOk1=true;
      lcd1.clear();
    }
}
delay(250);
}while(isOk1==true);
lcd1.clear();
}

//Setare limitCoin
void setCoin(){
  // pasCoin =  EEPROM.get(coinPasZone,pasCoin);
    boolean isOk = true;
   
   
     limitCoin =  EEPROM.get(coinLimitZone,limitCoin);
     lcd1.clear();
      delay(300);

while(isOk){
   if(digitalRead(meniuButton[2]) ==LOW){
    lcd1.clear();
      limitCoin=1;
    }
  lcd1.setCursor(0,0);
 lcd1.print ("SetatiLimitaCr :");
  lcd1.setCursor(1,1);
  lcd1.print("CreditMax : ");
  lcd1.print(limitCoin);

//delay(300);
  if(digitalRead(meniuButton[1]) ==LOW){
    //lcd1.clear();
    limitCoin ++;
    delay(100);
    if(limitCoin>200){
      limitCoin=0;
    }
   
    EEPROM.put(coinLimitZone,limitCoin);
    delay(300);
    }
    if(digitalRead(meniuButton[0]) ==LOW){
      pressedCharacter = " ";
      isOk = false;
      lcd1.clear();
      delay(300);
      
    }
}
}


// afisare contabilitate
void displayConta(){
  //int totalTimeCoin;
// rezervare zone de memorie in eeprom
int totalCoinsConta = 10; // zona memorare total bani incasati 
  boolean isOk = true;
while(isOk){
 
  lcd1.clear();
  lcd1.setCursor(0,0);
 lcd1.print ("Total incasare :");
  lcd1.setCursor(1,1);
 totalCoin =  EEPROM.get(totalCoinsConta,totalCoin);
  lcd1.print(totalCoin);
  lcd1.print(" RON");
  delay(600);
  lcd1.clear();
  lcd1.setCursor(0,0);
  lcd1.print("Iesire =>Meniu");
  lcd1.setCursor(0,1);
  lcd1.print("Reset =>SET ");
  delay(600);
  if(digitalRead(meniuButton[1]) ==LOW){
    pressedCharacter = " ";
    lcd1.clear();
    lcd1.print("Stergere conta....");
    EEPROM.put(totalCoinsConta,0);
    }
    if(digitalRead(meniuButton[0]) ==LOW){
      pressedCharacter = " ";
      isOk = false;
      lcd1.clear();
    }
}
}


//Meniu Program
void meniuprog(){
  digitalWrite(inhibitCoin,HIGH);
  //Set Time 
  delay(600);
 
  int meniu = 7;
  lcd1.clear();
  while (meniu == 7) {
     
    meniu = 1;
           delay(600);      
         lcd1.setCursor(0, 0);  
          lcd1.print(" * SetProg/Timp  "  );
          lcd1.setCursor(0, 1); lcd1.print(" AfisConta");delay(600);  
          lcd1.setCursor(1,0); lcd1.blink();
 do{
       
        if (digitalRead(meniuButton[1]) ==LOW) { 
          delay(200);
          lcd1.clear();
          lcd1.noBlink();
          pressedCharacter = " ";
          coin_time();
           meniu = 2;
        } 
        if (digitalRead(meniuButton[0]) ==LOW) 
          {
            delay(200);
            pressedCharacter = " ";
          meniu = 2;
          lcd1.clear();
          }
      }while(meniu==1);

      
       //Display Device
        delay(600);      
         lcd1.setCursor(0, 0);  
          lcd1.print(" * Display Device  "  );
          lcd1.setCursor(0, 1); lcd1.print(" AfisConta");delay(600);  
          lcd1.setCursor(1,0); lcd1.blink();
 do{

        if (digitalRead(meniuButton[1]) ==LOW) { 
          delay(200);
          lcd1.clear();
          lcd1.noBlink();
          pressedCharacter = " ";
         scannerRun();
           meniu = 3;
        } 
        if (digitalRead(meniuButton[0]) ==LOW) 
          {
            delay(200);
            pressedCharacter = "*";
          meniu = 3;
          lcd1.clear();
          }
      }while(meniu==2);

      
      //Display Conta

 delay(400);
    
      lcd1.print( " * AfisConta ");
      lcd1.setCursor(0, 1);
      lcd1.print("  SetCoin");
      lcd1.setCursor(1,0); 
      lcd1.blink();
                                                                         
      
      do {
       
        if (digitalRead(meniuButton[1]) ==LOW) { 
          delay(200);
          lcd1.noBlink();
          pressedCharacter = " ";
            displayConta();
          meniu = 4;
          lcd1.clear();
        }
           delay(200);
        if (digitalRead(meniuButton[0]) ==LOW) {
          delay(200);
          pressedCharacter = " ";
          meniu = 4;
           delay(200);
          lcd1.clear();
        }
      } while (meniu == 3);

//Set CoinPas
 delay(400);
    
      lcd1.print( " * SetCoin ");
      lcd1.setCursor(0, 1);
      lcd1.print("  Exit");
      lcd1.setCursor(1,0); 
      lcd1.blink();


      do {
      
        if (digitalRead(meniuButton[1]) ==LOW) { 
          delay(200);
          lcd1.noBlink();
          pressedCharacter = " ";
            setCoin();
          meniu = 5;
          lcd1.clear();
        }
           delay(200);
        if (digitalRead(meniuButton[0]) ==LOW) {
          delay(200);
          pressedCharacter = " ";
          meniu = 5;
           delay(200);
          lcd1.clear();
        }
      } while (meniu == 4);
      do
      {
      lcd1.print( " * Exit ");
      lcd1.setCursor(0, 1); 
      lcd1.print("   MENIU ");
      lcd1.setCursor(1,0);
      lcd1.blink(); 
      delay(200);
         // pcf8574Run(); //read the keypad
        if (digitalRead(meniuButton[1]) ==LOW) {
         lcd1.noBlink();
         pressedCharacter = " ";
          meniu = 6;
          digitalWrite(inhibitCoin,LOW);
          delay(200);
          lcd1.clear();
        }
        delay(150);
        if (digitalRead(meniuButton[0]) ==LOW) {
            delay(300);
            pressedCharacter = " ";
          meniu = 7;
          lcd1.clear();
         }
      } while (meniu == 5);
  } 
 delay(200);
  digitalWrite(inhibitCoin,LOW);
}



// activare releu vacuumCleaner
void vacuumCleaner(){
elapsedMillis timeDelay; //declare global if you don't want it reset every time loop runs
elapsedMillis timeContor; //declare global if you don't want it reset every time loop runs
elapsedMillis timeElapsed; //declare global if you don't want it reset every time loop runs
// delay in milliseconds between blinks of the LED
   unsigned int interval = 1000;
   float newCoin = 0;
 float newTimeCoin = 0;
boolean isOk =false;
 pressedCharacter = "*";
 programSelector=0;
 //Extract from memory price of coin
   timeCoin1 =  EEPROM.get(timeCoinsEeprom1,timeCoin1);
   timeCoin2 = EEPROM.get(timeCoinsEeprom2,timeCoin2);
   timeCoin3 = EEPROM.get(timeCoinsEeprom3,timeCoin3);
   timeCoin4 = EEPROM.get(timeCoinsEeprom4,timeCoin4);
   //unsigned long returnArray[4];
//Return if coin = 0
   lcd.noBlink();
   if(coin == 0){
     lcd.clear();
    lcd.print(" Credit 0");
    delay(1200);
    lcd.clear();
    return;
       }
     
       lcd.clear();
       while(isOk ==false){
     
       lcd.setCursor(2,0);
  lcd.print("Alegeti Programul");
  lcd.setCursor(8,1);
  lcd.print("Din ");
  lcd.setCursor(2,2);
 lcd.print("Lista de Butoane");
  lcd.setCursor(4,3);
  lcd.print("Credit:");
  lcd.print(coin);
  lcd.print(" RON");
     
    ledDriverRunVaccum();
     
    
        timeDelay = timeDelay +1;
        timeElapsed =timeElapsed+1;
        
       pressedCharacter = pcf8574Run();
       if(!pressedCharacter.equals("*")){
          totalCoin =  EEPROM.get(totalCoins,totalCoin);
          totalCoin = totalCoin+coin;
          EEPROM.put(totalCoins,totalCoin);
         isOk=true;
         programSelector=0;
         timeContor=0;
       }
       
       }
       
       while(isOk==true){
         digitalWrite(inhibitCoin,HIGH);
         pressedCharacter = pcf8574Run();
         Serial.println(pressedCharacter);
         // Set time-price for 
       aspirator( pressedCharacter);
       ledDriverRunVaccum();
    //Set time-price for Parfum
     parfum(pressedCharacter);
    //Set time-price for Cauciuc
      cauciuc(pressedCharacter);
  //Set time-price for AerComprimat
      aerComprimat(pressedCharacter);


   //__End Set time-Price
//Display Time
    lcd.setCursor(0,0);
   switch(programSelector){
     case 1:{
         lcd.print("TimpRamasVaccum:"); 
     }break;
     case 2: {
       lcd.print("TimpRamasParf: ");
     }break;
     case 3 :{
       lcd.print("TimpRamasCauciuc: ");
     }break;
     case 4 :{
       lcd.print("TimpRamasAir: ");
     }break;
   }
  lcd.setCursor(0,1);
   lcd.print("Min :");
   if(minuteRamase>=10){
     lcd.print(minuteRamase);
          }
          else{
            
             lcd.print("0");
            lcd.print(minuteRamase); 
          }      
   lcd.print(" :");
   if(secundeRamase>=10){
    lcd.setCursor(9,1);
     lcd.print(secundeRamase);
   }
   else{
    lcd.setCursor(9,1);
     lcd.print("0");
  lcd.print(secundeRamase);
   }
   
  lcd.print(" Sec");
  if(timeContor>interval){
    if((secundeRamase <=0)&&(minuteRamase>0)){
    minuteRamase--;
   secundeRamase = 59;
    }
    secundeRamase--;
    totalTimeCoin--;
    timeContor =0;
    }
   
    if(totalTimeCoin<=0){
      totalTimeCoin = 0;
      timeContor =0;
      programSelector=0;
      coin = 0;
      isOk = false;
      for(int i=0;i<4;i++){
        
      digitalWrite(pinReel[i],LOW);
      }
      digitalWrite(inhibitCoin,LOW);
    }
  } ;
}

//Set time-price for aspirator
void aspirator( String pressedChar){
    pressedCharacter.equals(pressedChar);
   
     if((pressedCharacter.equals("StartProg1"))&&(programSelector!=1)){
           for(int i=0;i<4;i++){
        
      digitalWrite(pinReel[i],LOW);
      }
       switch(programSelector){
         case 0 :{
            totalTimeCoin = coin*timeCoin1;
     minuteRamase = (coin*timeCoin1)/60;
     secundeRamase =((coin*timeCoin1)%60);
     programSelector=1;
     lcd.clear();
           pressedCharacter="*";
           digitalWrite(pinReel[0],HIGH);
         }break;
         case 2 :{
            newCoin = ((float)totalTimeCoin/(float)timeCoin2);
       totalTimeCoin =  newCoin* timeCoin1;
     minuteRamase = totalTimeCoin/60;
     secundeRamase =totalTimeCoin%60;
      programSelector=1;
      lcd.clear();
      pressedCharacter="*"; 
      coin = 0;
     digitalWrite(pinReel[0],HIGH);
         }break;
      case 3 :{
            newCoin = ((float)totalTimeCoin/(float)timeCoin3);
       totalTimeCoin =  newCoin* timeCoin1;
     minuteRamase = totalTimeCoin/60;
     secundeRamase =totalTimeCoin%60;
      programSelector=1;
      lcd.clear();
      pressedCharacter="*"; 
      coin = 0;
      digitalWrite(pinReel[0],HIGH);
         }break;
        case 4 :{
            newCoin = ((float)totalTimeCoin/(float)timeCoin4);
       totalTimeCoin =  newCoin* timeCoin1;
     minuteRamase = totalTimeCoin/60;
     secundeRamase =totalTimeCoin%60;
      programSelector=1;
      lcd.clear();
      pressedCharacter="*"; 
      coin = 0;
      digitalWrite(pinReel[0],HIGH);
         }break; 
       }
     } 

}
//__end void aspirator 
 //Set time-price for Parfum
 void parfum(String pressChar){
   pressedCharacter.equals(pressChar);
     if((pressedCharacter.equals("ParfSt1"))&&(programSelector!=2)){
          for(int i=0;i<4;i++){
        
      digitalWrite(pinReel[i],LOW);
      }
       switch(programSelector){
         case 0 :{
            totalTimeCoin = coin*timeCoin2;
     minuteRamase = (coin*timeCoin2)/60;
     secundeRamase =((coin*timeCoin2)%60);
     programSelector=2;
     lcd.clear();
           pressedCharacter="*";
           coin = 0;
      digitalWrite(pinReel[1],HIGH);
         }break;
         case 1 :{
            newCoin = ((float)totalTimeCoin/(float)timeCoin1);
       totalTimeCoin =  newCoin* timeCoin2;
     minuteRamase = totalTimeCoin/60;
     secundeRamase =totalTimeCoin%60;
      programSelector=2;
      lcd.clear();
      pressedCharacter="*"; 
      coin = 0;
      digitalWrite(pinReel[1],HIGH);
         }break;
      case 3 :{
            newCoin = ((float)totalTimeCoin/(float)timeCoin3);
       totalTimeCoin =  newCoin* timeCoin2;
     minuteRamase = totalTimeCoin/60;
     secundeRamase =totalTimeCoin%60;
      programSelector=2;
      lcd.clear();
      pressedCharacter="*"; 
      coin = 0;
      digitalWrite(pinReel[1],HIGH);
         }break;
        case 4 :{
            newCoin = ((float)totalTimeCoin/(float)timeCoin4);
       totalTimeCoin =  newCoin* timeCoin2;
     minuteRamase = totalTimeCoin/60;
     secundeRamase =totalTimeCoin%60;
      programSelector=2;
      lcd.clear();
      pressedCharacter="*"; 
      newCoin = 0;
      digitalWrite(pinReel[1],HIGH);
         }break; 
       
       }
     }
 }
 //__end void parfum  

 //Set time-price for Cauciuc
 void cauciuc(String pressChar){
    pressedCharacter.equals(pressChar);
     if((pressedCharacter.equals("CauciucSt1"))&&(programSelector!=3)){
          for(int i=0;i<4;i++){
        
      digitalWrite(pinReel[i],LOW);
      }
       switch(programSelector){
         case 0 :{
            totalTimeCoin = coin*timeCoin3;
     minuteRamase = (coin*timeCoin3)/60;
     secundeRamase =((coin*timeCoin3)%60);
     programSelector=3;
     lcd.clear();
           pressedCharacter="*";
         }break;
         case 1 :{
            newCoin = ((float)totalTimeCoin/(float)timeCoin1);
       totalTimeCoin =  newCoin* timeCoin3;
     minuteRamase = totalTimeCoin/60;
     secundeRamase =totalTimeCoin%60;
      programSelector=3;
      lcd.clear();
      pressedCharacter="*"; 
      newCoin = 0;
      digitalWrite(pinReel[2],HIGH);
         }break;
      case 2 :{
            newCoin = ((float)totalTimeCoin/(float)timeCoin2);
       totalTimeCoin =  newCoin* timeCoin3;
     minuteRamase = totalTimeCoin/60;
     secundeRamase =totalTimeCoin%60;
      programSelector=3;
      lcd.clear();
      pressedCharacter="*"; 
      newCoin = 0;
      digitalWrite(pinReel[2],HIGH);
         }break;
        case 4 :{
            newCoin = ((float)totalTimeCoin/(float)timeCoin4);
       totalTimeCoin =  newCoin* timeCoin3;
     minuteRamase = totalTimeCoin/60;
     secundeRamase =totalTimeCoin%60;
      programSelector=3;
      lcd.clear();
      pressedCharacter="*"; 
      newCoin = 0;
      digitalWrite(pinReel[2],HIGH);
         }break; 
       }
     }
 }
 //__end void cauciuc

//Set time-price for AerComprimat
 void aerComprimat(String pressChar){
      pressedCharacter.equals(pressChar);
     if((pressedCharacter.equals("AerCompSt1"))&&(programSelector!=4)){
       for(int i=0;i<4;i++){
        
      digitalWrite(pinReel[i],LOW);
      }
       switch(programSelector){
         case 0 :{
            totalTimeCoin = coin*timeCoin4;
     minuteRamase = (coin*timeCoin4)/60;
     secundeRamase =((coin*timeCoin4)%60);
     programSelector=4;
     lcd.clear();
           pressedCharacter="*";
           coin = 0;
      digitalWrite(pinReel[3],HIGH);
         }break;
         case 1 :{
            newCoin = ((float)totalTimeCoin/(float)timeCoin1);
       totalTimeCoin =  newCoin* timeCoin4;
     minuteRamase = totalTimeCoin/60;
     secundeRamase =totalTimeCoin%60;
      programSelector=4;
      lcd.clear();
      pressedCharacter="*"; 
     newCoin = 0;
      digitalWrite(pinReel[3],HIGH);
         }break;
      case 2 :{
            newCoin = ((float)totalTimeCoin/(float)timeCoin2);
       totalTimeCoin = newCoin* timeCoin4;
     minuteRamase = totalTimeCoin/60;
     secundeRamase =totalTimeCoin%60;
      programSelector=4;
      lcd.clear();
      pressedCharacter="*"; 
      newCoin = 0;
     digitalWrite(pinReel[3],HIGH);
         }break;
        case 3 :{
           newCoin = ((float)totalTimeCoin/(float)timeCoin3);
       totalTimeCoin = newCoin* timeCoin4;
     minuteRamase = totalTimeCoin/60;
     secundeRamase =totalTimeCoin%60;
      programSelector=4;
      lcd.clear();
      pressedCharacter="*"; 
     newCoin = 0;
      digitalWrite(pinReel[3],HIGH);
         }break; 
       }
     }
 }
 //__end void aerComprimat
