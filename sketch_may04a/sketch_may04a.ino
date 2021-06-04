//Punkt auf Matrix bewegen:

//We always have to include the library
#include "LedControl.h"
// http://wayoda.github.io/LedControl/pages/software


/*
 Now we need a LedControl to work with.
 ***** Control the Pins! *****
 pin 12 is connected to the DataIn 
 pin 11 is connected to the CLK 
 pin 10 is connected to LOAD 
 We have only a single MAX72XX.
 */
LedControl lc=LedControl(12,11,10,4);

int zeit = 200;
//============================================
// Initialization Module code runs 1x at start
//============================================


void setup() {
  
  pinMode(A1,INPUT);
  pinMode(2,OUTPUT);
  digitalWrite(2,HIGH);
  Serial.begin(9600);
  for(int i=0;i<4;i++){
    lc.shutdown(i,false);
    lc.setIntensity(i,8);
    lc.clearDisplay(i);
  }
  
}
//============================================
// End of Initialization Module
//============================================

//============================================
// Global Variables definition
//============================================

int Zahl =0;


//============================================
// Loop Module code runs continuously
//============================================

void loop() { 
  Zahl = analogRead(A0)/128;
  
  lc.setLed(0,0,Zahl,true);
  delay(100);
  lc.setLed(0,0,Zahl,false);
}
//============================================
// End of Loop Module code runs continuously
//============================================



//============================================
// User Defined Functions
//============================================



//============================================
// End Of User Defined Functions
//============================================
