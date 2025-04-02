#include "Delay.h"
#include "avr/interrupt.h"
#include "avr/power.h"
#include "avr/sleep.h"
#include "EEPROM.h"

unsigned PROGMEM const DelayTime[32] = {
  2560, 2153, 1810, 1522, 1280, 1076,  905,  761, 
   640,  538,  453,  381,  320,  269,  226,  190,
   160,  135,  113,   95,   80,   67,   57,   48,
    40,   34,   28,   24,   20,   17,   14,   12
};
  
byte ColorVal[5];  // variables to equalize the colors (yellow will need more current than blue) Not higher than 25!
byte SpeedVal;      // Set the global display speed (0==slow, 31==fast)
byte FadeN;


byte ddr0,ddr1,ddr2,ddr3,ddr4,ddr5,port0,port1,port2,port3,port4,port5;
#define nop() __asm__ __volatile__ ("nop")


void Fade(byte b) {
  port3 = port2;
  port2 = port1;
  port1 = port0;
  port0 = 1 << (b>>5);
  ddr3  = ddr2;
  ddr2  = ddr1;
  ddr1  = ddr0;
  ddr0  = (b & 0x1F) | port0;
  for (byte i=0; i<33-SpeedVal; i++) {
    for (byte k=18-(SpeedVal>>1); k; k--) {
      noInterrupts();
      DDRB  = ddr0;
      PORTB = port0;
      for (byte j=i+1; j; j--) nop();    // 1..34-SpeedVal
      DDRB  = ddr1;
      PORTB = port1;
      for (byte j=i+i+(34-SpeedVal); j; j--) nop();  // 34-SpeedVal .. 100-3*SpeedVal
      DDRB  = ddr2;
      PORTB = port2;
      for (byte j=100-SpeedVal-SpeedVal-SpeedVal-i-i; j; j--) nop();
      DDRB  = ddr3;
      PORTB = port3;
      for (byte j=34-SpeedVal-i; j; j--) nop();
      PORTB = 0xFF;  // We end with all outputs high, all leds off
      interrupts();
    }
  }
}


void Fade2(byte b) {
  port5 = port4;
  port4 = port3;
  port3 = port2;
  port2 = port1;
  port1 = port0;
  port0 = 1;
  port0 <<= (b>>5);
  ddr5  = ddr4;
  ddr4  = ddr3;
  ddr3  = ddr2;
  ddr2  = ddr1;
  ddr1  = ddr0;
  ddr0  = (b & 0x1F) | port0;
  for (byte i=0; i<33-SpeedVal; i++) {
    for (byte k=18-(SpeedVal>>1); k; k--) {
      noInterrupts();
      DDRB  = ddr0;
      PORTB = port0;
      for (byte j=i+1; j; j--) nop();
      DDRB  = ddr1;
      PORTB = port1;
      for (byte j=i+i+33-SpeedVal; j; j--) nop();
      DDRB  = ddr2;
      PORTB = port2;
      for (byte j=100-SpeedVal-SpeedVal-SpeedVal; j; j--) nop();
      DDRB  = ddr3;
      PORTB = port3;
      for (byte j=100-SpeedVal-SpeedVal-SpeedVal; j; j--) nop();
      DDRB  = ddr4;
      PORTB = port4;
      for (byte j=100-SpeedVal-SpeedVal-i-i; j; j--) nop();
      DDRB  = ddr5;
      PORTB = port5;
      for (byte j=34-SpeedVal-i; j; j--) nop();
      PORTB = 0xFF;  // We end with all outputs high, all leds off
      interrupts();
    }
  }
}


byte KeyPressed(void) {
  DDRB   = 0;
  PORTB  = 0xFF;
  
  for (byte i=255; i!=0; i--) {
    if (PINB == 0x1F) return 0;
  }
  return 1;
}


void PowerOff(void) {
  EEPROM.write(0, 1);
  sleep_enable();
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_cpu();
}

