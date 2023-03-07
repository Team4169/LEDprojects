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
//  Serial.begin(9600);           // start serial for debugging output
//  Serial.println("Setting up");
 
 // now initialize the LED strip
 strip.begin();
 strip.setBrightness(50);
 for (int i = 0; i < strip.numPixels(); i ++){
   strip.setPixelColor(i, 255, 0, 255);
 }
 strip.show(); // Initialize all pixels to 'off'
 
 
}
 
int currentCommand = 0;
 
 
 
void loop() {
 // we don't actually do anything directly here in the loop().
 // instead, we wait for commands to come in; when they do,
 // the receiveEvent() function will be called (since it was
 // set up that way in our Wire.onReceive(receiveEvent) call in setup() above
 if (currentCommand == COMMAND1) {
  //  Serial.println("Solid Red Lights");
   showRed(COMMAND1);  
 } else if (currentCommand == COMMAND2) {
   showBlue(COMMAND2);
 }else if (currentCommand == COMMAND3) {
   redTrail(COMMAND3);
 }else if (currentCommand == COMMAND4) {
   blueTrail(COMMAND4);
  }//else{
//    strip.clear();
//  }
}
 
volatile boolean stop_animation = false;
 
// this function executes whenever data is received from master
// this function is registered as an event, see setup()
void receiveEvent(int howMany) {
 int command = Wire.read(); // receive the command (a single byte as a character)
 stop_animation = true; // cancel any previously running animation
//  char secondcommand = Wire.read();
//  Serial.print("howMany: ");
//  Serial.println(howMany);
//  Serial.print("recieved: ");
//  Serial.println(command);
//  Serial.println(secondcommand);
//  strip.clear();
 if (command == COMMAND1) {
   currentCommand = COMMAND1;   
 } else if (command == COMMAND2) {
   currentCommand = COMMAND2;
 } else if (command == COMMAND3) {
   currentCommand = COMMAND3;
 } else if (command == COMMAND4) {
   currentCommand = COMMAND4;
 }
//  strip.show();
}
 
bool checkForNewDesign(int showingCommand){
  //  Serial.println("Checking for new");

  if (showingCommand == currentCommand){
    return true;
  } else {
    strip.clear();
    return false;
  }

}

void showBlue(int commandNumber){
  if (checkForNewDesign(commandNumber) == false){ 
     return;
   }
  for (int i = 0; i < strip.numPixels(); i ++){
    strip.setPixelColor(i, 0, 255, 255);
  }
  strip.show();
}

void showRed(int commandNumber){
  if (checkForNewDesign(commandNumber) == false){ 
     return;
   }
  for (int i = 0; i < strip.numPixels(); i++){
    strip.setPixelColor(i, 255, 0, 0);
  }
  strip.show();
}

void blueTrail(int commandNumber){
  strip.clear();
  for (int i = 0; i < strip.numPixels(); i ++){
    strip.setPixelColor(i, 0, 0, 255);
    strip.show();
    if (checkForNewDesign(commandNumber) == false){ 
     return;
   }
    delay(30);
  }
  for (int i = 0; i < strip.numPixels(); i ++){
    strip.setPixelColor(i, 0, 0, 0);
    strip.show();
    if (checkForNewDesign(commandNumber) == false){ 
     return;
   }
    delay(30);
  }
}

void redTrail(int commandNumber){
  strip.clear();
  for (int i = 0; i < strip.numPixels(); i ++){
    strip.setPixelColor(i, 255, 0, 0);
    strip.show();
    if (checkForNewDesign(commandNumber) == false){ 
     return;
   }
    delay(30);
  }
  for (int i = 0; i < strip.numPixels(); i ++){
    strip.setPixelColor(i, 0, 0, 0);
    strip.show();
    if (checkForNewDesign(commandNumber) == false){ 
     return;
   }
    delay(30);
  }
}


void redGreenTrail(int wait, int commandNumber){
 strip.clear();
 for (int i = 0; i < strip.numPixels(); i+=2){
   strip.setPixelColor(i, 255, 0, 0);
   strip.setPixelColor(i+1, 0, 255, 0);
   strip.show();
   if (checkForNewDesign(commandNumber) == false){ //don't know if this works
     break;
   }
   delay(wait);
 }
 
}


//these are not going to be used, but are here anyway
void flashBlueRedGreen(int wait, int commandNumber){
 strip.clear();
 for (int i = 0; i< strip.numPixels(); i++){
   strip.setPixelColor(i, 0, 0, 255); 
   if (checkForNewDesign(commandNumber) == false){ //don't know if this works
     break;
   }
 }
 strip.show();
 delay(wait);
 for (int i = 0; i< strip.numPixels(); i++){
   strip.setPixelColor(i, 255, 0, 0); 
   if (checkForNewDesign(commandNumber) == false){ //don't know if this works
     break;
   }
 }
 strip.show();
 delay(wait);
 for (int i = 0; i< strip.numPixels(); i++){
   strip.setPixelColor(i, 0, 255, 0); 
   if (checkForNewDesign(commandNumber) == false){ //don't know if this works
     break;
   }
 }
 strip.show();
 delay(wait);
} 
 
 
// Rainbow-enhanced theater marquee. Pass delay time (in ms) between frames.
void theaterChaseRainbow(int wait, int commandNumber) {
 int firstPixelHue = 0;     // First pixel starts at red (hue 0)
 for(int a=0; a<30; a++) {  // Repeat 30 times...
   for(int b=0; b<3; b++) { //  'b' counts from 0 to 2...
     strip.clear();         //   Set all pixels in RAM to 0 (off)
     // 'c' counts up from 'b' to end of strip in increments of 3...
     for(int c=b; c<strip.numPixels(); c += 3) {
       // hue of pixel 'c' is offset by an amount to make one full
       // revolution of the color wheel (range 65536) along the length
       // of the strip (strip.numPixels() steps):
       int      hue   = firstPixelHue + c * 65536L / strip.numPixels();
       uint32_t color = strip.gamma32(strip.ColorHSV(hue)); // hue -> RGB
       strip.setPixelColor(c, color); // Set pixel 'c' to value 'color'

       if (checkForNewDesign(commandNumber) == false){ //don't know if this works
        break;
       }
     }
     if (checkForNewDesign(commandNumber) == false){ //don't know if this works
      break;
     }

     strip.show();                // Update strip with new contents
     delay(wait);                 // Pause for a moment
     firstPixelHue += 65536 / 90; // One cycle of color wheel over 90 frames
   }
   if (checkForNewDesign(commandNumber) == false){ //don't know if this works
     break;
   }
 }
}
