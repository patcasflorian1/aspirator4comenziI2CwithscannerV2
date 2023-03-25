volatile int coinPin = 3;
int inhibitCoin = 4; 
long timeCoinMill = 0;
int limitCoin ;
int coin = 0;
bool insert = false;
//int pasCoin = 0;
int coinPasZone = 310,coinLimitZone = 320;
/*

*/
 //interrupt
void coinInterrupt() {
//  pasCoin =  EEPROM.get(coinPasZone,pasCoin);
  limitCoin =  EEPROM.get(coinLimitZone,limitCoin);
 
while(digitalRead(coinPin) == LOW){
    timeCoinMill++;
}
  if(timeCoinMill>0){ 
 
    
     if(coin>limitCoin){
      digitalWrite(inhibitCoin,HIGH);
     }
     else{
      digitalWrite(inhibitCoin,LOW);
       coin ++ ;
     }
  // Serial.println(coin);
 // Serial.println();
      insert = true;
}
  }