const byte *RunProgram(const byte *pt) {
  byte  Command, Param;
  
  //Serial<<"pt:"<<pt-Main<<"\n";
  while (Command = pgm_read_byte(pt++)) {
    //Serial<<"Loop:"<<pt-Main<<"\n";
    if (KeyPressed()) return NULL;
    Param   = Command & 31;
    Command >>=5;
    switch (Command) {
      case CALL:  // Call subroutine
        RunProgram(Subroutines[Param]);
        break;
        
      case REPEAT:  // Repeat. Play a frame of ch bytes RepCnt times 
               // If Param==0, set ColorVal from extra byt. Upper 3 bits is channel, lower 5 is value
        { switch (Param) {
            case SETCOLOR:  // repeat 0 times is SetColorVal
              { byte Param2 = pgm_read_byte(pt++);
                byte Color  = Param2 >> 5;
                byte Val    = Param2 & 31;
                switch (Val) {
                  case UP:    ColorVal[Color]++; ColorVal[Color] &= 31; break;
                  case DOWN:  ColorVal[Color]--; ColorVal[Color] &= 31; break;
                  default:    ColorVal[Color] = Val; break;
                }
              }
              break;
              
            case CHAR:
              { byte ch = pgm_read_byte(pt++);
                for (byte i=0; i<TEXTDELAY; i++) {
                  if (!RunProgram(Charset + 6*ch)) return NULL;
                }
                if (!RunProgram(Charset + 6*_SPACE)) return NULL;
                delay(100);
              }
              break;
              
            default: // Normally, we repeat Param times
              { const byte *NewPt;
                do {
                  NewPt = RunProgram(pt); 
                  if (!NewPt) return NULL;
                } while (--Param);
                pt = NewPt;  // skip the instruction range we just repeated
              }
              break;
          }
        }
        break;
        
      case SPEED:  // Speed. SpeedVal can be set from 0..31 (logarithmic steps)
                   // Delay after displaying 'our' color is DelayTime[SpeedVal] * ColorVal[Color] / C
        { switch (Param) {
            case UP:    SpeedVal++;         break;
            case DOWN:  SpeedVal--;         break;
            case FADE:  FadeN = 1;          break;
            case NOFADE:FadeN = 0;          break;
            default:    SpeedVal = Param;   break;
          }
          SpeedVal  &=31;  // Keep SpeedVal in range
          //Serial<<"SetSpeed Param: "<<Param<<" SpeedVal: "<<SpeedVal<<"\n";
        }
        break;
        
      default:  // Commands 0..4 are lighting of a number of LEDs from the same color
        if (FadeN) {
          Fade2((Command<<5) | Param);
          if (PINB!=0x1F) {
            byte i;
            for (i=255; i; i++) if (PINB==0x1f) break;
            if (!i) return NULL;
          }
        } else { 
          unsigned long Delay;
          byte          Color = Command;
          Delay = (long)((Param==0 ? AVG_ONTIME : ColorVal[Color]) * pgm_read_word(&DelayTime[SpeedVal]))<<2; 
          //Serial<<Color<<" "<<Param<<" "<<SpeedVal<<" "<<Delay<<"\n";
          noInterrupts();
          PORTB  = (1<<Color);
          DDRB  = Param | (1<<Color);
          interrupts();
          
          if (Param) Command=1;                    // Command is used as delay counter for Off(). For 'normal' colors, this is 1
          while (Command--) delayRealMicroseconds(Delay);
          PORTB  = 0xFF;
        } 
        break;
    } 
  }
  return pt;  // pt now points 1 past the End/EndRep instruction
}

void setup(void) {
  //Serial.begin(115200);
  ADCSRA = 0;    // Disable AD converter
  power_adc_disable();
  power_usi_disable();
  // delay(1000);
  if (EEPROM.read(0)==0) PowerOff();    // Don't start until we press the button
  EEPROM.write(0, 0);
  pinMode(LED_BUILTIN, OUTPUT);
}

extern const byte PROGMEM Main[];
void loop(void) {
  digitalWrite(LED_BUILTIN, 0);
  delay(100);
  digitalWrite(LED_BUILTIN, 1);
  delay(1000);
//  RunProgram(Main);
}
