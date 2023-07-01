#include <avr/interrupt.h>

#define STR(X)		#X
#define SHIFT(R1,R2)	\
	"ror	" STR(R1) "\n"	\
	"ror	" STR(R2) "\n"

ISR(TIMER1_COMPA_vect) {
	asm volatile(
		// Haal pointer naar begin van juiste kolom
		// Per kolom van 7 leds zijn er 8x2 tikken nodig voor serie/parallel omzetting (16)
		// Dit per bit, dus 128 totaal
		// Laten we 512 tikken nemen voor de hele cyclus om minimaal 50% rekentijd over te houden buiten de interrupt. Dat is 2 tikken per LSB
		// 
		SHIFT(r2,r16)
		SHIFT(r3,r16)	
		SHIFT(r4,r16)
		SHIFT(r5,r16)
		SHIFT(r6,r16)
		SHIFT(r7,r16)
		SHIFT(r8,r16)
		SHIFT(r9,r16)	// 16
		// B0 is nu bekend (in r16), maar kunnen we nog niet uitgeven omdat B1 2 tikken later al moet komen, en die hebben we nog niet uitgerekend.
		SHIFT(r2,r17)
		SHIFT(r3,r17)
		SHIFT(r4,r17)
		SHIFT(r5,r17)
		SHIFT(r6,r17)
		SHIFT(r7,r17)
		SHIFT(r8,r17)
		SHIFT(r9,r17)	// 32
		// B1 in r17, maar B2 moet 4 tikken na B1 komen, dat redden we niet
		SHIFT(r2,r18)
		SHIFT(r3,r18)
		SHIFT(r4,r18)
		SHIFT(r5,r18)
		SHIFT(r6,r18)
		SHIFT(r7,r18)
		SHIFT(r8,r18)
		SHIFT(r9,r18)	// 48
		// B2 in r18, maar B3 uitrekenen in 8 tikken lukt nog steeds niet.
		SHIFT(r2,r19)
		SHIFT(r3,r19)
		SHIFT(r4,r19)
		SHIFT(r5,r19)
		SHIFT(r6,r19)
		SHIFT(r7,r19)
		SHIFT(r8,r19)
		SHIFT(r9,r19)	// 64
		// B3 in r19
		// Hier moeten we r21 nog even met de goede bitvector laden
		"ser	r20\n"	// 65
		"out	%[port], r20\n"		// alle leds uit om bleeding te voorkomen
		"out	%[ddr], r21\n"		// selecteer nieuw kolom
		"out	%[port], r16\n"	// B0	// zet leds aan/uit
		// vanaf hier kunnen we r16 hergebruiken voor B4
		"ror	r2\n"
		"out	%[port], r17\n"	// B1
		"ror	r16\n"
		SHIFT(r3,r16)
		"out	%[port], r18\n"	// B2
		SHIFT(r4,r16)
		SHIFT(r5,r16)
		SHIFT(r6,r16)
		"ror	r7\n"
		"out	%[port], r19\n"	// B3
		"ror	r16\n"
		SHIFT(r7,r16)
		SHIFT(r8,r16)
		SHIFT(r9,r16)
		// B4 in R16
		SHIFT(r2,r17)
		SHIFT(r3,r17)
		SHIFT(r4,r17)
		SHIFT(r5,r17)
		"out	%[port], r16\n"	// B4
		SHIFT(r6,r17)
		SHIFT(r7,r17)
		SHIFT(r8,r17)
		SHIFT(r9,r17)
		// B5 in r17
		SHIFT(r2,r18)
		SHIFT(r3,r18)
		SHIFT(r4,r18)
		SHIFT(r5,r18)
		SHIFT(r6,r18)
		SHIFT(r7,r18)
		SHIFT(r8,r18)
		SHIFT(r9,r18)
		// B6 in r18
		SHIFT(r2,r19)
		SHIFT(r3,r19)
		SHIFT(r4,r19)
		"ror	r5\n"
		"out	%[port], r17\n"	// B5
		"ror	r19\n"
		SHIFT(r6,r19)
		SHIFT(r7,r19)
		SHIFT(r8,r19)
		SHIFT(r9,r19)
		// B7 in r19
		
		:
		:
		[port] "I" (_SFR_IO_ADDR(PORTB)),
		[ddr] "I" (_SFR_IO_ADDR(DDRB))
		
	);
}

void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}
