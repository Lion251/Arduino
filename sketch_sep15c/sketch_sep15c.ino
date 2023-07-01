#include <diskio.h>
#include <integer.h>
#include <mmc.h>
#include <pffs.h>

byte SPI_Transfer(byte x) {
	return x;
}

inline byte rx(void) {
	return SPI_Transfer(0xFF);
}

inline void tx(byte x) {
	SPI_Transfer(x);
}

void setup() {
	FRESULT	RetVal	= pf_mount(&FatFS);

}

void loop() {
  // put your main code here, to run repeatedly:

}
