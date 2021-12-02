#include <Wire.h> // apparently the most important shit idk arduino too yeah
#include <Arduino.h>// Include the Arduino Stepper Library
#include <Stepper.h> // stepper motor
#include <Encoder.h> // encoder
// Number of steps per output rotation
const int stepsPerRevolution = 200;
// Create Instance of Stepper library
Stepper myStepper(stepsPerRevolution, 4, 5, 6, 7);

#define S1 11
#define S2 12

int prevS1;
int curS1, curS2;

int val = 0;

void setup()
{
	// set the speed at 60 rpm:
	myStepper.setSpeed(60);
	// initialize the serial port:
	Serial.begin(9600);
	pinMode (S1, INPUT);
  pinMode (S2, INPUT);

  prevS1 = digitalRead(S1);
}

void loop()
{
  curS1 = digitalRead(S1);

  if (curS2!=curS1) {
    curS2 = digitalRead(S2);

    if(curS2 == curS1){
      val++;
    }
    else{
      val--;
    }
    Serial.print("Value = ");
    Serial.println(val);
  }

  prevS1=curS1;
  
  bool flag;

  if(curS1!=prevS1){
    curS2 = digitalRead(S2);
    if(flag){
      if(curS2==curS1){
        val++;
      }
      else{
        val--;
      }
      Serial.print("Value = ");
      Serial.println(val);
      flag=false;
    }
    else{
      flag=true;
    }
  }
  prevS1=curS1;

	// step one revolution in one direction:
	myStepper.step(stepsPerRevolution);
	delay(1500);

	// step one revolution in the other direction:
	myStepper.step(-stepsPerRevolution);
	delay(1500);
}
