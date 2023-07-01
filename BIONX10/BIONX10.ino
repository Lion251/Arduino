/*
Copyright (c) 2011, DIMITRIOS SIMITAS
 All rights reserved.
 
 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:
 
 1. Redistributions of source code must retain the above copyright
 notice, this list of conditions and the following disclaimer.
 2. Redistributions in binary form must reproduce the above copyright
 notice, this list of conditions and the following disclaimer in the
 documentation and/or other materials provided with the distribution.
 3. All advertising materials mentioning features or use of this software
 must display the following acknowledgement:
 This product includes software developed by Dimitrios Simitas.
 4. Neither the name of Dimitrios Simitas nor the
 names of its contributors may be used to endorse or promote products
 derived from this software without specific prior written permission.
 
 THIS SOFTWARE IS PROVIDED BY DIMITRIOS SIMITAS ''AS IS'' AND ANY
 EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 DISCLAIMED. IN NO EVENT SHALL DIMITRIOS SIMITAS BE LIABLE FOR ANY
 DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */


#include <SPI.h>                        // Arduino SPI Library
#include <MCP2515.h>                    // Frank Kienast's MCP2515 Library which appears to be the best.
#include <SdFat.h>
#include <LiquidCrystal_I2C.h>

#define FILE_BASE_NAME "Data"
// SD chip select pin.  Be sure to disable any other SPI devices such as Enet.
const uint8_t SD_chipSelect = 4;
SdFat sd;
SdFile file;

LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display

ArduinoOutStream	Log(file), LCD(lcd);

MCP2515 CAN1;      // Start CAN shield, cannot call up more then one currently
CANMSG msg;


void printHex(byte ch) {
	file.print(' ');
	if ((ch&0xF0)==0) file.print('0');
	file.print(ch, HEX);
}

//==============================================================================
// Error messages stored in flash.
#define error(msg) errorHalt(F(msg))
void errorHalt(const __FlashStringHelper *msg) {
	lcd.clear();
	lcd.setCursor(0,0);
	lcd.print(msg);
	for (;;);
}


void openFile(void) {
	const uint8_t BASE_NAME_SIZE = sizeof(FILE_BASE_NAME) - 1;
	char fileName[13] = FILE_BASE_NAME "00.txt";

  lcd.clear();
  // Initialize the SD card at SPI_HALF_SPEED to avoid bus errors with
  // breadboards.  use SPI_FULL_SPEED for better performance.
  if (!sd.begin(SD_chipSelect, SPI_HALF_SPEED)) {
    sd.initErrorHalt();
  }

  // Find an unused file name.
  if (BASE_NAME_SIZE > 6) {
    error("FILE_BASE_NAME too long");
  }
  while (sd.exists(fileName)) {
    if (fileName[BASE_NAME_SIZE + 1] != '9') {
      fileName[BASE_NAME_SIZE + 1]++;
    } else if (fileName[BASE_NAME_SIZE] != '9') {
      fileName[BASE_NAME_SIZE + 1] = '0';
      fileName[BASE_NAME_SIZE]++;
    } else {
      error("Can't create file name");
    }
  }
  if (!file.open(fileName, O_CREAT | O_WRITE | O_EXCL)) {
    error("file.open");
  }
  LCD << F("Logging to: ");
  lcd.setCursor(0,1);
  LCD << fileName;

	
}


void setup(){
	pinMode(17, OUTPUT);                   // Status LED
	pinMode(SD_chipSelect, OUTPUT);
	digitalWrite(SD_chipSelect, HIGH);
	pinMode(10,OUTPUT);
	pinMode(10, HIGH);
	
	lcd.init();
	lcd.backlight();

	for (;;) {
		CAN1.initCAN(CAN_BAUD_125K);
		delay(10);
	}

	for (;;) {

	LCD << "CAN ";
	if (CAN1.initCAN(CAN_BAUD_125K)){ // Starts up the CAN chip
		if (CAN1.setCANNormalMode()){      // Sets to Normal mode.
			LCD << "OK";
		} else LCD << "setMode fail";
	} else LCD << "initCAN fail";

	delay(1000);
	}
//	openFile();
	digitalWrite(17, HIGH);
}


void txmsg (){
  if (CAN1.transmitCANMessage(msg, 1000)){
  } 
}


void receivemsg (){
  if (CAN1.receiveCANMessage(&msg, 1000)){
  	Log << millis();
  	printHex(msg.adrsValue);
  	printHex(msg.dataLength);
  	for (int i=0; i<msg.dataLength; i++) printHex(msg.data[i]);
  	file.println();
  }
}

void loop(){
	static uint32_t	LastWrite;
	
	uint32_t	Now=millis();
	if (Now-LastWrite > 15000) {	// Write data to card every 15 seconds
		Log << Now << endl;
		file.sync();
		LastWrite	= Now;
	}

	receivemsg();
} 




























