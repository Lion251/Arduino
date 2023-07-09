/*
  06/01/2016
  Author: Makerbro
  Platforms: ESP8266
  Language: C++
  File: SSD1306.cpp
  ------------------------------------------------------------------------
  Description: 
  SSD1306 OLED Driver Library.
  ------------------------------------------------------------------------
  Please consider buying products from ACROBOTIC to help fund future
  Open-Source projects like this! We'll always put our best effort in every
  project, and release all our design files and code for you to use. 
  https://acrobotic.com/
  ------------------------------------------------------------------------
  License:
  Released under the MIT license. Please check LICENSE.txt for more
  information.  All text above must be included in any redistribution. 
*/

#include "SSD1306_LargeFonts.h"

/*
 * een TFontStretcher krijgt een class mee met een "Get" functie die een volgende verticale lijn
 * van het karakter levert, en een boolean die aangeeft of dit nog lukt.
 * T1 is het type dat binnenkomt, T2 het 'uitgerekte' type dat gemaakt wordt.
 */

struct TTextProvider {
	virtual byte Get(uint8_t &c);
};

struct TCharProvider : public TTextProvider {
	byte	ch, Done;
	TCharProvider(char aCh) : ch(aCh), Done(0) { };
	virtual byte Get(uint8_t &c);
};


byte TCharProvider::Get(uint8_t &c) {
	if (Done) return 0; 
	
	c=ch; 
	Done=1; 
	return 1; 
};

struct TTextReader {
	static uint8_t	N;
	static TTextProvider *Provider;
	TTextReader(TTextProvider *aProvider) { Provider = aProvider; };
	byte static Get(uint8_t &New);
};

byte TTextReader::Get(uint8_t &New) {
	uint8_t	ch;
	if (N==0) {
		if (!Provider->Get(ch)) return 0;
	}
	return 1;
}

template <typename TFrom, typename TTo> // TTo moet dubbel zo groot zijn als TFrom
TTo Stretch(TFrom x) {
	TTo	y=0;

	for (uint8_t i=0; i<8*sizeof(TFrom); i++) {
		if (x & (1<<i)) y |= (1<<2*i);
	}
	return y | ((y<<1) & (y>>1));
};


template <typename T1, typename T2, typename T3> 
struct TFontStretcher {
	static uint8_t	N;
	static T3	Parent;
	static T2 	X1, X2, X3, X4, X5;
	TFontStretcher(void)	{ N=0;	X2=0; }
	void static Fill(void);
	byte static Get(T2 &New);
	byte static GetStretched(T2 &New);	// gets a byte from Parent and stretch it
};

template <typename T1, typename T2, typename T3>
void inline TFontStretcher<T1, T2, T3>::Fill(void) {
	if (N==0) {	// alleen bij nieuw of einde. Normaal is 3 en aanvullen tot 5.
		if (!GetStretched(X1)) return;
		N=1;
		if (!GetStretched(X3)) return;
		N=3; // nog wel X2 invullen
	}
	if (N==3) {
		T2	Schuin;
		if (GetStretched(X5)) {
			// lange lijnen vullen in X2 en X4
			X2 = X4 = X1 & X3 & X5;	// lange horizontale lijn; heeft voorrang op schuin.
			
			Schuin	= (X1>>2) & X3 & (X5<<2) & ~X2;
			X2 |= Schuin>>1;
			X4 |= Schuin<<1;

			Schuin	= (X1<<2) & X3 & (X5>>2) & ~X2;
			X2 |= Schuin<<1;
			X4 |= Schuin>>1;
		}
		// X2 invullen met korte lijnen. 
		// Als er al iets in X2 staat is dat een lange lijn.
		// Die hebben voorrang

		// Trek een korte horizontale lijn als er geen aanliggende punten al ingevuld zijn
		X2	|= X1 & X3 & ~(X2>>1 | X2<<1);
		
		// Trek een korte diagonale lijn als er geen aanliggende punten al ingevuld zijn.
		X2	|= ((X1<<1 & X3>>1) | (X1>>1 & X3<<1)) & ~(X2>>1 | X2<<1);

	}

};

template <typename T1, typename T2, typename T3>
byte TFontStretcher<T1, T2, T3>::Get(T2 &New) {
	Fill();
	switch (N) {
		case 0:			return 0;
		case 5:			// same as case 1
		case 3: 		// same as case 1
		case 1:	New=X1;		break;
		case 4: X2=X4;	X3=X5;	// continue as with case 2	
		case 2: New=X2; X1=X3;	break;
	}
	N--;
	return 1;
};

