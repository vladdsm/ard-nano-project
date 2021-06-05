//+------------------------------------------------------------------------------+
//|                                                     Punkt auf Matrix bewegen:|
//|                                             Copyright 2021,Viacheslav Zhbanko| 
//|                                                            Gymnasium Thun G23|
//+------------------------------------------------------------------------------+
//Include the library to control LED see http://wayoda.github.io/LedControl/pages/software
//
//============================================
// Hardware definition
//============================================
// see: 
//
//============================================
// Include libraries
//============================================
#include "LedControl.h"
//============================================
// Properties definition
//============================================
/*
 Now we need a LedControl to work with.
 ***** Control the Pins! *****
 pin 12 is connected to the DataIn 
 pin 11 is connected to the CLK 
 pin 10 is connected to LOAD 
 We have 4 cascaded modules MAX72XX.
 */
LedControl lc=LedControl(12,11,10,4);

//============================================
// Global Variables definition
//============================================
int Xaxis = 0; //Joystick X Asis
int Yaxis = 0; //Joystick Y Axis
int Psegm = 0; //Segment position

int Tloop = 200;
//============================================
// Initialization Module code runs 1x at start
//============================================


void setup() {
   
  pinMode(2,OUTPUT);         //set D2 pin as Output
  digitalWrite(2,HIGH);      //activate D2 to control Joystick
  Serial.begin(9600);        //activate serial monitor, use Serial.print(var) to view status
  //initialize display
  for(int i=0;i<4;i++){
    lc.shutdown(i,false);
    lc.setIntensity(i,2);
    lc.clearDisplay(i);
  }
  
}
//============================================
// End of Initialization Module
//============================================



//============================================
// Loop Module code runs continuously
//============================================

void loop() { 
  Xaxis = analogRead(A1)/128; //value from 0 to 7, default 3
  //Serial.println(" XAxis: "); Serial.print(Xaxis);
  Yaxis = analogRead(A0)/128; //value from 0 to 7, default 4
  //Serial.println(" YAxis: "); Serial.print(Yaxis);

  Psegm = jumpSegment(Yaxis, Psegm);
  delay(100);

  //set state of LED
  lc.setLed(Psegm,Xaxis,Yaxis,true);
  //delay
  delay(Tloop);
  lc.setLed(Psegm,Xaxis,Yaxis,false);

 
  
}
//============================================
// End of Loop Module code runs continuously
//============================================



//============================================
// User Defined Functions
//============================================
int jumpSegment(int xpos, int segment){
 //@Purpose: Jump to next segment when keeping joystick on the edge of the module  
 //@Return: Function returns integer value from 0 to 3
 int result = 0;
 
 //moving right
 if(xpos == 0 && segment == 0) {result = 0; delay(100);} 
 else if(xpos == 0 && segment == 1) {result = 0; delay(100);}
 else if(xpos == 0 && segment == 2) {result = 1; delay(100);}
 else if(xpos == 0 && segment == 3) {result = 2; delay(100);}
 //moving left
 else if(xpos == 7 && segment == 0) {result = 1; delay(100);}
 else if(xpos == 7 && segment == 1) {result = 2; delay(100);}
 else if(xpos == 7 && segment == 2) {result = 3; delay(100);}
 else if(xpos == 7 && segment == 3) {result = 3; delay(100);}
 else result = segment;
 
 return(result); 

}



//============================================
// End Of User Defined Functions
//============================================
