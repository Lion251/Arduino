#include <Streaming.h>

//YWROBOT
//Compatible with the Arduino IDE 1.0
//Library version:1.1
//#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display

void setup() {
	pinMode(4, OUTPUT);
	digitalWrite(4, HIGH);
	delay(100);
  lcd.init();                      // initialize the lcd 
  // Print a message to the LCD.
  lcd.backlight();
  lcd.setCursor(3,0);
  lcd << "Hello, world!";
  lcd.setCursor(2,1);
  lcd << "Ywrobot Arduino!";
  lcd.createChar(0, NULL);
}


void loop()
{
}
