#include <Arduino_GFX_Library.h>
#include <driver/adc.h>
#include <esp_sleep.h>
#include <esp_wifi.h>
#include <Streaming.h>
#include <WiFi.h>
#include "soc/rtc_cntl_reg.h"
#include "soc/rtc.h"
#include "driver/rtc_io.h"

#define MAX_ONTIME 600000L
 
#define TFT_SCK    18
#define TFT_MOSI   23
#define TFT_MISO   19
#define TFT_CS     5
#define TFT_DC     13
#define TFT_RESET  12
#define TFT_BACKLT GPIO_NUM_14
#define ENCODER_VP 27
#define LOCK_OUT   2
#define TOUCH_CS   27

uint8_t WifiOn;

char *Romeins[] = {
  "    ", // '0'
  "I   ", "II  ", "III ",  "IV  ", "V   ", "VI  ", "VII ",  "VIII",   "IX  ", "X   ", 
  "XI  ", "XII ", "XIII",  "XIV ", "XV  ", "XVI ", "XVII",  "XVIII",  "XIX ", "XX  ", 
  "XXI ", "XXII", "XXIII", "XXIV", "XXV ", "XXVI", "XXVII", "XXVIII", "XXIX", "XXX ",
  "XXXI"
};

const uint8_t MorseBitmap[] PROGMEM = {
  0b11110000,
  0b11111111,
  0b11110000,
};

const GFXglyph MorseGlyphs[] PROGMEM = {
  { 0, 2, 2, 4, 0, 3 },
  { 1, 6, 2, 8, 0, 3 }
};

GFXfont MorseFont = {
  (uint8_t *)MorseBitmap, (GFXglyph *)MorseGlyphs, '0', '1', 8
};

const char *MorseCijfers[] PROGMEM = {
  "01111",
  "00111",
  "00011",
  "00001",
  "00000",
  "10000",
  "11000",
  "11100",
  "11110",
  "11111"
};

const uint8_t BrailleBitmap[] PROGMEM = {
  0b10000000,
};

const GFXglyph BrailleGlyphs[] PROGMEM = {
  { 0, 1, 1, 0, 0, -4 },
  { 0, 1, 1, 0, 0, -2 },
  { 0, 1, 1, 0, 0,  0 },
  { 0, 1, 1, 0, 2, -4 },
  { 0, 1, 1, 0, 2, -2 },
  { 0, 1, 1, 0, 2,  0 },
  { 0, 0, 0, 6, 2,  1 }
};

GFXfont BrailleFont = {
  (uint8_t *)BrailleBitmap, (GFXglyph *)BrailleGlyphs, '0', '6', 8
};

const uint8_t BrailleBitmap4[] PROGMEM = {
  0b01101111,
  0b11110110
};

const GFXglyph BrailleGlyphs4[] PROGMEM = {
  { 0, 4, 4, 0, 0, -5 },
  { 0, 4, 4, 0, 0,  3 },
  { 0, 4, 4, 0, 0, 11 },
  { 0, 4, 4, 0, 8, -5 },
  { 0, 4, 4, 0, 8,  3 },
  { 0, 4, 4, 0, 8, 11 },
  { 0, 0, 0,24, 0,  0 },
};

GFXfont BrailleFont4 = {
  (uint8_t *)BrailleBitmap4, (GFXglyph *)BrailleGlyphs4, '0', '6', 32
};

const uint8_t BrailleCharSet[] = {
  0b00000001, // a
  0b00000011, // b
  0b00001001, // c
  0b00011001, // d
  0b00010001, // e
  0b00001011, // f
  0b00011011, // g
  0b00010011, // h
  0b00001010, // i
  0b00011010, // j
  0b00000101, // k
  0b00000111, // l
  0b00001101, // m
  0b00011101, // n
  0b00010101, // o
  0b00001111, // p
  0b00011111, // q
  0b00010111, // r
  0b00001110, // s
  0b00011110, // t
  0b00100101, // u
  0b00100111, // v
  0b00111010, // w
  0b00101101, // x
  0b00111101, // y
  0b00110101, // z
  0b00111100, // Number
};

const uint8_t SevenSegmentBitmap[] PROGMEM = {
  0b11111000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
  
  0b00001000,
  0b00001000,
  0b00001000,
  0b00001000,
  0b00001000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
  
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00001000,
  0b00001000,
  0b00001000,
  0b00001000,
  0b00001000,
  
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b11111000,
  
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b10000000,
  0b10000000,
  0b10000000,
  0b10000000,
  0b10000000,
  
  0b10000000,
  0b10000000,
  0b10000000,
  0b10000000,
  0b10000000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
  
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b11111000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
  
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
};

