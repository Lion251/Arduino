/*
 * POI maakt een cirkel. Stel dat een omwenteling 1 seconde duurt, en we kunnen 360 updates per seconde doen van de leds, dan hebben we 1 graad per pixel.
 * Ik wil een gekleurde vlecht maken, van drie door elkaar heen lopende sinussen.
 * Een mooie sinus is 3.14x zo lang als top-top hoog.
 * 8 pixels hoog, dus 25 pixels lang. Maak hier 32 van voor makkelijk digitaal verwerken.
 * -1 => 0, 1 => 7: y=3.5*(1+sin(x*pi/16))
 */
#include <FastLED.h>

// How many leds are in the strip?
#define NUM_LEDS 8

// Data pin that led data will be written out over
#define DATA_PIN 1

// Clock pin only needed for SPI based chipsets when not using hardware SPI
#define CLOCK_PIN 2

#define COLOR_ORDER GRB


// This is an array of leds.  One item for each led in your strip.
CRGB leds[NUM_LEDS];

const PROGMEM byte Heart[]	= {
  B00001110,
  B00011111,
  B00111111,
  B01111110,
  B11111100,
  B01111110,
  B00011111,
  B00011111,
  B00001110,
  0,
  0,
  0,
  0,
  0,
  B11111111,
  B00010001,
  B00110001,
  B01010001,
  B10001110,
  0,
  0,
  B11111100,
  B00010010,
  B00010001,
  B00010010,
  B11111100,
  0,
  0, 0, 0,
};



byte	ColorIdx;
byte	Pt;


void setup() {
  FastLED.addLeds<WS2812, DATA_PIN, GRB>(leds, NUM_LEDS);
  ColorIdx	= 0;
  Pt		= 0;
}

void loop() {
  int	i;
  byte	Line	= pgm_read_byte_near(Heart + Pt);

  for (i = 0; i < 8; i++) {
    leds[i]	= Line & (1 << i) ? ColorFromPalette(RainbowColors_p, ColorIdx, 31, LINEARBLEND) : CRGB::Black;
  }
  if (++Pt == sizeof(Heart) / sizeof(Heart[0])) {
    Pt	= 0;
  }
  ColorIdx++;
  FastLED.show();
}


