#define USE_HW_I2C 0

#include <Wire.h>
#include <Lion251_SSD1306.h> 

/*
long Count, Count2;

ISR (PCINT0_vect) { // handle pin change interrupt for A0 to A5 here
	byte		NewVal = PINA & 4, NewVal2 = PINA & 0x20;
	static byte	OldVal, OldVal2;
	
	if (PINA & 2) {
		if (NewVal && !OldVal) Count--;
		else if (OldVal && !NewVal) Count++;
	} else {
		if (NewVal && !OldVal) Count++;
		else if (OldVal && !NewVal) Count--;
	}

	if (!(PINA & 1)) Count=0; 
	
	OldVal	= NewVal;

	
	if (PINA & 0x10) {
		if (NewVal2 && !OldVal2) Count2--;
		else if (OldVal2 && !NewVal2) Count2++;
	} else {
		if (NewVal2 && !OldVal2) Count2++;
		else if (OldVal2 && !NewVal2) Count2--;
	}

	if (!(PINA & 0x08)) Count2=0; 
	
	OldVal2	= NewVal2;
}  


typedef struct Filt {
	float	Average;
	float	Speed;		// 0..1. 0.001 is slow, 0.999 is fast
	Filt(float aSpeed=0.25, float Init=0.0) { Speed=aSpeed;	Average=Init;	}
	float NewVal(float Val)	{ Average = Val*Speed + Average*(1-Speed);	return Average;	}
} TFilt;

TFilt	PressFilt(0.1, 1013.0);
*/
void setup() {
/*
	DDRA	= 0;		// port C all bits inputs
	PORTA	= 0xFF;		// Pullups on all pins
	PCMSK0	= 0x3F;		// Pin change interrupt on A0/A1/A2 /A3/A4/A5
	PCICR	|= 1<<PCIE0;	// Enable pin change interrupts
*/
	Wire.begin();	
	oled.init();                      // Initialze SSD1306 OLED display
	oled.setFont(font5x7);            // Set font type (default 8x8)
	oled.clearDisplay();              // Clear screen

 oled.Mode  = SSD1306_Bold | SSD1306_Compressed;
 oled.setTextXY(0, 0);
 oled.putString("De CODE van");
 oled.setTextXY(2, 0);
 oled.putString("De KLUIS is");
 oled.setTextXY(4, 0);
 oled.putString("R30-L10-R85");
 oled.setTextXY(6,0);
 oled.putString("L63-R28-L43");
//	oled.setTextXY(0,0);              // Set cursor position, start of line 0
//	oled.putString("Dual Rotary");
//	oled.setTextXY(2,0);              // Set cursor position, start of line 0
//	oled.putString("Encoder");
  
}

void loop() {
/*
	static uint8_t	i=0;
  return;
	for (;;) {
		oled.setTextXY(0, 0);
		oled.putString(F("T:   "));
		oled.setTextXY(4, 0);
		oled.putNumber(Count);
 		oled.putNumber(i++);
		oled.setTextXY(6, 0);
		oled.putNumber(Count2);
	}
*/
}
