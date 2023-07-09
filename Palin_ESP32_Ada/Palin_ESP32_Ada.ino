#include <Adafruit_GFX.h>
#include <Adafruit_SPITFT_Macros.h>
#include <Adafruit_GrayOLED.h>
#include <gfxfont.h>
#include <Adafruit_SPITFT.h>

#include <Adafruit_ILI9341.h>

//#include <Arduino_GFX_Library.h>
#include <Streaming.h>
 
#define TFT_CLK    18
#define TFT_MOSI   23
#define TFT_MISO   19
#define TFT_CS     5
#define TFT_DC     13
#define TFT_RST    12

char *Romeins[] = {
  "     ", // '0'
  "    I", "   II", "  III", "   IV", "    V", "   VI", "  VII", " VIII", "   IX", "    X", 
  "   XI", "  XII", " XIII", "  XIX", "   XX", "  XXI", " XXII", "XXIII", " XXIV", "  XXV", 
  " XXVI"
};

const uint8_t MorseBitmap[] PROGMEM = {
  0b11110000,
  0b11111111,
  0b11110000,
};


const GFXglyph MorseGlyphs[] PROGMEM = {
  { 0, 2, 2, 4, 0, -2 },
  { 1, 6, 2, 8, 0, -2 }
};

GFXfont MorseFont = {
  (uint8_t *)MorseBitmap, (GFXglyph *)MorseGlyphs, '0', '1', 8
};

const uint8_t SevenSegmentBitmap[] PROGMEM = {
  0b11111000,
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
};

const GFXglyph SevenSegmentGlyphs[] PROGMEM = {
  {  0, 8, 7, 6, 0, -6 },  // Characters '0' tot '6' zijn not-forwarding (overstrike)
  {  7, 8, 7, 0, 0, -6 },  // Gebruik met BGColor=FGColor(transparant)
  { 14, 8, 7, 0, 0, -6 },
  { 21, 8, 7, 0, 0, -6 },
  { 28, 8, 7, 0, 0, -6 },
  { 35, 8, 7, 0, 0, -6 },
  { 42, 8, 7, 0, 0, -6 },
  { 49, 8, 7, 6, 0, -6 },  // Character 7 is forwarding space
  { 49, 8, 7, 0, 0, -6 }   // Character 8 is overwriting space
};

GFXfont SevenSegmentFont = {
  (uint8_t *)SevenSegmentBitmap, (GFXglyph *)SevenSegmentGlyphs, '0', '8', 8
};

struct IRAM_ATTR counter {
  volatile int Val;
  int Min, Max, ClockPin, DataPin;
  int State;
  static int  Updated;  // Shared for all counters
  counter(int ClockPin, int DataPin, int StartVal, int Min, int Max); 
  void IRAM_ATTR update(void);
};

int counter::Updated;

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
}


void counter::update(void) {
  int x1, x2;
  x1  = digitalRead(ClockPin);
  x2  = digitalRead(DataPin);
  
  switch (State) {
    case 0:
      if (x1) {
        State++;
        Val--;
      } else if (x2) {
        State--;
        //Val++;
      }
      break;
      
    case 1:
      if (x2) {
        State++;
        //Val--;
      } else if (!x1) {
        State--;
        Val++;
      }
      break;

    case 2:
      if (!x1) {
        State++;
        Val--;
      } else if (!x2) {
        State--;
        //Val++;
      }
      break;

    case 3:
      if (!x2) {
        State++;
        //Val--;
      } else if (x1) {
        State--;
        Val++;
      }
      break;
  }
  State &=3;
  if (Val<Min) Val=Min;
  else if (Val>Max) Val=Max;

  Updated = 1;
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

Adafruit_ILI9341 display(TFT_CS, TFT_DC, TFT_MOSI, TFT_CLK, TFT_RST, TFT_MISO);
#define LIGHTBLUE 0b1000010000011111

Counter(LCD, 25, 26, 3, 0, 31);
Counter(Braille, 17, 16, 4, 0, 63);
Counter(Decimal, 27, 14, 5, 0, 9);
Counter(Morse, 39, 36, 6, 0, 31);
Counter(Binary, 34, 35, 7, 0, 9);

String NumToBitPos(int x) {
  String S;

  x|=0x80;  // end marker. Will be printed as '7'.
  for (int i=0; x; x>>=1, i++) {
    if (x&1) S=S+(char)('0'+i);
  }
  return S;
}

#define BLACK ILI9341_BLACK
#define RED   ILI9341_RED
#define ORANGE ILI9341_ORANGE
#define YELLOW ILI9341_YELLOW
#define GREEN  ILI9341_GREEN
#define BLUE   ILI9341_BLUE
#define WHITE  ILI9341_WHITE
#define DARKGREY ILI9341_DARKGREY

void setup(void)
{
  display.begin();
  display.setRotation(1);
  display.invertDisplay(1);
  display.fillScreen(BLACK);
  display.setTextSize(2);
  display.setTextWrap(0);
  
  pinMode(14, OUTPUT);
  digitalWrite(14, HIGH);
  
  display.setCursor(40, 40);
  display.setTextColor(RED, DARKGREY);
  display << "7-segment";

  display.setCursor(40, 90);
  display.setTextColor(ORANGE, BLACK);
  display <<"Binair ";

  display.setCursor(40, 120);
  display.setTextColor(YELLOW);
  display << "Romeins ";

  display.setCursor(20, 150);
  display.setTextColor(GREEN);
  display << "Braille ";

  display.setCursor(20, 180);
  display.setTextColor(LIGHTBLUE);
  display << "Morse ";
}


void loop() {

  if (!LCDCounter.Updated) return;
  LCDCounter.Updated = 0;
  
  display.setTextColor(RED, YELLOW);
  display.setFont(&SevenSegmentFont);
  display.setCursor(40, 20);
  display<< "8"; // Wis eerste teken
  
  display.setTextColor(RED);  // Start overwriting
  display << NumToBitPos(LCDCounter.Val);

  display.setFont(NULL);
  display.setTextColor(ORANGE,BLACK);
  display << " " << _WIDTHZ(_BIN(LCDCounter.Val), 5);

  display.setTextColor(WHITE, BLACK);
  display<<"/";

  display.setTextColor(YELLOW, BLACK);
  display << Romeins[LCDCounter.Val];
  
  display.setTextColor(WHITE, BLACK);
  display<<"/";

  display.setFont(&MorseFont);
  display.setTextColor(LIGHTBLUE, DARKGREY);
  display << _WIDTHZ(_BIN(LCDCounter.Val), 5);  

  display.setFont(NULL);
  display.setTextColor(WHITE, BLACK);
  display<<"AB         ";

 }
