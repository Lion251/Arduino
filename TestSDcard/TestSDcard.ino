#include <MinimumSerial.h>
#include <SdFat.h>
#include <SdFatConfig.h>
#include <SdFatmainpage.h>
#include <SdFatUtil.h>
#include <SdInfo.h>
#include <SdSpi.h>
#include <SdSpiCard.h>
#include <SdVolume.h>

SdFat	fs;
SdFile	file;

#define MMC_CS	3;

void sdsource_initcard() {
  lcd_printhome("Checking SD Card");
  if (!fs.begin(MMC_CS, 3)) {
    if (fs.card()->errorCode()) { 
      // Can use sd.card()->errorData() to find out more information.
      // errorCode of 0x01; errorData of 0xFF is wiring error or no card.
      strcpy(sd->last_error, "No card found");
      return;
    }
    if (fs.vol()->fatType() == 0) {
      strcpy(sd->last_error, "Bad Partition");
      return;
    }
    if (fs.vwd()->isOpen()) {
      strcpy(sd->last_error, "Bad Root");
      return;
    }
    strcpy(sd->last_error, "Unknown error");
    return;    
  }


void setup() {
  // put your setup code here, to run once:
  sdsource_initcard();
}

void loop() {
  // put your main code here, to run repeatedly:

}