const GFXglyph SevenSegmentGlyphs[] PROGMEM = {
  {  0, 8, 9, 0, 0, -4 },  // Characters '0' tot '6' zijn not-forwarding (overstrike)
  {  9, 8, 9, 0, 0, -4 },  // Gebruik met BGColor=FGColor(transparant)
  { 18, 8, 9, 0, 0, -4 },
  { 27, 8, 9, 0, 0, -4 },
  { 36, 8, 9, 0, 0, -4 },
  { 45, 8, 9, 0, 0, -4 },
  { 54, 8, 9, 0, 0, -4 },
  { 63, 8, 9, 6, 0, -4 },  // Character 7 is forwarding space
};

GFXfont SevenSegmentFont = {
  (uint8_t *)SevenSegmentBitmap, (GFXglyph *)SevenSegmentGlyphs, '0', '8', 10
};

struct IRAM_ATTR counter {
  volatile int Val;
  int Min, Max, ClockPin, DataPin;
  int State;
  static int  Updated;  // Shared for all counters
  counter(int ClockPin, int DataPin, int StartVal, int Min, int Max); 
  void IRAM_ATTR update(void);
  operator int() { return Val; };
};

int IRAM_ATTR counter::Updated;

counter::counter(int ClockPin, int DataPin, int StartVal, int Min,int Max) :
    ClockPin(ClockPin),
    DataPin(DataPin),
    Val(StartVal),
    Min(Min),
    Max(Max) {
  
  int x1, x2;
  pinMode(ClockPin, INPUT_PULLUP);
  pinMode(DataPin, INPUT_PULLUP);

  x1  = digitalRead(ClockPin);
  x2  = digitalRead(DataPin);
  State = (x2 ^ x1) | (x2)<<1;

  Updated = 1;

  if (Val==-1) Val = random(Min, Max);
}


void IRAM_ATTR counter::update(void) {
  int x1, x2, OldVal;
  x1  = digitalRead(ClockPin);
  x2  = digitalRead(DataPin);

  OldVal  = Val;
  switch (State) {
    case 0:
      if (x1) {
        State++;
        Val++;
      } else if (x2) {
        State--;
        //Val--;
      }
      break;
      
    case 1:
      if (x2) {
        State++;
        //Val++;
      } else if (!x1) {
        State--;
        Val--;
      }
      break;

    case 2:
      if (!x1) {
        State++;
        //Val++;
      } else if (!x2) {
        State--;
        //Val--;
      }
      break;

    case 3:
      if (!x2) {
        State++;
        //Val++;
      } else if (x1) {
        State--;
        //Val--;
      }
      break;
  }
  State &=3;
  if (Val<Min) Val=Max;
  else if (Val>Max) Val=Min;

  if (Val != OldVal) { Updated = 1; }
}


#define Counter(Name, Clock, Data, StartVal, Min, Max) \
  struct Name##Counter##T : public counter { \
    static void IRAM_ATTR IntrptFunc(void); \
    Name##Counter##T(void); \
    ~Name##Counter##T(); \
  } Name##Counter; \
  \
  void IRAM_ATTR Name##Counter##T::IntrptFunc(void) { Name##Counter.update(); } \
  \
  Name##Counter##T::Name##Counter##T(void) : counter(Clock, Data, StartVal, Min, Max) { \
      attachInterrupt(digitalPinToInterrupt(Clock), IntrptFunc, CHANGE); \
      attachInterrupt(digitalPinToInterrupt(Data), IntrptFunc, CHANGE); \
  }; \
  \
  Name##Counter##T::~Name##Counter##T() { \
      detachInterrupt(digitalPinToInterrupt(Clock)); \
      detachInterrupt(digitalPinToInterrupt(Data)); \
  };

Arduino_ESP32SPI bus(TFT_DC, TFT_CS, TFT_SCK, TFT_MOSI, TFT_MISO);
Arduino_ILI9341 display(&bus, TFT_RESET);
#define LIGHTBLUE 0b1000010000011111

String NumToBitPos(int x) {
  String S;

  for (int i=0; x; x>>=1, i++) {
    if (x&1) S=S+(char)('0'+i);
  }
  return S;
}

String MorseString(int x) {
  String    S;
  uint16_t  Mask  = 0x4000;

  while (Mask && !(x & Mask)) { Mask>>=1; } // Find first 1. Does not produce output.
  Mask>>=1;                                 // Skip first 1.
  while (Mask) {
    S = S + ((x&Mask) ? "1" : "0");
    Mask>>=1;
  }
  return S;
}

Counter(LCD, 34, 35, -1, 0, 127);
Counter(Binary, 39, 36, -1, 0, 15);
Counter(Roman, 25, 26, -1, 1, 12);
Counter(Braille, 16, 17, -1, 0, 9);
Counter(Morse, 22, 21, -1, 0, 9);

void disableWiFi(void) {
  adc_power_off();
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
}


void PowerOff(void) {
    digitalWrite(TFT_BACKLT, LOW);
    gpio_hold_en(TFT_BACKLT);
    display.displayOff();
    pinMode(ENCODER_VP, INPUT);
    disableWiFi();
    esp_deep_sleep_start();
    for ( ; ; ) ;  
}


