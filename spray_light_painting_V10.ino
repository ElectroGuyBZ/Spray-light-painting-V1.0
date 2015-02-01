/* Spray light painting V1.0
 * writted by Mickael Captant aka "ElectroGuy" from BulbZone
 * 01/02/2015
 */
/*Revisions
 *  V1.0
 * 		- Add comments
 *		- Separate NeoPixels
 *  V2.0
 *              - coming soon
 */
///////////////////////////////////////////////////////////////
//	  Libraries used
#include <Encoder.h> 
#include <Adafruit_NeoPixel.h>
///////////////////////////////////////////////////////////////
//        Define variable
//	Define encoder Pins
Encoder rotary(2, 3);

//	Define encoder's push button pin 
const int rotaryPush = 12;

//	Define spray button pin
const int sprayButton = 11;
//	Define NeoPixels pins
#define NEO1 5
#define NEO2 9
//	Attach variable to NeoPixels
Adafruit_NeoPixel ring = Adafruit_NeoPixel(16, NEO1, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel jewel = Adafruit_NeoPixel(7, NEO2, NEO_GRB + NEO_KHZ800);

//  Variable for position of the Led ON (ring)
int ringPosition;
int ringCalibration;

//  Variable for encoder's push button
int rotaryPushState = 0;         // current state of the button
int lastRotaryPushState = 0;     // previous state of the button
int rotaryPushDelay = 0;         // increment when button is pressed
int countRotaryPush = -1;
int pushCounter = 0;

//  Variable for encoder's push button
long oldRotaryPosition  = -999;
long newRotaryPosition;

//  Variable for spray button
int sprayButtonState = 0;

// Variable for ring RGB 
int ringR=0;
int ringG=0;
int ringB=0;

// Variable for jewel RGB 
int jewelR=0;
int jewelG=0;
int jewelB=0;

//variable for reset counter
int countMode = 0;

///////////////////////////////////////////////////////////////
	//  Define function
      //	turn the encoder to select color you whant
      //	second click to confirm the color
      //	push the spray button
      //	enjoy !
void action(){
  //  Define function
  //  MODE: encoder color + Jewel
  //  1 CLICK = encoder color 
  //  2 CLICK = spray
  //  3 CLICK = return / RGB 0

  if(countRotaryPush == 0){
      rainbow(20);
    }
  if(countRotaryPush == 2){
    makeColors(0, 0, 0, 0);
    if(sprayButtonState == LOW)
    {
      makeColors(1, ringR, ringG, ringB);
      delay(100);
    }
    else{
      makeColors(1, 0, 0, 0);
    }
  }
    countPush();
    pushDelay();
}
///////////////////////////////////////////////////////////////
//		change color of a kind of neopixel with one command:
//		makeColors(NeoPixel,red value, green value, blue value);
void makeColors(int Neo,int R, int G, int B){
  
  if (Neo == 0) // define color of NeoRing
  {
    for(int x=0; x<=15;x++)
    {
      ring.setPixelColor(x, ring.Color(R, G, B));
      ring.show();
    }
  }
  else if (Neo == 1) // define color of NeoJewel
  {
    for(int x=0; x<=6; x++)
    {
      jewel.setPixelColor(x, jewel.Color(R, G, B));
      jewel.show();
    }
  }
}
/////////////////////////////////////////////////////////////////
//		modified version of rainbow function from Adafruit's neopixel library
void rainbow(uint8_t wait) {
  uint16_t i;
  for(i=0; i<=15; i++) {
  //		the encoder's counter changes the value of the color
    ring.setPixelColor(i, Wheel((i+newRotaryPosition) & 255));
  }
  ring.show();
  delay(wait); //define frequency of the change
}
//////////////////////////////////////////////////////////////////
//		modified version of wheel function from Adafruit's neopixel library
// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  if(WheelPos < 85) {
   ringR = WheelPos*3;
   ringG = 255 - WheelPos * 3;
   ringB = 0;
   return ring.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  }
  else if(WheelPos < 170) {
   WheelPos -= 85;
   ringR = 255 - WheelPos * 3;
   ringG = 0;
   ringB = WheelPos * 3;
   return ring.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } 
  else {
   WheelPos -= 170;
   ringR = 0;
   ringG = WheelPos * 3;
   ringB = 255 - WheelPos * 3;
   return ring.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}
