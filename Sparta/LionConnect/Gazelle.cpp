#include <Streaming.h>

#include "Gazelle.h"
#include <Arduino.h>
#include <SoftwareSerial.h>


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

SoftwareSerial	MySerial(0,1);

#define		Cmd	MySerial

char		s[80];

uint32_t	Now, LastSend;
#define Cmd	MySerial

void TGazelle::Action(void) {
	Now=millis();

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
					}
				} else {
					if ((RSA & 0x0F) == 0x0F) {	// Battery controller says "empty"
					}
				}
				break;
			case 49:	SendState=0;	break;
		}
		LastSend	+= 200;
	}
	HandleResponse();

}


void TGazelle::HandleResponse(void) {
	static byte	State, Checksum, Length, PrintMillis, LED=1, From, To;

	while (Serial.available()) {
		byte	ch	= Serial.read();
		if (State==0) {	// Hunt for start of text, 0xFE
			if (ch==0xFE) {
				LED	= !LED;
				digitalWrite(17, LED);
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
				}
			} else if (State==Length-1) {
				if (ch!=0xFD) {
				}
				s[Length-6]	= 0;
				if (From==0 && To==2) {
					if (!strncmp_P(s, PSTR("RL"),2) || !strncmp_P(s, PSTR("RB1"), 3)) {
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


void TGazelle::Init(void) {
	MySerial.begin(9600);
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
		BatVoltage	= atoi(s+4);
	} else if (!strncmp_P(s, PSTR("RIA"), 3)) {	// Battery current
		Current	= atoi(s+3);
	} else if (!strncmp_P(s, PSTR("RSA"), 3)) {	// Battery charge state
//		lcd.setCursor(8,0);
		sscanf(s+3, "%x", &RSA);
//		LCD << RSA;		
	} else if (!strncmp_P(s, PSTR("RKR"), 3)) {	// Remaining capacity in mAh
		Remaining	= atoi(s+5);
	} else if (!strncmp_P(s, PSTR("RKF"), 3)) {	// 'Full' capacity in mAh
//		lcd.setCursor(0,1);
//		LCD << (s[5]!='0' ? s[5] : ' ') << s[6] << '.' << s[7] << s[8] << pstr("Ah ");
		Capacity	= atoi(s+5);
		Percent		= 1000L * Remaining / Capacity;
//		lcd.setCursor(8,1);
//		LCD << Percent/10 << '.' << Percent % 10 << pstr("% ") << (int)gCycles << ' ';
	} else if (!strncmp_P(s, PSTR("RV1"), 3)) {	// Version
//		lcd.setCursor(8,0);
//		LCD << s+3;
	} else if (!strncmp_P(s, PSTR("RB "), 3)) {	// Serial
//		lcd.setCursor(0,0);
//		LCD << s+4;
	} else if (!strncmp_P(s, PSTR("RB1"), 3)) {	// Serial 2
//		lcd.setCursor(0,1);
//		LCD << s+4;
	}
}