void DrawCheckmark(void) {
  int i;
  for (i=0; i<10; i++) {
    display.drawLine(181+i/3, 120+i/2, 230-i, 200, GREEN);
  }
  for (i=0; i<10; i++) {
    display.drawLine(230-i, 200, 280-i/2, 50+i/2, GREEN);
  }
}

void setup(void){
  pinMode(LOCK_OUT, OUTPUT);
  digitalWrite(LOCK_OUT, LOW);

  pinMode(TOUCH_CS, OUTPUT);
  digitalWrite(TOUCH_CS, HIGH);
  
  display.begin();
  display.setRotation(1);
  display.invertDisplay(1);
  display.fillScreen(BLACK);
  display.setTextSize(3);
  display.setTextWrap(0);
  
  display.setCursor(0, 60);
  display.setTextColor(RED);
  display << "7-segment";

  display.setCursor(0, 90);
  display.setTextColor(ORANGE);
  display <<"Binair ";

  display.setCursor(0, 120);
  display.setTextColor(WHITE);
  display << "Romeins ";

  display.setCursor(0, 150);
  display.setTextColor(YELLOW);
  display << "Braille ";

  display.setCursor(0, 180);
  display.setTextColor(LIGHTBLUE);
  display << "Morse ";

  display.setTextSize(2);

  pinMode(TFT_BACKLT, OUTPUT);
  digitalWrite(TFT_BACKLT, HIGH);
  pinMode(ENCODER_VP, OUTPUT);
  digitalWrite(ENCODER_VP, HIGH);
  WifiOn  = 1;
}

void loop() {
  static int OKTime =0;
  
  if (WifiOn && millis()>10000) {
    WifiOn  = 0;
    disableWiFi();
  }

  if (LCDCounter==91 && BinaryCounter==2 && RomanCounter==2 && BrailleCounter==1 && MorseCounter==1) {
    delay(100);
    if (++OKTime==7) {
      DrawCheckmark();
      digitalWrite(LOCK_OUT, HIGH);
      delay(2000);
      digitalWrite(LOCK_OUT, LOW);
      delay(30000);
      PowerOff();
    }
  } else {
    OKTime=0;
  }

  if (millis()>MAX_ONTIME) {  // First, switch display off, then switch power off
    PowerOff();
  }

  if (!counter::Updated) return;
  counter::Updated = 0;

  display.fillRect(0, 0, 320, 40, BLACK);
  display.setFont(NULL);
  display.setCursor(00, 20);

  display.setTextSize(3);
  display.setFont(&SevenSegmentFont);
  display.setTextColor(0b0011000000000000);
  display << NumToBitPos(127);
  display.setTextColor(RED);  // Start overwriting
  display << NumToBitPos(LCDCounter | 0x80);    // 0x80 is the move-forward space

  display.setFont(NULL);
  display.setTextSize(1);
  display<<" ";

  display.setTextSize(2);
  display.setTextColor(ORANGE);
  display <<_WIDTHZ(_BIN(BinaryCounter&0x0F), 4);

  display.setTextColor(WHITE);
  display<<"/";

  display.setTextColor(WHITE);
  display << Romeins[RomanCounter%16];
  
  display.setTextColor(WHITE);
  display<<"/";

  display.setTextSize(1);
  display<<" ";

  display.setFont(&BrailleFont4);
  display.setTextSize(1);

  display.setTextColor(0b0000000100000000);
  display << NumToBitPos(63);
  display.setTextColor(YELLOW);
  display << NumToBitPos(BrailleCharSet[26] | 0x40);  // 0x40 is the move-forward space

  display.setTextColor(0b0000000100000000);
  display << NumToBitPos(63);
  display.setTextColor(YELLOW);
  display << NumToBitPos(BrailleCharSet[BrailleCounter] | 0x40);  // 0x40 is the move-forward space

  display.setFont(&MorseFont);
  display.setTextColor(LIGHTBLUE);
  display.setTextSize(3);
  display << MorseCijfers[MorseCounter];  
/*
  display.setFont(NULL);
  display.setTextSize(3);
  
  display.setCursor(180, 60);
  display.setTextColor(RED, BLACK);
  display << (int)LCDCounter << "  ";

  display.setCursor(180, 90);
  display.setTextColor(ORANGE, BLACK);
  display << (int)BinaryCounter << "  ";

  display.setCursor(180, 120);
  display.setTextColor(WHITE, BLACK);
  display << (int)RomanCounter << "  ";

  display.setCursor(180, 150);
  display.setTextColor(YELLOW, BLACK);
  display << (int)BrailleCounter << "  ";

  display.setCursor(180, 180);
  display.setTextColor(LIGHTBLUE, BLACK);
  display << (int)MorseCounter << "  ";
*/

 }
