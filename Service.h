
int meniuButton[3] = {5,6,7}; // Meniu,Set, Conta

 void buttonInit()
 {
  for(int i=0;i<3;i++){
    pinMode(meniuButton[i], INPUT_PULLUP);
    delay(50);
  }
 }