/*******************************************************************************
 * Start of Arduino_GFX setting
 * 
 * Arduino_GFX try to find the settings depends on selected board in Arduino IDE
 * Or you can define the display dev kit not in the board list
 * Defalult pin list for non display dev kit:
 * Arduino Nano, Micro and more: TFT_CS:  9, TFT_DC:  8, TFT_RST:  7, TFT_BL:  6
 * ESP32 various dev board     : TFT_CS:  5, TFT_DC: 27, TFT_RST: 33, TFT_BL: 22
 * ESP32-C3 various dev board  : TFT_CS:  7, TFT_DC:  2, TFT_RST:  1, TFT_BL:  3
 * ESP32-S2 various dev board  : TFT_CS: 34, TFT_DC: 26, TFT_RST: 33, TFT_BL: 21
 * ESP8266 various dev board   : TFT_CS: 15, TFT_DC:  4, TFT_RST:  2, TFT_BL:  5
 * Raspberry Pi Pico dev board : TFT_CS: 17, TFT_DC: 27, TFT_RST: 26, TFT_BL: 28
 * RTL8720 BW16 old patch core : TFT_CS: 18, TFT_DC: 17, TFT_RST:  2, TFT_BL: 23
 * RTL8720_BW16 Official core  : TFT_CS:  9, TFT_DC:  8, TFT_RST:  6, TFT_BL:  3
 * RTL8722 dev board           : TFT_CS: 18, TFT_DC: 17, TFT_RST: 22, TFT_BL: 23
 * RTL8722_mini dev board      : TFT_CS: 12, TFT_DC: 14, TFT_RST: 15, TFT_BL: 13
 * Seeeduino XIAO dev board    : TFT_CS:  3, TFT_DC:  2, TFT_RST:  1, TFT_BL:  0
 * Teensy 4.1 dev board        : TFT_CS: 39, TFT_DC: 41, TFT_RST: 40, TFT_BL: 22
 ******************************************************************************/
#include <Arduino_GFX_Library.h>
#include <Streaming.h>

#define TFT_SCK    18
#define TFT_MOSI   23
#define TFT_MISO   19
#define TFT_CS     5
#define TFT_DC     13
#define TFT_RESET  12

Arduino_ESP32SPI bus(TFT_DC, TFT_CS, TFT_SCK, TFT_MOSI, TFT_MISO);
Arduino_ILI9341 display(&bus, TFT_RESET), *gfx=&display;
#define LIGHTBLUE 0b1000010000011111

#if 0
/* More dev device declaration: https://github.com/moononournation/Arduino_GFX/wiki/Dev-Device-Declaration */
#if defined(DISPLAY_DEV_KIT)
Arduino_GFX *gfx = create_default_Arduino_GFX();
#else /* !defined(DISPLAY_DEV_KIT) */

/* More data bus class: https://github.com/moononournation/Arduino_GFX/wiki/Data-Bus-Class */
Arduino_DataBus *bus = create_default_Arduino_DataBus();

/* More display class: https://github.com/moononournation/Arduino_GFX/wiki/Display-Class */
Arduino_GFX *gfx = new Arduino_ILI9341(bus, TFT_RST, 0 /* rotation */, false /* IPS */);

#endif /* !defined(DISPLAY_DEV_KIT) */
/*******************************************************************************
 * End of Arduino_GFX setting
 ******************************************************************************/
#endif
/* more fonts at: https://github.com/moononournation/ArduinoFreeFontFile.git */
#include "FreeMono8pt7b.h"
#include "FreeSansBold10pt7b.h"
#include "FreeSerifBoldItalic12pt7b.h"

const uint8_t MorseBitmap[2] PROGMEM = {
  0b10000000,
  0b11100000,
};

const GFXglyph MorseGlyphs[] PROGMEM = {
  { 0, 1, 1, 2, 0, 0 },
  { 1, 3, 1, 4, 0, 0 }
};

GFXfont MorseFont = {
  (uint8_t *)MorseBitmap, (GFXglyph *)MorseGlyphs, '0', '1', 8
};

void setup(void)
{
    gfx->begin();
    gfx->setRotation(1);
    gfx->invertDisplay(1);
    gfx->fillScreen(BLACK);
  display.setTextSize(3);
  display.setTextWrap(0);

#ifdef TFT_BL
    pinMode(TFT_BL, OUTPUT);
    digitalWrite(TFT_BL, HIGH);
#endif

    gfx->setCursor(20, 20);
    gfx->println("Hello World!");
    gfx->setFont(&FreeMono8pt7b);
    gfx->setTextColor(RED);

    delay(5000); // 5 seconds
}

void loop()
{
    gfx->setCursor(random(gfx->width()), random(gfx->height()));
    gfx->setTextColor(random(0xffff), BLACK);
    uint8_t textSize = 3; //random(4);
    switch (textSize)
    {
    case 1:
        gfx->setFont(&FreeMono8pt7b);
        break;
        
    case 2:
        gfx->setFont(&FreeSansBold10pt7b);
        break;
        
    case 3:
        gfx->setFont(&MorseFont);
        break;
        
    default:
        gfx->setFont(&FreeSerifBoldItalic12pt7b);
        break;
    }

    display<<"010011000111";
    display.setFont(NULL);
    display<<"World!";

    delay(1000); // 1 second
}
