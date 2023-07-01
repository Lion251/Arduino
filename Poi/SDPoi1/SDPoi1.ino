#define SCL_PORT	PORTB
#define SCL_PIN		3
#define SDA_PORT	PORTB
#define SDA_PIN		4
#include <LiquidCrystal_I2C_SoftWire.h>
#include <Streaming.h>
#include <integer.h>
/*
#include <diskio.h>
#include <mmc.h>
#include <pffs.h>
*/

LiquidCrystal_I2C	lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display
#define LCD		lcd

/******************************************************************************************************
   Poi / LED strip sturing met data vanuit (micro-) SD kaartje op ATtiny85 / Digispark
   Pinout:
   	ATtiny85:	Digispark:	Function:
   	PB0 (5)		1		DI - Data In from SD card / Data Out to APA102 LED strip
   	PB1 (6)		2		DO - Data Out to SD card / LED
   	PB2 (7)		3		SCK - clock to SD card
	PB3 (2)		4		/CS - Chip Select to SD card
	PB4 (3)		5		LEDClk - clock to LED strip
	/Reset (1)	6
********************************************************************************************************/

byte RcvSPI(void) {
	return DoSPI(0xFF);
}

void SendSPI(byte ch) {
	DoSPI(ch);
}

byte DoSPI(byte ch) {
	byte retval;
	
	asm volatile (
		"nop\n\t"
		: "=d" (retval)
		: "r" (ch)
		:
	);
	return retval;
}

void SD_Select(void) {
	asm volatile (
		"CBI	%[_PORTB], %[_CS]\n\t"
		:
		:	[_PORTB]	"I" (_SFR_IO_ADDR(PORTB)), 
			[_CS]		"I" (PORTB3)
		:
	);
}

void SD_Deselect(void) {
	asm volatile (
		"SBI	%[_PORTB], %[_CS]\n\t"
		:
		:	[_PORTB]	"I" (_SFR_IO_ADDR(PORTB)), 
			[_CS]		"I" (PORTB3)
		:
	);
}

char	Buff[256];
//FATFS	Fatfs;

void setup() {
  DWORD fa;	/* Flash address */
  WORD br;	/* Bytes read */

	lcd.init();
	lcd.backlight();
	LCD	<< "Hallo";
/*
  disk_attach_spi_functs(SendSPI, RcvSPI, SD_Select, SD_Deselect);

  pf_mount(&Fatfs);	// Initialize file system 
  if (pf_open("app.bin") == FR_OK) {	// Open application file 
    pf_read(Buff, 256, &br);	// Load a page data 
  }
*/
  for (;;) ;	/* No application, Halt. */
}

void loop() {
  // put your main code here, to run repeatedly:

}
