#include <petit_fatfs.h>


byte rx()
{
        USIDR = 0xFF;
        USISR = _BV(USIOIF);
        do {
                USICR = _BV(USIWM0) | _BV(USICS1) |
                        _BV(USICLK) | _BV(USITC);
        } while ((USISR & _BV(USIOIF)) == 0);
        return USIDR;
}

void tx(byte d)
{
        USIDR = d;
        USISR = _BV(USIOIF);
        do {
                USICR = _BV(USIWM0) | _BV(USICS1) |
                        _BV(USICLK) | _BV(USITC);
        } while ((USISR & _BV(USIOIF)) == 0);
        
}

void spi_init()
{
/*
 * DON'T use the MOSI/MISO pins. They're for ISP programming only!
 * Read the datasheet.
 */
/*
// USI stuff
  DDRB |= _BV(PB1); // as output (latch)
  DDRA |= _BV(PA5); // as output (DO)
  DDRA |= _BV(PA4); // as output (USISCK)
  DDRA &= ~_BV(PA6); // as input (DI)
  PORTA |= _BV(PA6); // pullup on (DI)
  */
 // USICR = bit(USIWM0); //?????????????????????????????
} 



void setup()
{
  char * s = (char*)calloc(65, sizeof(char));
  
  spi_init();
  PFFS.begin(1, rx, tx);
  
  fileReadTest(PFFS.open_file("test.txt"), "test.txt");
  
}


void fileReadTest(int err, const char * fp)
{
  if (err == 0)
  {  
    char * s = (char*)calloc(65, sizeof(char));
    int bytes_read;
    int bytes_cnt = 0;
    
    do
    {
	PFFS.read_file(s, 64, &bytes_read);
	bytes_cnt += bytes_read;
    }
    while (bytes_read == 64);

    free(s);
    
  }
  else
  {
  }
}

void loop()
{
  
} 

