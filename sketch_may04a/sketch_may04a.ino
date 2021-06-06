//+------------------------------------------------------------------------------+
//|                                                    Fussball Cup 'GymThun2021'|
//|                                             Copyright 2021,Viacheslav Zhbanko| 
//|                                                            Gymnasium Thun G23|
//+------------------------------------------------------------------------------+
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
//Library to control LED see http://wayoda.github.io/LedControl/pages/software
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
int Psegm = 2; //Segment position
int Xpos  = 0; //Position in X axis
int Ypos  = 0; //Position in Y axis
int Xdir  = 3; //Direction in X axis
int Ydir  = 7; //Direction in Y axis
int Tloop = 200;
bool Opt_Serial = false; //option to enable serial monitor
bool Opt_ExLed = true;   //option to use extra led
//============================================
// Setup Module: code runs 1x at start
//============================================
void setup() {
  //initialize HW pins 
  pinMode(2,OUTPUT);         //set D2 pin as Output
  digitalWrite(2,HIGH);      //activate D2 to control Joystick
  //set options
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
  //create doors in playground
  makeDoors();
  
}
//============================================
// End of Initialization Module
//============================================

//============================================
// Loop Module: code runs continuously
//============================================
void loop() { 
  
  //Refresh status of Joystick
  Xaxis = analogRead(A0)/128; //value from 0 to 7, default 3
  Yaxis = analogRead(A1)/128; //value from 0 to 7, default 4
  
  //Enable diagnostics via Serial Print (top right corner)
  if(Opt_Serial){
    Serial.println(" XAxis: "); Serial.print(Xaxis);
    //Serial.println(" YAxiSerial.print(Yaxis);
  }
  
  //Change position of point if needed
  Xdir = moveDir(Xaxis, Xdir);
  Ydir = moveDir(Yaxis, Ydir);

  //Change position of segment if needed
  Psegm = jumpSegment(Yaxis, Psegm);
  
  //Set state of LED
  lc.setLed(Psegm,Xdir,Ydir,true);
  //delay
  delay(Tloop);
  lc.setLed(Psegm,Xdir,Ydir,false);

  //check of goal
  litLed(Xdir, Ydir, Psegm);

  //Refresh doors
  makeDoors();
  
}
//============================================
// End of Loop Module code runs continuously
//============================================



//============================================
// User Defined Functions
//============================================

//-------------
// Function: jumpSegment(int xpos, int segment)
//-------------
int jumpSegment(int xpos, int segment){
 //@Purpose: Jump to next segment when directing joystick on Y axis  
 //@xpos    integer, input from Joystic, typically from 0 to 7
 //@segment integer, provided current position of segment, typically from 0 to 3
 //@Return: Value from 0 to 3
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
//-------------
// End Of Function: jumpSegment(int xpos, int segment)
//-------------
//-------------
// Function: moveDir (int AxPos, int oldpos)
//-------------
int moveDir (int AxPos, int oldpos) {
 //@Purpose: defines new position of point on X or Y axis based on Joystick position and old position
 //@AxPos  integer, input from Joystic, typically from 0 to 7 
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
//-------------
// End of Function: moveDir (int AxPos, int oldpos)
//-------------
//-------------
// Function: litLed(int xPos, int yPos, int zSegm)
//-------------
void litLed(int xPos, int yPos, int zSegm) {
 //@Purpose: change status of extra LED 
 //@xPos  integer, position on x axis
 //@yPos  integer, position on y axis
 //@zSegm integer, position of segment
 //@Return: Lit LED only when point is on extreme edges and within doors
  if(zSegm == 0) {
      if((xPos >= 2 && xPos <= 5) && yPos == 7) {digitalWrite(3,HIGH);} else {digitalWrite(3,LOW);} 
    } else if(zSegm == 3) {
      if((xPos >= 2 && xPos <= 5) && yPos == 0) {digitalWrite(3,HIGH);} else {digitalWrite(3,LOW);}
    } else {digitalWrite(3,LOW);}
   
  }
//-------------
// End of Function: litLed(int xPos, int yPos, int zSegm)
//-------------
//-------------
// Function: makeDoors()
//-------------
void makeDoors(){
 //@Purpose: Creates doors on the edges of LED playzone 
 //@Return:  Function lit LED to represent doors
  //Door on segment 0
  lc.setLed(0,7,7,true);
  lc.setLed(0,6,7,true);
  lc.setLed(0,1,7,true);
  lc.setLed(0,0,7,true);
  //Door on segment 3
  lc.setLed(3,7,0,true);
  lc.setLed(3,6,0,true);
  lc.setLed(3,1,0,true);
  lc.setLed(3,0,0,true);
  }  
//-------------
// End of Function: makeDoors()
//-------------
  
//============================================
// End Of User Defined Functions
//============================================