template <typename T1, typename T2, typename T3>
byte TFontStretcher<T1, T2, T3>::GetStretched(T2 &New) {
	T1	x;

	if (!Parent.Get(x)) return 0;
	New	= Stretch<T1, T2>(x);
	return 1;
};

typedef TFontStretcher<uint8_t, uint16_t, TTextReader>		TTextReader16;
typedef TFontStretcher<uint16_t, uint32_t, TTextReader16>	TTextReader32;

void SSD1306::test(void) {
	uint32_t val;
	TCharProvider	P('e');

	{
		uint8_t	c;

		beginData();
		while (P.Get(c)) {
			sendByte(c);
		}
		endTransmission();
	}
/*
	TTextReader::Provider	= &P;

	TTextReader32	t3;
	
	beginData();
	while (t3.Get(val)) {
		sendByte(val);
		sendByte(val>>8);
		sendByte(val>>16);
		sendByte(val>>24);
	}
	endTransmission();
*/
}


byte const PROGMEM SSD1306::InitCode[] = {
  0xAE,		//display off
  0xA6,            //Set Normal Display (default)
//  0xAE,            //DISPLAYOFF
  0xD5,            //SETDISPLAYCLOCKDIV
  0x80,            // the suggested ratio 0x80
  0xA8,            //SSD1306_SETMULTIPLEX
  0x3F,
  0xD3,            //SETDISPLAYOFFSET
  0x0,             //no offset
  0x40|0x0,        //SETSTARTLINE
  0x8D,            //CHARGEPUMP
  0x14,
//  0x20,            //MEMORYMODE
//  0x00,            //0x0 act like ks0108
  0xA1,            //SEGREMAP   Mirror screen horizontally (A0)
  0xC8,            //COMSCANDEC Rotate screen vertically (C0)
  0xDA,            //0xDA
  0x12,            //COMSCANDEC
  0x81,            //SETCONTRAST
  0xCF,            //
  0xd9,            //SETPRECHARGE 
  0xF1, 
  0xDB,            //SETVCOMDETECT                
  0x40,
  0xA4,            //DISPLAYALLON_RESUME        
  0xA6,            //NORMALDISPLAY 
  0x20,		// Display mode ...
  0x01,		// ... vertical
//  0x2E,	// Stop scroll
  0xAF,		// display on
};

void SSD1306::init(void)
{ int	i;

  for (i=0; i<sizeof(InitCode); i++) {
  	beginCommands();
	sendByte(pgm_read_byte(&InitCode[i]));
  	endTransmission();
  }

  clearDisplay();
}

void SSD1306::setFont(const uint8_t* font)
{
  m_font = font;
  m_font_width = pgm_read_byte(&m_font[0]);
}


void SSD1306::beginCommands(void)
{
  Wire.beginTransmission(SSD1306_Address);    // begin I2C communication
  Wire.write(SSD1306_Command_Mode);           // Set OLED Command mode
}


void SSD1306::beginData(void)
{
  Wire.beginTransmission(SSD1306_Address);    // begin I2C communication
  Wire.write(SSD1306_Data_Mode);           // Set OLED Command mode
}


void SSD1306::sendCommand(unsigned char command)
{
  beginCommands();
  sendByte(command);
  endTransmission();                       // End I2C communication
}


void SSD1306::setBrightness(unsigned char Brightness)
{
   sendCommand(SSD1306_Set_Brightness_Cmd);
   sendCommand(Brightness);
}


void SSD1306::setTextXY(unsigned char row, unsigned char col)
{
  setWindow(col, row, 0, 2);
//    sendCommand(0xB0 + row);                          //set page address
//    sendCommand(0x00 + (m_font_width*col & 0x0F));    //set column lower addr
//    sendCommand(0x10 + ((m_font_width*col>>4)&0x0F)); //set column higher addr
}

void SSD1306::clearDisplay() {
  int i, j;

  sendCommand(SSD1306_Display_Off_Cmd);     //display off

  for (i=0; i<128; i++) {
    setWindow(i, 0, 0, 0);
    beginData();
    for (j=0; j<8; j++) sendByte(0);
    endTransmission();				// stop I2C transmission
  }

  sendCommand(SSD1306_Display_On_Cmd);     //display on
  setTextXY(0,0);    
}


