// lightserver.ino
//
// This sketch listens on the I2C "wire" bus for commands.
// Upon receiving a command, it does some things with LED lights.
//
// The wire "slave" (listener) function is based on the "Slave Receiver" example sketch (see File/Examples/Wire/Slave Receiver)
// Background info at: https://docs.arduino.cc/learn/communication/wire
//

// The Wire Library implements the I2C communiation protocol between a 
// controller ("master") and a peripheral ("slave")
#include <Wire.h>

// the SLAVE_ADDRESS must be defined the same on the master and the slave,
// so that the slave will know that the master is trying to communicate with it
// as opposed to some other slave device on the same wire

const int SLAVE_ADDRESS = 100;

// define the commands we'll send down the wire
// the master and slave need to agree on what these are!
const char COMMAND1 = 1;
const char COMMAND2 = 2;
const char COMMAND3 = 3;
const char COMMAND4 = 4;

// Example code for controlling the LED strip can be found in
// Exampes/Adafruit NeoPixel/...

// the Adafruit_NeoPixel library is used to control our LED strips
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif
#include <time.h>

// define the pin that the LED strip is connected to
#define LED_STRIP_PIN 6

// define the length of the LED strip (number of pixels)
#define NUM_PIXELS 150

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_PIXELS, LED_STRIP_PIN, NEO_GRB + NEO_KHZ800);

// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.


void setup() {
  Wire.begin(SLAVE_ADDRESS);    // join i2c bus, using the specified address as ours
  Wire.onReceive(receiveEvent); // register event
  Serial.begin(9600);           // start serial for debugging output
  Serial.println("Setting up");

  // now initialize the LED strip
  strip.begin();
  strip.setBrightness(50);
  strip.show(); // Initialize all pixels to 'off'
  allPurple();
}

void loop() {
  // we don't actually do anything directly here in the loop().
  // instead, we wait for commands to come in; when they do,
  // the receiveEvent() function will be called (since it was
  // set up that way in our Wire.onReceive(receiveEvent) call in setup() above
}

volatile boolean stop_animation = false;

// this function executes whenever data is received from master
// this function is registered as an event, see setup()
void receiveEvent(int howMany) {
  int command = Wire.read(); // receive the command (a single byte as a character)
  stop_animation = true; // cancel any previously running animation
//  char secondcommand = Wire.read();
  Serial.print("howMany: ");
  Serial.println(howMany);
  Serial.print("recieved: ");
  Serial.println(command);
//  Serial.println(secondcommand);
  clear();
//   if (command == COMMAND1) {
//    headlights();
//    } else if (command == COMMAND2) {
//      break();
//    } else if (command == COMMAND3) {
//      left_signal();
//    } else if (command == COMMAND4) {
//      right_signal();
//    }
  create_headlights();
  if (command == COMMAND1) {
    light_display(false, false, true, false);
  } else if (command == COMMAND2) {
    setLights(true,true,true);
  } else if (command == COMMAND3) {
    left_signal();
  } else if (command == COMMAND4) {
    right_signal();
  }
  strip.show();
}

void setLights(bool brake, bool left, bool right) {
  if (brake==true) {
    for (int i = 49; i < 54; i ++){
      strip.setPixelColor(i, 255, 0, 0);
    }
    for (int i = 56; i < 61; i++){
    strip.setPixelColor(i, 255, 0, 0);
    }
  }
  if (left==true){
    for(int i = 18; i < 26; i++){
      strip.setPixelColor(i,255, 30, 0);
    }
 
    for(int i = 40; i < 48; i++){
      strip.setPixelColor(i,255, 30, 0);
    }
  }
  if (right==true){
    for(int i = 0; i < 4; i++){
      strip.setPixelColor(i,255, 30, 0);
    }
    for(int i = 85; i < 89; i++){
      strip.setPixelColor(i,255, 30, 0);
    }
    for(int i = 62; i < 70; i++){
      strip.setPixelColor(i,255, 30, 0);
    }
  }
}

