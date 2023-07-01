#include <SPI.h>
#include <SdFat.h>
#include <LiquidCrystal_I2C.h>

#include "Gazelle.h"

LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display

int32_t	LastWrite, Now;

// SD chip select pin.  Be sure to disable any other SPI devices such as Enet.
const uint8_t SDChipSelect = 4;


#define FILE_BASE_NAME "F"
SdFat sd;
SdFile file;

TGazelle	Gazelle;
#define printGazelle(X)	Gazelle.SendCmd(X)

ArduinoOutStream	Log(file), Cmd(Serial1), LCD(lcd);

#define error(msg) 

char s[80];

void throb(char ch) {
	lcd.setCursor(0,0);
	LCD << ch;
}

//------------------------------------------------------------------------------
void setup() {
	lcd.init();
	lcd.backlight();
	Gazelle.Init();

	const uint8_t BASE_NAME_SIZE = sizeof(FILE_BASE_NAME) - 1;
	char fileName[13] = FILE_BASE_NAME "0000.csv";

	Serial1.begin(9600);	// logger in/output
	pinMode(1, INPUT_PULLUP);

  // Initialize the SD card at SPI_HALF_SPEED to avoid bus errors with
  // breadboards.  use SPI_FULL_SPEED for better performance.  

	pinMode(SDChipSelect, OUTPUT);

 	if (!sd.begin(SDChipSelect, SPI_HALF_SPEED)) {
    		LCD << "Card error";
  	}

  	// Find an unused file name.
  	if (BASE_NAME_SIZE > 6) {
    		error("FILE_BASE_NAME too long");
  	}
  
  	while (sd.exists(fileName)) {
  		for (int i=3; i>=0; i--) {
  			if (fileName[BASE_NAME_SIZE + i]++ != '9')	break;
  			fileName[BASE_NAME_SIZE + i]	= '0';
  		}
  	}
  	
  	if (!file.open(fileName, O_CREAT | O_WRITE | O_EXCL)) {
    		error("file.open");
  	}
  
	LCD << F("Logging to: ");
	lcd.setCursor(0,1);
	LCD << fileName;

	Cmd << 'A';

	delay(5000);
	lcd.clear();

	LastWrite	= millis();
	pinMode(17, OUTPUT);

	Gazelle.DisplayBatteryInfo();
	delay(5000);

	for (byte i=0; i<5; i++) Gazelle.IncreaseCycles();
	Gazelle.Charge(1);
	delay(1);
}

//------------------------------------------------------------------------------
void loop() {
	Gazelle.Action();
}
