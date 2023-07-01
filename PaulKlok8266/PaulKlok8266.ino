#include <WiFi.h>
#include "time.h"
#include "sntp.h"

char* ssid       = "Henknet4";
char* password   = "maakhiermaareenssoepvan";

const char* ntpServer1 = "pool.ntp.org";
const char* ntpServer2 = "time.nist.gov";

const char* time_zone = "CET-1CEST,M3.5.0,M10.5.0/3";  // TimeZone rule for Europe/Rome including daylight adjustment rules (optional)
uint32_t    counter;


int segPin[7]  = { 14, 16, 17, 26, 25, 27, 12 }; // pins connected to segments a-g of all digits
int posPin[4]  = { 13, 5, 23, 19 };              // pins connected to common of digits, right to left
char shape[19] = {
  0b00111111, // 0
  0b00000110, // 1
  0b01011011, // 2
  0b01001111, // 3
  0b01100110, // 4
  0b01101101, // 5
  0b01111101, // 6
  0b00000111, // 7
  0b01111111, // 8
  0b01101111, // 9
  0b01110111, // a
  0b01111100, // b
  0b00111001, // c
  0b01011110, // d
  0b01111001, // e
  0b01110001, // f
  0b00001000, // _
  0b01000000, // -
  0b00000001
};

char Digits[4] = {
  shape[16], shape[16], shape[16], shape[16]
};
/*
void printLocalTime()
{
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Digits[0] = Digits[1] = Digits[2] = Digits[3] = shape[17];  // '-'
  } else {
    Digits[0] = shape[timeinfo.tm_sec % 10];
    Digits[1] = shape[timeinfo.tm_sec / 10];
    Digits[2] = shape[timeinfo.tm_min % 10];
    Digits[3] = shape[timeinfo.tm_min / 10];
  }
}

void IRAM_ATTR onClock(void) {
  int         i;
  static int  digit;

  digitalWrite(posPin[digit++], LOW);   // previous digit off, select next digit
  digit &= 3;                           // if digit==4 digit=0

  for (i = 0; i < 7; i++) {
    digitalWrite(segPin[i], !(Digits[digit] & (1L << i)));
  }
  digitalWrite(posPin[digit], HIGH);    // new digit on until next interrupt
}
*/
void setup()
{
  int i;

  Serial.begin(115200);

#if 0
  // init output pins
  for (i = 0; i < 7; i++) {
    gpio_set_drive_capability((gpio_num_t)segPin[i], GPIO_DRIVE_CAP_1);
    pinMode(segPin[i], OUTPUT);
    digitalWrite(segPin[i], HIGH); // segments are off when high
  }
  for (i = 0; i < 4; i++) {
    gpio_set_drive_capability((gpio_num_t)posPin[i], GPIO_DRIVE_CAP_3);
    pinMode(posPin[i], OUTPUT);
    digitalWrite(posPin[i], LOW);
  }
#endif

#if 0
  // Initialize timer
  Clock = timerBegin(1, 80, true);  // 1 MHz repeating timer. Channel 0, prescaler 80, count up.
  timerAttachInterrupt(Clock, onClock, true);
  timerAlarmWrite(Clock, 1000, true); // 1 ms timer, auto reload.
  timerAlarmEnable(Clock);
#endif

#if 0
//DO NOT TOUCH
    //  This is here to force the ESP32 to reset the WiFi and initialise correctly.
    Serial.print("WIFI status = ");
    Serial.println(WiFi.getMode());
    WiFi.disconnect(true);
    delay(1000);
    WiFi.mode(WIFI_STA);
    delay(1000);
    Serial.print("WIFI status = ");
    Serial.println(WiFi.getMode());
    // End silly stuff !!!
#endif

  //connect to WiFi
//  WiFi.setHostname("PaulKlok");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Digits[0] = shape[WiFi.status() & 0x0F];
  }

  /**
     A more convenient approach to handle TimeZones with daylightOffset
     would be to specify a environmnet variable with TimeZone definition including daylight adjustmnet rules.
     A list of rules for your zone could be obtained from https://github.com/esp8266/Arduino/blob/master/cores/esp8266/TZ.h
  */
//  configTzTime(time_zone, ntpServer1, ntpServer2);
}

void loop()
{
//  printLocalTime();
  delay(100);
}