void light_display(bool green_flash, bool redTrail, bool blueline, bool rainbow){
  if (green_flash){
     greenflash();
  }else if(redTrail){
    redtrail();
  }else if (blueline){
    BlueLine();
  }else if (rainbow){
    rainbOw();
  }
}



void rainbOw(){
  
}

void BlueLine(){
  int num = 5;
  for (int i = 0;i<89; i++){
    for (int i = num-5; i < num; i++){
      strip.setPixelColor(i, 0,0, 255);
      strip.show();
    }
    delay(20);
    for  (int i = 0; i<100; i++){
      strip.setPixelColor(i, 0, 0, 0);
    }
    strip.show();
    num = num+1;
  }
}

void redtrail(){
  for (int i = 0; i < 4; i++){
    for (int i = 0; i < 89; i ++){
      strip.setPixelColor(i, 255,0, 0);
      strip.show();
      delay(10);
    }
    for (int i = 0; i < 89; i ++){
      strip.setPixelColor(i, 0,0, 0);
      strip.show();
      delay(10);
    }
  }
}

void greenflash(){
  for (int i=0;i<4;i++){
    for  (int i = 0; i<89; i++){
      strip.setPixelColor(i, 0, 255, 0);
    }
    strip.show();
    delay(500);
    for  (int i = 0; i<89; i++){
      strip.setPixelColor(i, 0, 0, 0);
    }
    strip.show();
    delay(500);
  }  
}







void create_headlights() {
    headlights();
}

void brake_lights() {
  brake();
}

void allBlue() {
  left_signal();
}

void allPurple() {
  allSameColor(255, 0, 255);
}

void allBlack() {
  allSameColor(0, 0, 0);
}

void allSameColor(int red, int green, int blue) {
  for(int i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, strip.Color(red, green, blue));
  }
  strip.show();
}

boolean delayUnlessInterrupted(int delay_milliseconds) {
  // this function will delay for the specified delay_milliseconds
  // and then return false.
  // However, if at some point during this delay,
  // stop_animation is set to true, then this function
  // will immediately be interrupted and return true.
  
  unsigned long start_millis = millis();
  while (true) {
    if (stop_animation) {
      return true;
    }
    if ((millis() - start_millis) >= delay_milliseconds) {
      return false;
    }
  }
}

void flashGreen() {
  stop_animation = false; // allow this animation to run forever -- until stop_animation is set to true
  while (1) {
    //allGreen();
    if (delayUnlessInterrupted(100) == true) {
      return;
    }
    allBlack();
    if (delayUnlessInterrupted(100) == true) {
      return;
    }
  }
}

void headlights(){

  for(int i =5;i<10; i++){
    strip.setPixelColor(i,150, 100, 0);
  }
  for(int i=12; i<17; i++){
    strip.setPixelColor(i, 150, 100, 0);
  }
  
}

void brake(){
 
  for (int i = 49; i < 54; i ++){
    strip.setPixelColor(i, 255, 0, 0);
  }
  for (int i = 56; i < 61; i++){
    strip.setPixelColor(i, 255, 0, 0);
  }
  
}

void left_signal(){
  
  for(int i = 18; i < 26; i++){
    strip.setPixelColor(i,255, 30, 0);
  }
 
  for(int i = 40; i < 48; i++){
    strip.setPixelColor(i,255, 30, 0);
  }
}

void right_signal(){
  
  for(int i = 0; i < 4; i++){
    strip.setPixelColor(i,255, 30, 0);
  }
  for(int i = 85; i < 89; i++){
    strip.setPixelColor(i,255, 30, 0);
  }
  for(int i = 62; i < 70; i++){
    strip.setPixelColor(i,255, 30, 0);
  }
  
}



void clear(){
  for(int i=0; i<strip.numPixels();i++){
    strip.setPixelColor(i,0,0,0);
  }
  strip.show();
}