void SSD1306::setWindow(	unsigned char x,	// in pixels
	       				unsigned char y,	// in bytes
					unsigned char w,	// in pixels
					unsigned char h		// in bytes
				) {
  beginCommands();
  
  sendByte(0x21);	// Set column addresses ...
  sendByte(x);		// ... start address
  sendByte(w ? x+w-1 : SSD1306_Max_X);	// ... end address

  sendByte(0x22);	// Set row addresses ...
  sendByte(y);		// ... start address
  sendByte(h ? y+h-1 : SSD1306_Max_Y);	// ... end address

  endTransmission();
}


void SSD1306::sendData(unsigned char Data)
{
     Wire.beginTransmission(SSD1306_Address); // begin I2C transmission
     Wire.write(SSD1306_Data_Mode);            // data mode
     Wire.write(Data);
     Wire.endTransmission();                    // stop I2C transmission
}


uint16_t ByteToWord(uint8_t x) {
	return Stretch<int8_t, int16_t>(x);
}

int32_t WordToLong(uint16_t x) {
	uint32_t	y=0;

	for (uint8_t i=0; i<16; i++) {
		if (x & (1<<i)) y|=(1<<2*i);
	}
	return y | ((y<<1) & (y>>1));
}


void SSD1306::putBar(unsigned char lineNo, unsigned char length) {
	unsigned char	x;

	setWindow(0, lineNo, 0, 1);
	for (x=0; x<length; x++) sendData(0x18);
	for ( ; x<128; x++) sendData(0);
}


bool SSD1306::putChar(unsigned char ch) {
    const uint8_t	*Pt;
    uint8_t		NextByte;
    uint16_t		x1, x2, x3,	// uitgerekt
		       	X2, X3,		// verticaal verdubbeld
			X_2, X_3;	// wat er uiteindelijk op scherm komt

    if (!m_font) return 0;
    //Replace non-printable ASCII characters by space. This can be modified for
    //multilingual font.  
    if (ch < 32 || ch > 129) {
        ch = ' ';
    }

       // Font array starts at 0, ASCII starts at 32
    Pt	= &m_font[(((uint16_t)ch)-32)*m_font_width+m_font_offset];

    NextByte	= pgm_read_byte(Pt++);
    x3	= ByteToWord(NextByte & 0x7F);
    X3	= x3;
    if (Mode & SSD1306_Bold) X3 |= (x3<<1);

    beginData();
    sendByte(lowByte(X3));
    sendByte(highByte(X3));
    endTransmission();

    for (unsigned char i=0; i<m_font_width-1 && !(NextByte & 0x80); i++) {
	x1	= x3;
	NextByte= pgm_read_byte(Pt++);
	x3	= ByteToWord(NextByte & 0x7F);
	x2	= (x1 & x3) | ( (~(x1 | x3)) & (((x1>>1) & (x3<<1)) | ((x1<<1) & (x3>>1)))) ;

	if (Mode & SSD1306_Bold) {
		X2	= x2 | (x2<<1);
		X_2	= X2 | X3;	// de "Oude" X3, dus eig X1 
		X3	= x3 | (x3<<1);
	        X_3	= X3 | X2;
	} else {
		X_2	= x2;
		X_3	= x3;
	}	
 
	beginData();
	if (!(Mode & SSD1306_Compressed)) {
		sendByte(lowByte(X_2)); 
		sendByte(highByte(X_2));
		sendByte(lowByte(X_3)); 
		sendByte(highByte(X_3));
	} else {
		sendByte(lowByte(X_2|X_3)); 
		sendByte(highByte(X_2|X_3));
	}
        endTransmission();	
    }

    beginData();
    if (Mode & SSD1306_Bold) {
	    sendByte(lowByte(X3));
	    sendByte(highByte(X3));
	    sendByte(0);
	    sendByte(0);
    }

    sendByte(0);
    sendByte(0);

    endTransmission();
}

void SSD1306::putFlashPascalString(const char *s) {
	unsigned char i, Blank, Len=pgm_read_byte(s++);
	uint8_t	OldMode=Mode;

	if ((Mode & SSD1306_Autocompress) && (Len>12)) Mode |= SSD1306_Compressed;

	Blank	= 128 - Len * (Mode & SSD1306_Compressed ? 6 : 10);

	for (i=Blank; i--; ) sendData(0);

	for (i=Len; i--; ) {
		putChar(pgm_read_byte(s++));
	}

	for (i=Blank; i--; ) sendData(0);

	Mode	= OldMode;

}

