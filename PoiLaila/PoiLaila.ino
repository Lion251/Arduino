 #include <util/atomic.h>
#include "avr/sleep.h"

const unsigned PROGMEM Table[257]	= {
	0,
	64,	256,	262,	267,	273,	279,	285,	292,	298,	305,	311,	318,	325,	332,	340,	347,
	355,	362,	370,	379,	387,	395,	404,	413,	422,	431,	441,	450,	460,	470,	481,	491,
	502,	513,	524,	536,	548,	560,	572,	585,	597,	611,	624,	638,	652,	666,	681,	696,
	711,	727,	742,	759,	775,	792,	810,	828,	846,	864,	883,	903,	923,	943,	964,	985,
	1006,	1029,	1051,	1074,	1098,	1122,	1147,	1172,	1198,	1224,	1251,	1278,	1306,	1335,	1364,	1394,
	1425,	1456,	1488,	1521,	1554,	1588,	1623,	1659,	1695,	1733,	1771,	1810,	1849,	1890,	1932,	1974,
	2017,	2062,	2107,	2153,	2201,	2249,	2298,	2349,	2400,	2453,	2507,	2562,	2618,	2676,	2735,	2795,
	2856,	2919,	2983,	3049,	3116,	3184,	3254,	3325,	3399,	3473,	3549,	3627,	3707,	3789,	3872,	3957,
	4044,	4133,	4223,	4316,	4411,	4508,	4607,	4708,	4812,	4917,	5025,	5136,	5249,	5364,	5482,	5602,
	5725,	5851,	5979,	6111,	6245,	6382,	6523,	6666,	6812,	6962,	7115,	7271,	7431,	7594,	7761,	7931,
	8106,	8284,	8466,	8652,	8842,	9036,	9234,	9645,	9857,	10073,	10294,	10521,	10752,	10988,	11229,	11476,
	11728,	11986,	12249,	12518,	12793,	13074,	13361,	13655,	13955,	14261,	14575,	14895,	15222,	15556,	15898,	16247,
	16604,	16969,	17342,	17723,	18112,	18510,	18917,	19332,	19757,	20191,	20635,	21088,	21551,	22025,	22509,	23003,
	23508,	24025,	24553,	25092,	25643,	26206,	26782,	27371,	27972,	28586,	29214,	29856,	30512,	31182,	31867,	32567,
	33283,	34014,	34761,	35525,	36305,	37107,	37918,	38751,	39602,	40472,	41361,	42270,	43199,	44148,	45117,	46109,
	47121,	48157,	49215,	50296,	51401,	52530,	53684,	54863,	56068,	57300,	58559,	59845,	61160,	62504,	63877,	65280
};


unsigned GetExp(unsigned Val) {
	unsigned	Low	= pgm_read_word( &Table[highByte(Val)]);
	unsigned	High	= pgm_read_word( &Table[highByte(Val)+1]);
	return Low + (unsigned long)(High - Low) * lowByte(Val) / 256;
}


void setup() {
/*
 * HiResPWM loopt op 4 kHz voor kanaal 0 (Timer0 met x8 prescaler), 4/8/16/32 kHz voor kanaal 1 en 2 
 * 
 * 0:	Check in. 0: Next 1: => 0			; Check Start Bit
 * 1:	Check in. 0: Next 1: => 0
 * 2:	Reg=1. Next					; Prepare
 * 3:	Next						; Next
 * 4:	C<-Reg<-In. C? +> 6 !C? Next			; ShiftIn
 * 5:	=> 3						; NextBit
 * 6:	Next
 * 7:	Check in. 1: Store Reg. 0: Framing Error. => 0	; CheckStop
 */
	PORTB	= 0;		//Reset values on port B
	DIDR0	= 0x3F;		// no digital inputs

	// After setting up the timer counter, 
	// set the direction of the ports to output
	DDRB = 0x1F; //(1<<PB1) | (1<<PB0); // Set the direction of PB1 to an output

	TCCR0A =
		(3<<COM0A0) |	// Inverted mode
		(3<<WGM00);	// Mode 3: PWM 0-MAX. WGM02 must NOT be set!

	TCCR1 =
            (0<<CTC1) | 	// Not normal counter timer clear
            (1<<PWM1A) |	// set PWM mode
            (1<<(CS10)) |	// Set prescaler to 1 for 32 kHz
            (3<<COM1A0);	// Set pin on match with OCR1A. Inverted mode, so we can get pulse width of 0

	GTCCR =
            (1<<PWM1B) |	// set PWM mode
            (3<<COM1B0);	// Set pin on match with OCR1B. Inverted mode, so we can get pulse width of 0


	// Set PWM TOP value - counter will count and reset
	OCR1C = 255;
	TIMSK |= (1 << TOIE1);

	set_sleep_mode(SLEEP_MODE_IDLE);	// sleep mode is set here
	sleep_enable();				// enables the sleep bit in the mcucr register
}

void loop2(void) {
	static unsigned i;
	HiResPWM(1, GetExp(i++));
	delay(1);
}

// 06-15130251 Overbuurvrouw mama
void loop() {
	unsigned	Val[3]	= { 256, 256, 256 };
	int		dVal[3]	= {  137,  29,  31 };
	unsigned long	DisplayVal[3];
	byte		i;
	unsigned long	SumVal;

	for (;;) {
		SumVal	= 0;
		for (i=0; i<3; i++) {
			Val[i]	+= dVal[i];
			if (Val[i]<256 || Val[i]>65000) dVal[i]=-dVal[i];
			DisplayVal[i]	= GetExp(Val[i]);
			SumVal	+= DisplayVal[i];
		}
		for (i=0; i<3; i++) {
			HiResPWM(i, DisplayVal[i] * 65536UL / SumVal);
		}
		delay(150);
	}
}
