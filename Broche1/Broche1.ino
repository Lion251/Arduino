enum {	First5=0,	_M=First5, _N, _Q, _V, _W, _X, _Z, 
	First4,		_A=First4, _B, _C, _D, _E, _F, _G, _H, _J, _K, _L, _O, _P, _R, _S, _U, _Y, 
	First3,		_I=First3, 
	First2,		_COLON=First2, _SEMICOLON, _PERIOD, _COMMA, 
	First1,		_EXCL=First1, _SPACE 
};

const byte FontTable[] PROGMEM = {
	// M
	B11111110,
	B00001100,
	B00010000,
	B00001100,
	B11111111,
	// N
	B11111110,
	B00001100,
	B00010000,
	B01100000,
	B11111111,
	// Q
	B01111100,
	B10000010,
	B10100010,
	B01000010,
	B10111101,
	// T
	B00000010,
	B00000010,
	B11111110,
	B00000010,
	B00000011,
	// V
	B00001110,
	B00110000,
	B11000000,
	B00110000,
	B00001111,
	// W
	B01111110,
	B10000000,
	B01100000,
	B10000000,
	B01111111,
	// X
	B11000110,
	B00101000,
	B00010000,
	B00101000,
	B11000111,
	// Z
	B11000010,
	B10100010,
	B10010010,
	B10001010,
	B10000111,
	// A
	B11111100,
	B00100010,
	B00100010,
	B11111101,
	// B
	B11111110,
	B10010010,
	B10010010,
	B01101101,
	// C
	B01111100,
	B10000010,
	B10000010,
	B01000101,
	// D
	B11111110,
	B10000010,
	B10000010,
	B01111101,
	// E
	B11111110,
	B10010010,
	B10010010,
	B10000011,
	// F
	B11111110,
	B00010010,
	B00010010,
	B00000011,
	// G
	B01111100,
	B10000010,
	B10010010,
	B01110101,
	// H
	B11111110,
	B00010000,
	B00010000,
	B11111111,
	// J
	B01000000,
	B10000000,
	B10000000,
	B01111111,
	// K
	B11111110,
	B00010000,
	B00101000,
	B11000111,
	// L
	B11111110,
	B10000000,
	B10000000,
	B10000001,
	// O
	B01111100,
	B10000010,
	B10000010,
	B01111101,
	// P
	B11111110,
	B00010010,
	B00010010,
	B00001101,
	// R
	B11111110,
	B00010010,
	B00010010,
	B11101101,
	// S
	B01100100,
	B10010010,
	B10010010,
	B01100101,
	// U
	B01111110,
	B10000000,
	B10000000,
	B01111111,
	// Y
	B00001110,
	B10010000,
	B10010000,
	B01111111,
	// I
	B10000010,
	B11111110,
	B10000011,
	// :
	B01101100,
	B01101101,
	// ;
	B10101100,
	B01101101,
	// .
	B11000000,
	B11000001,
	// ,
	B10100000,
	B01100001,
	// !
	B10111111,
	// SPACE
	B00000001,
};

byte Buf[8][2];

void ShowBuf(const void *Buf, byte Duration) {
	asm volatile(
	"movw	r30,r24\n"
	"ldi	r25,0x80\n"
"l0:	ld	r24,Z+\n"
	"out	%[port],r24\n"
	"or	r24,r25\n"
	"mov	r0,r22\n"
	"cli\n"
	"out	%[ddr],r24\n"
"l1:	dec	r0\n"
	"brvc	l1\n"
	"out	%[ddr],r1\n"
	"sei\n"
	"lsr	r25\n"
	"brcc	l0\n"
	:
	:
	[port] "I" (_SFR_IO_ADDR(PORTB)),
	[ddr] "I" (_SFR_IO_ADDR(DDRB))
//	: r22, r24, r25, r0
	);
}

void Fade(byte Speed) {
	byte	i, j;

	for (j=1; j; j<<=1) {
		for (i=Speed; i; i--) {
			ShowBuf(Buf[1], -j);	// 255, 254, 252, 248, 240, 224, 192, 128
			ShowBuf(Buf[0], j);	// 1,   2,   4,   8,   16,  32,  64,  128
		}
	}
	for (j=0x40; j; j>>=1) {
		for (i=Speed; i; i--) {
			ShowBuf(Buf[1], j);	// 64,  32,  16,  8,   4,   2,   1
			ShowBuf(Buf[0], -j);	// 192, 224, 240, 248, 252, 254, 255
		}
	}

}

void ShiftInChar(byte ch) {
	unsigned	pt=0;

	pt	+= ch>First4 ? First4 : ch;
	pt	+= ch>First3 ? First3 : ch;
	pt	+= ch>First2 ? First2 : ch;
	pt	+= ch>First1 ? First1 : ch;
	pt	+= ch;

	do {
		ch	= pgm_read_byte(FontTable+pt++);
		Fade(1);
	} while (ch&1 != 0);
}

void setup() {
  // put your setup code here, to run once:

}

void loop() {
	ShiftInChar(_C);
  // put your main code here, to run repeatedly:

}