void SSD1306::putString(const char *string)
{
    unsigned char i=0;
    while(string[i])
    {
        putChar(string[i]);     
        i++;
    }
}

void SSD1306::putString(String string)
{
    putString(string.c_str());
}

unsigned char SSD1306::putNumber(long long_num)
{
  unsigned char char_buffer[10]="";
  unsigned char i = 0;
  unsigned char f = 0;

  if (long_num < 0) 
  {
    f=1;
    putChar('-');
    long_num = -long_num;
  } 
  else if (long_num == 0) 
  {
    f=1;
    putChar('0');
    return f;
  } 

  while (long_num > 0) 
  {
    char_buffer[i++] = long_num % 10;
    long_num /= 10;
  }

  f=f+i;
  for(; i > 0; i--)
  {
    putChar('0'+ char_buffer[i - 1]);
  }
  return f;

}

unsigned char SSD1306::putFloat(float floatNumber,unsigned char decimal)
{
  unsigned int temp=0;
  float decy=0.0;
  float rounding = 0.5;
  unsigned char f=0;
  if(floatNumber<0.0)
  {
    putString("-");
    floatNumber = -floatNumber;
    f +=1;
  }
  for (unsigned char i=0; i<decimal; ++i)
  {
    rounding /= 10.0;
  }
    floatNumber += rounding;
  
  temp = floatNumber;
  f += putNumber(temp);
  if(decimal>0)
  {
    putChar('.');
    f +=1;
 }
  decy = floatNumber-temp;//decimal part, 
  for(unsigned char i=0;i<decimal;i++)//4 
  {
    decy *=10;// for the next decimal
    temp = decy;//get the decimal
    putNumber(temp);
    decy -= temp;
  }
  f +=decimal;
  return f;
}


unsigned char SSD1306::putFloat(float floatNumber)
{
  unsigned char decimal=2;
  unsigned int temp=0;
  float decy=0.0;
  float rounding = 0.5;
  unsigned char f=0;
  if(floatNumber<0.0)
  {
    putString("-");
    floatNumber = -floatNumber;
    f +=1;
  }
  for (unsigned char i=0; i<decimal; ++i)
  {
    rounding /= 10.0;
  }
    floatNumber += rounding;
  
  temp = floatNumber;
  f += putNumber(temp);
  if(decimal>0)
  {
    putChar('.');
    f +=1;
 }
  decy = floatNumber-temp;//decimal part, 
  for(unsigned char i=0;i<decimal;i++)//4 
  {
    decy *=10;// for the next decimal
    temp = decy;//get the decimal
    putNumber(temp);
    decy -= temp;
  }
  f +=decimal;
  return f;
}


void SSD1306::drawBitmap(unsigned char *bitmaparray,int bytes)
{
//  char localAddressMode = addressingMode;
//  if(addressingMode != HORIZONTAL_MODE)
//  {
//      //Bitmap is drawn in horizontal mode     
//      setHorizontalMode();
//  }

  for(int i=0;i<bytes;i++)
  {
      sendData(pgm_read_byte(&bitmaparray[i]));
  }

//  if(localAddressMode == PAGE_MODE)
//  {
//     //If pageMode was used earlier, restore it.
//     setPageMode(); 
//  }
  
}


void SSD1306::setHorizontalScrollProperties(bool direction,unsigned char startPage, unsigned char endPage, unsigned char scrollSpeed)
{
   if(Scroll_Right == direction)
   {
        //Scroll right
        sendCommand(0x26);
   }
   else
   {
        //Scroll left  
        sendCommand(0x27);

   }
    sendCommand(0x00);
    sendCommand(startPage);
    sendCommand(scrollSpeed);
    sendCommand(endPage);
    sendCommand(0x00);
    sendCommand(0xFF);
}

void SSD1306::activateScroll()
{
    sendCommand(SSD1306_Activate_Scroll_Cmd);
}

void SSD1306::deactivateScroll()
{
    sendCommand(SSD1306_Dectivate_Scroll_Cmd);
}

void SSD1306::setNormalDisplay()
{
    sendCommand(SSD1306_Normal_Display_Cmd);
}

void SSD1306::setInverseDisplay()
{
    sendCommand(SSD1306_Inverse_Display_Cmd);
}


SSD1306 oled;  // Pre-instantiate object
