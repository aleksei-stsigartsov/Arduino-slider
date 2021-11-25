// apparently the most important shit idk arduino too yeah
#include <Wire.h>
#include <Arduino.h>
// Include the Arduino Stepper Library
#include <Stepper.h>

// encoder
#include <Encoder.h>

// Number of steps per output rotation
const int stepsPerRevolution = 200;

// Create Instance of Stepper library
Stepper myStepper(stepsPerRevolution, 4, 5, 6, 7);

#define CLK 11
#define DT 12

Encoder knob(CLK, DT);

void setup()
{
	// set the speed at 60 rpm:
	myStepper.setSpeed(60);
	// initialize the serial port:
	Serial.begin(9600);
	Serial.println("TwoKnobs Encoder Test:");
}


long positionLeft  = -999;
long positionRight = -999;

void loop() 
{
	// step one revolution in one direction:
	Serial.println("clockwise");
	myStepper.step(stepsPerRevolution);
	delay(500);

	// step one revolution in the other direction:
	Serial.println("counterclockwise");
	myStepper.step(-stepsPerRevolution);
	delay(500);

	long newLeft, newRight;
  newLeft = knob.read();
  newRight = knob.read();
  if (newLeft != positionLeft || newRight != positionRight) {
    Serial.print("Left = ");
    Serial.print(newLeft);
    Serial.print(", Right = ");
    Serial.print(newRight);
    Serial.println();
    positionLeft = newLeft;
    positionRight = newRight;
  }
  // if a character is sent from the serial monitor,
  // reset both back to zero.
  if (Serial.available()) {
    Serial.read();
    Serial.println("Reset both knobs to zero");
    knob.write(0);
    knob.write(0);
  }
}
