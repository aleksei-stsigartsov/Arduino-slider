#include <Wire.h> // apparently the most important shit idk arduino too yeah
#include <Arduino.h>// Include the Arduino Stepper Library
#include <Stepper.h> // stepper motor
#include <Encoder.h> // encoder
#include <LedControl.h> //maxon
#include <LiquidCrystal_I2C.h> //lcd

// Number of steps per output rotation
const int stepsPerRevolution = 200;
// Create Instance of Stepper library

/////////////////////MAXON SECTION//////////////////////
int DIN = 12;
int CS = 11;
int LCLK = 10;
byte center[8] = {0xE7, 0x99, 0xA5, 0x5A, 0x5A, 0xA5, 0x99, 0xE7};
byte rightArrow[8] = {0x08, 0x0C, 0xFE, 0xFF, 0xFF, 0xFE, 0x0C, 0x08};
byte leftArrow[8] = {0x10, 0x30, 0x7F, 0xFF, 0xFF, 0x7F, 0x30, 0x10};
byte smile[8] = {0x3C, 0x42, 0xA5, 0x81, 0xA5, 0x99, 0x42, 0x3C};
LedControl lc = LedControl(DIN, LCLK, CS, 0);
void printByte(byte character[])
{
  int i = 0;
  for (i = 0; i < 8; i++)
  {
    lc.setRow(0, i, character[i]);
  }
}
//////////////////LCD I2C SECTION////////////////////
LiquidCrystal_I2C lcd(0x27, 16, 2);
void movementCenter()
{
  lcd.setCursor(1, 0);
  lcd.print("Movement to");
  lcd.setCursor(8, 1);
  lcd.print("the center");
}
void movementLeft()
{
  lcd.setCursor(1, 0);
  lcd.print("Movement to");
  lcd.setCursor(8, 1);   
  lcd.print("the left");
}
void movementRight()
{
  lcd.setCursor(1, 0);
  lcd.print("Movement to");
  lcd.setCursor(8, 1);
  lcd.print("the right");
}
void movementCenterDone()
{
  lcd.setCursor(1, 0);
  lcd.print("SUCCESS");
  lcd.setCursor(8, 1);
  lcd.print("");
}

Stepper myStepper(stepsPerRevolution, 4, 5, 6, 7);

#define S1 11
#define S2 12

int prevS1;
int curS1, curS2;

int val = 0;

void setup()
{
  lcd.init();
  lcd.backlight();
  lc.shutdown(0, false);  //the maxon is in power-saving mode on startup
  lc.setIntensity(0, 15); //set the brightness of maxon to maximum value
  lc.clearDisplay(0);     //and clear the display
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

  // init displays
  lcd.print(millis() / 1000); 
  lcd.display();
  
  // movement to the center 
  movementCenter();
  printByte(center);
  delay(1500);

	// step one revolution in one direction:
  movementRight();
  printByte(rightArrow);
	myStepper.step(stepsPerRevolution);
	delay(1500);

	// step one revolution in the other direction:
  movementLeft();
  printByte(leftArrow);
	myStepper.step(-stepsPerRevolution);
	delay(1500);

  // we are done 
  movementCenterDone();
  printByte(center);
  delay(1500);
  printByte(smile);
  delay(1500);

  // clear displays
  lc.clearDisplay(0); 
  lcd.noDisplay();
}