/////////////////////////////////////////////////////////////////
//		define number of push for reset the mode
void countPush(){
    countMode = 4;
  //		reset variables and return to main mode  
  if(countRotaryPush == countMode){
    resetAll();
  } 
}
///////////////////////////////////////////////////////////////
//              Reset all the variables
void resetAll(){
  makeColors(0, 0, 0, 0);
  makeColors(1, 0, 0, 0);
  countRotaryPush=0;
  pushCounter=0;
  oldRotaryPosition=-999;
  rotary.write(0);
  jewelR=0;
  jewelG=0;
  jewelB=0;
  ringR=0;
  ringG=0;
  ringB=0;
}
///////////////////////////////////////////////////////////////
//		read different sensors and stock values in variables
void readSensors(){
  // read the state of the rotary push button value:
  rotaryPushState = digitalRead(rotaryPush);
  // read the state of the joystick button value:
  sprayButtonState = digitalRead(sprayButton);
  // read the state of the encoder:
  newRotaryPosition = rotary.read();
  if (lastRotaryPushState != rotaryPushState) {     
   countRotaryPush++;
  }
}
///////////////////////////////////////////////////////////////
//		return mode when push button count is between 150 and 200
void pushDelay(){
  if(rotaryPushState == LOW){
    rotaryPushDelay++;
    pushCounter++;
    delay(50);
  }
  else{
    rotaryPushDelay = 0;
  }
  
  if(rotaryPushDelay >150 && rotaryPushDelay <= 200 && countRotaryPush != 0 && countRotaryPush > 0){
    countRotaryPush = countRotaryPush -2;
    rotaryPushDelay = 0;
    pushCounter = 0;
  }
}
///////////////////////////////////////////////////////////////
//		update color of the ring with the encoder
void RingPosition(){
  if (newRotaryPosition != oldRotaryPosition) {
    ringCalibration = newRotaryPosition / 15;
    if(newRotaryPosition < 0){
      for(int x = 0; x <= 15; x++){
    ring.setPixelColor(x, ring.Color(255,0,0));
      }
    } 
    if(newRotaryPosition >15)
    {
    ringPosition = newRotaryPosition - (15 * ringCalibration);
    }
    else{  
    ringPosition = newRotaryPosition;
    }
  }
}
//////////////////////////////////////////////////////////////
//		make a sequence when the spray start
void start(){
  
  for(int x=0;x<=7;x++){
    jewel.setPixelColor(x,jewel.Color( 255,255,0));
    jewel.setBrightness(15*x);
    jewel.show();
    delay(60);
  }
  delay(100);
  for(int x=7;x>=0;x--){
    jewel.setPixelColor(x,0);
    jewel.setBrightness(15*x);
    jewel.show();
    delay(60);
  }
  jewel.setBrightness(255);
  ring.setBrightness(255);
}
  
///////////////////////////////////////////////////////////////
//	SETUP
void setup() {
  Serial.begin(9600);
  // initialize the pushbuttons pins as an input:
  pinMode(rotaryPush, INPUT);
  pinMode(sprayButton, INPUT_PULLUP);
  //	initialize NeoPixels
  ring.begin();
  ring.show(); // Initialize all pixels to 'off'
  jewel.begin();
  jewel.show(); // Initialize all pixels to 'off'
  start();

 
}
///////////////////////////////////////////////////////////////
//		MAIN LOOP
void loop() {
	//		read values of sensors and stock in data
	readSensors();
	//		read values of push button and increment delay counter
	pushDelay();
	//		Go to the main menu 
	action();
	//		update RotaryPush state
	lastRotaryPushState = rotaryPushState;
}
