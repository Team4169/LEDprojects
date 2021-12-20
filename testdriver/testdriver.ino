// testdriver.ino
//
// This sketch implements a simple test driver that will send
// commands over the I2C "wire" interface.
// These commands are intended to be received by an I2C "slave"
// device that is listening on the I2C wire.
//
// This test driver can be used to simulate the RoboRio sending these same commands.
//
// Based on the "Master Writer" example sketch (see File/Examples/Wire/Master Writer)
// Background info at: https://docs.arduino.cc/learn/communication/wire
//


// The Wire Library implements the I2C communiation protocol between a 
// controller ("master") and a peripheral ("slave")
#include <Wire.h>

// the SLAVE_ADDRESS must be defined the same on the master and the slave,
// so that the slave will know that the master is trying to communicate with it
// as opposed to some other slave device on the same wire

const int SLAVE_ADDRESS = 100;

// define the pins we'll use for our various pushbuttons
const int PUSHBUTTON1 = 7;
const int PUSHBUTTON2 = 8;
const int PUSHBUTTON3 = 9;
const int PUSHBUTTON4 = 10;

// define the commands we'll send down the wire
// the master and slave need to agree on what these are!
const char COMMAND1 = 1;
const char COMMAND2 = 2;
const char COMMAND3 = 3;
const char COMMAND4 = 4;


void setup() {
  Wire.begin(); // join i2c bus (address optional for master)

  // set up some pins to read as inputs, to control our test driver
  pinMode(PUSHBUTTON1, INPUT_PULLUP);
  pinMode(PUSHBUTTON2, INPUT_PULLUP);
  pinMode(PUSHBUTTON3, INPUT_PULLUP);
  pinMode(PUSHBUTTON4, INPUT_PULLUP);

  //Serial.begin(9600);           // start serial for debugging output
  //Serial.println("Test Driver Ready...");
  
}

void loop() {

  if (digitalRead(PUSHBUTTON1) == LOW) {
    sendCommand(COMMAND1);
    while (digitalRead(PUSHBUTTON1) == LOW) {      
    }
  } else if (digitalRead(PUSHBUTTON2) == LOW) {
    sendCommand(COMMAND2);
    while (digitalRead(PUSHBUTTON2) == LOW) {      
    }
  } else if (digitalRead(PUSHBUTTON3) == LOW) {
    sendCommand(COMMAND3);
    while (digitalRead(PUSHBUTTON3) == LOW) {      
    }
  } else if (digitalRead(PUSHBUTTON4) == LOW) {
    sendCommand(COMMAND4);
    while (digitalRead(PUSHBUTTON4) == LOW) {      
    }
  }
};

void sendCommand(int command) {
  Wire.beginTransmission(SLAVE_ADDRESS); // transmit to device that has this address
  Wire.write(command);       // sends the (one byte) command
  Wire.endTransmission();    // stop transmitting
}
