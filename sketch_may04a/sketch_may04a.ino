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
int Xaxis = 0; //Joystick X Asis
int Yaxis = 0; //Joystick Y Axis
int Psegm = 0; //Segment position

//============================================
// Loop Module code runs continuously
//============================================

void loop() { 
  Yaxis = analogRead(A0)/128; //value from 0 to 7, default 4
  Serial.print(" YAxis: "); Serial.print(Yaxis);
  Xaxis = analogRead(A1)/128; //value from 0 to 7, default 3
  Serial.print(" XAxis: "); Serial.print(Xaxis);
  Psegm = jumpSegment(Xaxis);
  
  lc.setLed(Psegm,Xaxis,Yaxis,true);
  delay(100);
  lc.setLed(Psegm,Xaxis,Yaxis,false);

 
  
}
//============================================
// End of Loop Module code runs continuously
//============================================



//============================================
// User Defined Functions
//============================================
int jumpSegment(int xpos){
 //@Purpose: Jump to next segment when keeping joystick on the edge of the module  
 //@Return: Function returns integer value from 0 to 3
 int result = 0;
 if(xpos < 1 && result == 0) result = 0;
 if(xpos > 6 && result == 0) result = 1;
 if(xpos > 6 && result == 1) result = 2;
 if(xpos > 6 && result == 2) result = 3;
 if(xpos > 6 && result == 3) result = 3;
 
 
 return(result); 

  
}

//============================================
// End Of User Defined Functions
//============================================
