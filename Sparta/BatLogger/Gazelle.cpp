#include <LiquidCrystal_I2C.h>
#include <SdFat.h>

#include "Gazelle.h"

/* Refresh Gazelle accu:
 *  
 *  Opladen tot RSA & 0x0070 == 0x0050 (RSA=0x00DC / 0x805C)
 *  Ontladen tot RSA == 0x002E
 *  herhalen
 *  Eindig met opladen.
 *  
 *  gCycles=3: opladen
 *  gCycles=2: ontladen
 *  gCycles=1: opladen
 *  
 */

const int ChargeRelay=A0, DischargeRelay=A1, ResistorRelay=A2, OPEN=1, CLOSED=0;



extern ArduinoOutStream		Cmd, Log, LCD;
extern int32_t Now, LastWrite;
extern SdFile	file;
extern char s[80];
extern LiquidCrystal_I2C	lcd;


void PrintHex(byte ch) {
	file.print(' ');
	if ((ch&0xF0)==0) file.print('0');
	file.print(ch, HEX);
}


void TGazelle::Action(void) {
	static int32_t	LastSend;
	static int32_t ChargeAt = 0, DischargeAt=0;	// Start charging 20 seconds after start

	Now=millis();
	if (Now-LastWrite > 15000) {	// Write data to card when more than 2 seconds inactive
		file.sync();
		LastWrite	= Now;
	}

	if (Now-LastSend>200) {	// Check every 200 ms if we have to send something
		static byte	SendState;
		
		switch (SendState++) {
			case 0:	SendCmd(F("RSA"));	break;
			case 1:	SendCmd(F("RUA")); 	break;
			case 2:	SendCmd(F("RIA")); 	break;
			case 3:	SendCmd(F("RKR")); 	break;
			case 4: SendCmd(F("RKF"));	break;
			case 5:	SendCmd(F("RT1"));	break;
			case 6:	SendCmd(F("RT2")); 	break;
			case 7:	SendCmd(F("RL ")); 	break;
			case 9:
				if (!gCycles) break;
				if (gCycles & 1)	{ //Charge until full
					if (RSA & 0x0040)	{	// Battery controller says "full"
						Charge(0);
						if (--gCycles) {
							DischargeAt	= Now + 600000L;	// Cool off
						}
					}
				} else {
					if ((RSA & 0x0F) == 0x0F) {	// Battery controller says "empty"
						--gCycles;
						Discharge(0);
						ChargeAt	= Now + 600000L;	// Cool off
					}
				}
				
				if (ChargeAt && (Now-ChargeAt>0)) {
					ChargeAt	= 0;
					Charge(1);
				}

				if (DischargeAt && (Now-DischargeAt>0)) {
					DischargeAt	= 0;
					Discharge(1);
				}

				break;
			case 49:	SendState=0;	break;
		}
		LastSend	+= 200;
	}
	HandleResponse();

}


void TGazelle::Charge(byte On) {
	if (On) {
		digitalWrite(ChargeRelay, CLOSED);
		delay(1000);
		digitalWrite(ResistorRelay, CLOSED);
		delay(100);
	} else {
		digitalWrite(ChargeRelay, OPEN);
		delay(100);
		digitalWrite(ResistorRelay, OPEN);
		delay(100);
	}
}


void TGazelle::Discharge(byte On) {
	if (On) {
		digitalWrite(DischargeRelay, CLOSED);
		delay(1000);
		digitalWrite(ResistorRelay, CLOSED);
		delay(100);
	} else {
		digitalWrite(DischargeRelay, OPEN);
		delay(100);
		digitalWrite(ResistorRelay, OPEN);
		delay(100);
	}	
}


void TGazelle::DisplayBatteryInfo(void) {
  lcd.clear();
  SendCmd(F("RV1"));	// Version
  delay(100);
  HandleResponse();
  UpdateDisplay(s);

  SendCmd(F("RB "));	// Serial no
  delay(100);
  HandleResponse();
  UpdateDisplay(s);

  SendCmd(F("RB1"));	// Serial no 2
  delay(100);
  HandleResponse();
  UpdateDisplay(s);	
}

