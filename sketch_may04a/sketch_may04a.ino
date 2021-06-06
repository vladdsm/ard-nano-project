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
/*
   LCD Panel                                    X
      §ΩΩΩΩΩΩΩΩ§ΩΩΩΩΩΩΩΩ§ΩΩΩΩΩΩΩΩ§ΩΩΩΩΩΩΩΩ§     7
 =====§ΩΩΩΩΩΩΩΩ§ΩΩΩΩΩΩΩΩ§ΩΩΩΩΩΩΩΩ§ΩΩΩΩΩΩΩΩ§     6
 =====§ΩΩΩΩΩΩΩΩ§ΩΩΩΩΩΩΩΩ§ΩΩΩΩΩΩΩΩ§ΩΩΩΩΩΩΩΩ§     5
 =====§ΩΩΩΩΩΩΩΩ§ΩΩΩΩΩΩΩΩ§ΩΩΩΩΩΩΩΩ§ΩΩΩΩΩΩΩΩ§     4
 =====§ΩΩΩΩΩΩΩΩ§ΩΩΩΩΩΩΩΩ§ΩΩΩΩΩΩΩΩ§ΩΩΩΩΩΩΩΩ§     3
 =====§ΩΩΩΩΩΩΩΩ§ΩΩΩΩΩΩΩΩ§ΩΩΩΩΩΩΩΩ§ΩΩΩΩΩΩΩΩ§     2
      §ΩΩΩΩΩΩΩΩ§ΩΩΩΩΩΩΩΩ§ΩΩΩΩΩΩΩΩ§ΩΩΩΩΩΩΩΩ§     1
      §ΩΩΩΩΩΩΩΩ§ΩΩΩΩΩΩΩΩ§ΩΩΩΩΩΩΩΩ§ΩΩΩΩΩΩΩΩ§     0
    Y  76543210 76543210 76543210 76543210 
segments  0         1        2        3
----------

   Joystick
 =====§ΩΩΩΩΩΩΩΩ§
 =====§ΩΩΩΩΩΩΩΩ§
 =====§ΩΩΩΩΩΩΩΩ§
 =====§ΩΩΩΩΩΩΩΩ§
 =====§ΩΩΩΩΩΩΩΩ§

axis   ◀ Y ►
         △
axis     X
         ▽
*/
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
int Xpos  = 0; //Position in X axis
int Ypos  = 0; //Position in Y axis
int Xdir  = 7; //Direction in X axis
int Ydir  = 7; //Direction in Y axis
int Tloop = 200;
bool Opt_Serial = false; //option to enable serial monitor
bool Opt_ExLed = true;   //option to use extra led
//============================================
// Setup Module: code runs 1x at start
//============================================


void setup() {
   
  pinMode(2,OUTPUT);         //set D2 pin as Output
  digitalWrite(2,HIGH);      //activate D2 to control Joystick
  if(Opt_ExLed) { pinMode(3,OUTPUT);
                  digitalWrite(3,HIGH);      //test to activate LED
                  delay(3000);
                  digitalWrite(3,LOW);}
  if(Opt_Serial) Serial.begin(9600);        //activate serial monitor, use Serial.print(var) to view status
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
// Loop Module: code runs continuously
//============================================

void loop() { 
  Xaxis = analogRead(A0)/128; //value from 0 to 7, default 3
  //Serial.println(" XAxis: "); Serial.print(Xaxis);
  Yaxis = analogRead(A1)/128; //value from 0 to 7, default 4
  //Serial.println(" YAxis: "); Serial.print(Yaxis);
  
  //Change position of point if needed
  Xdir = moveDir(Xaxis, Xdir);
  Ydir = moveDir(Yaxis, Ydir);

  //Change position of segment if needed
  Psegm = jumpSegment(Yaxis, Psegm);
  
  //set state of LED
  lc.setLed(Psegm,Xdir,Ydir,true);
  //delay
  delay(Tloop);
  lc.setLed(Psegm,Xdir,Ydir,false);

 
  
}
//============================================
// End of Loop Module code runs continuously
//============================================



//============================================
// User Defined Functions
//============================================
int jumpSegment(int xpos, int segment){
 //@Purpose: Jump to next segment when keeping joystick on the edge of the module  
 //@xpos    integer, input from Joystic
 //@segment integer, provided current position of segment
 //@Return: Function returns integer value from 0 to 3
 int result = 0;
 
// //moving right
// if(xpos == 0 && segment == 0) {result = 0; delay(100);} 
// else if(xpos == 0 && segment == 1) {result = 0; delay(100);}
// else if(xpos == 0 && segment == 2) {result = 1; delay(100);}
// else if(xpos == 0 && segment == 3) {result = 2; delay(100);}
// //moving left
// else if(xpos == 7 && segment == 0) {result = 1; delay(100);}
// else if(xpos == 7 && segment == 1) {result = 2; delay(100);}
// else if(xpos == 7 && segment == 2) {result = 3; delay(100);}
// else if(xpos == 7 && segment == 3) {result = 3; delay(100);}
// else result = segment;
 
 return(result); 

}

int moveDir (int AxPos, int oldpos) {
 //@Purpose: defines new position of point on X axis based on Joystic position and old position
 //@AxPos  integer, input from Joystic
 //@oldpos integer, provided old position
 //@Return: Function returns integer value from 0 to 7
 int result = 7;
 if(AxPos == 0 && oldpos == 7) {result = 7;}
 else if(AxPos == 7 && oldpos == 0) {result = 0;}
 else if(AxPos == 0) {result = oldpos + 1;}
 else if(AxPos == 7) {result = oldpos - 1;}
 else result = oldpos;
 return(result);
}

//============================================
// End Of User Defined Functions
//============================================
