#include <Streaming.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#include <SoftSerial.h>

class DummySerial : public Stream {
public:
  DummySerial(uint8_t receivePin, uint8_t transmitPin, bool inverse_logic = false) { };
  ~DummySerial() { };
  void begin(long speed) { };
  bool listen() {return 0; };
  void end() { };
  bool isListening() { return 0; }
  bool overflow() { return 0; }
  int peek() { return 0; };
  void txMode() { };
  void rxMode() { };
  virtual size_t write(uint8_t byte) { return 0; };
  virtual int read() { return 0; };
  virtual int available() { return 0; };
  virtual void flush() { };
};

const int Pin=1;
byte      Direction=1;

SoftSerial  MySerial(0,0);

void InitADC(void) {
  
}

uint16_t ReadADC(byte channel){
  uint16_t  Result = 0;
  byte      i;

  i = 64;
  do {
    Result  += ADC; 
  } while (--i);
}

void setup() {
    PORTB = 0;      //Reset values on port B

    // After setting up the timer counter, 
    // set the direction of the ports to output
    DDRB = (1<<PB1); // Set the direction of PB1 to an output

    // PLLCSR - PLL control and status register:
    // PLL is a clock multiplier - multiplies system     8Mhz by 8 to 64Mhz
    // PLL is enabled when:PLLE bit is enabled, 
    // CKSEL fuse is programmed to 0001.  This clock is 
    //   switched off in sleep modes!
    PLLCSR |= (1<<PLLE);    // PLL enable

    // Wait until the PLOCK bit is enabled 
    //  before allowing the PCK to be enabled
    while ((PLLCSR & (1<<PLOCK)) == 0x00)
    {
        // Do nothing until plock bit is set
    }

    PLLCSR |= (1<<PCKE); // Enable asynchronous mode, sets PWM clock source


    TCCR1 =
            (1<<CTC1) |  //enable pwm
            (1<<PWM1A) | // set source to pck
            (1<<(CS10)) | // clear the pin when match with ocr1x
            (1<<COM1A1);


    // Set PWM TOP value - counter will count and reset
    //  after reaching this value
    //          OCR1C
    // 400Khz   159     
    // 450khz   141
    // 500khz   127
    OCR1C = 255;

    //This should set the duty cycle to about 75%
    OCR1A = 12;

    MySerial.begin(9600);
    MySerial.txMode();
}

unsigned MaxVoltage = 30000;

void loop() {
  static unsigned LastCurrent;
  unsigned        Current, Voltage;

  Current = ReadADC(0); // Read current;
  if (Current<LastCurrent) Direction  = -Direction;

  Voltage = ReadADC(1);
  if (Voltage>MaxVoltage) Direction = -1;
  
  if (OCR1A==0) Direction = 1;
  if (OCR1A==0xFF) Direction = -1;
  OCR1A += Direction;
//  OCR1A++;

  MySerial << "I: " << Current << "  V: " << Voltage << " PWM: " << OCR1A << endl;
  delay(100);
}
