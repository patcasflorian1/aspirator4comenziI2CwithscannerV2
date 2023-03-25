#include <Wire.h> //include Wire.h library
 void scannerInit(){
  Wire.begin(); // Wire communication begin
  Serial.begin(9600); // The baudrate of Serial monitor is set in 9600
  while (!Serial); // Waiting for Serial Monitor
  Serial.println("\nI2C Scanner");
  lcd1.begin(16, 2);
  lcd1.setBacklight(HIGH); 
  lcd1.setCursor(0, 0);
  lcd1.print("  \nI2C Scanner");
  delay(500);
  lcd1.clear();
 }
 
 void scannerRun(){
  int test = 0;
  while(test==0){
  
 lcd1.print("Scanning...");
  byte error, address; //variable for error and I2C address
  int nDevices;
delay(400);
  Serial.println("Scanning...");
 lcd1.clear();
  nDevices = 0;
  for (address = 1; address < 127; address++ )
  {
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0)
    {
      
      Serial.print("I2C device found at address 0x");
      if (address < 16)
        Serial.print("0");
      Serial.print(address, HEX);
      Serial.println("  !");
      // lcd.setCursor(0, 0);
  lcd1.print(address, HEX);
      
      nDevices++;
    }
    else if (error == 4)
    {
      Serial.print("Unknown error at address 0x");
      if (address < 16){
        Serial.print("0");
      Serial.println(address, HEX);
      lcd1.print(address, HEX);
      }
    }
  }
  if (nDevices == 0){
    Serial.println("No I2C devices found\n");
    lcd1.print("No I2C devices found\n");
  }
  else{
    Serial.println("done\n");
  }

  delay(2500); // wait 5 seconds for the next I2C scan
  lcd1.clear();
   //pcf8574Run(); //read the keypad
   
        if (digitalRead(meniuButton[1]) ==LOW) // Set
        { 
          delay(200);
          lcd1.clear();
           test=1;
        }
        
  }
 }