void TGazelle::HandleResponse(void) {
	static byte	State, Checksum, Length, PrintMillis, LED=1, From, To;

	while (Serial1.available()) {
		byte	ch	= Serial1.read();
		if (State==0) {	// Hunt for start of text, 0xFE
			if (ch==0xFE) {
				LED	= !LED;
				digitalWrite(17, LED);
				if (PrintMillis) {
					file.print(millis()/1000);
					PrintMillis	= 0;
				}
				Checksum	= 0;
				Length		= 0;
				State++;
			}
		} else { 
			if (State==1) {
				To	= ch;
			} else if (State==2) {
				From	= ch;
			} else if (State==3) {
				Length	= ch;
			} else if (State==Length-2) {
				Checksum ^= ch;
				if (Checksum!=0)	{
					Log << pstr(" ** Checksum: ");
					PrintHex(Checksum);
				}
			} else if (State==Length-1) {
				if (ch!=0xFD) {
					Log << pstr(" ** EOL: ");
					PrintHex(ch);
				}
				s[Length-6]	= 0;
				if (From==0 && To==2) {
					UpdateDisplay(s);
					Log << ',' << s+3;
					if (!strncmp_P(s, PSTR("RL"),2) || !strncmp_P(s, PSTR("RB1"), 3)) {
						file.println();
						PrintMillis	= 1;
					}
				}
				State=-1;
			} else { // 'ordinary' data
				Checksum ^= ch;
				if (State<sizeof(s))	s[State-4]	= ch;
			}
			State++;
		}
	}
}


void TGazelle::IncreaseCycles(void) {
	if (gCycles) gCycles++;
	gCycles++;
}


void TGazelle::Init(void) {
	Charge(0);
	Discharge(0);
	pinMode(ChargeRelay, OUTPUT);
	pinMode(DischargeRelay, OUTPUT);
	pinMode(ResistorRelay, OUTPUT);
}
	

void TGazelle::SendCmd(const __FlashStringHelper *Command) {
	int	i, Len;
	char	ch, Checksum;

	Len		= strlen_P((const char *)Command);
	Checksum	= 0;

	Cmd << (char)0xFE << (char)0x00 << (char)0x02 << (char)(Len+6);
	for	(i=0; i<Len; i++) {
		ch	= pgm_read_byte_near((const char *)Command+i);
		Checksum	^= ch;
		Cmd << ch;
	}
	Cmd << Checksum << (char)0xFD;

}


void TGazelle::UpdateDisplay(const char *s) {
	static uint16_t	Remaining;
	uint16_t	Capacity, Percent;

	if (!strncmp_P(s, PSTR("RUA"), 3)) {		// Battery voltage
		lcd.setCursor(0,0);
		LCD << s[4] << s[5] << '.' << s[6] << s[7] << s[8] << 'V' << ' '; 
	} else if (!strncmp_P(s, PSTR("RIA"), 3)) {	// Battery current
		lcd.setCursor(8,0);
		LCD << s[3] << s[6] << '.' << s[7] << s[8] << 'A' << ' ' << ' ';		
	} else if (!strncmp_P(s, PSTR("RSA"), 3)) {	// Battery charge state
		lcd.setCursor(8,0);
		sscanf(s+3, "%x", &RSA);
		//LCD << s+3;		
	} else if (!strncmp_P(s, PSTR("RKR"), 3)) {	// Remaining capacity in mAh
		Remaining	= atoi(s+5);
	} else if (!strncmp_P(s, PSTR("RKF"), 3)) {	// 'Full' capacity in mAh
		lcd.setCursor(0,1);
		LCD << (s[5]!='0' ? s[5] : ' ') << s[6] << '.' << s[7] << s[8] << pstr("Ah ");
		Capacity	= atoi(s+5);
		Percent		= 1000L * Remaining / Capacity;
		lcd.setCursor(8,1);
		LCD << Percent/10 << '.' << Percent % 10 << pstr("% ") << (int)gCycles << ' ';
	} else if (!strncmp_P(s, PSTR("RV1"), 3)) {	// Version
		lcd.setCursor(8,0);
		LCD << s+3;
	} else if (!strncmp_P(s, PSTR("RB "), 3)) {	// Serial
		lcd.setCursor(0,0);
		LCD << s+4;
	} else if (!strncmp_P(s, PSTR("RB1"), 3)) {	// Serial 2
		lcd.setCursor(0,1);
		LCD << s+4;
	}
}



