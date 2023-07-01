//#include <Streaming.h>

/*
 * Simple data logger.
 */
#include <SPI.h>
#include <SdFat.h>

uint32_t	LastWrite;

// SD chip select pin.  Be sure to disable any other SPI devices such as Enet.
const uint8_t chipSelect = 3;

// Interval between data records in milliseconds.
// The interval must be greater than the maximum SD write latency plus the
// time to acquire and write data to the SD to avoid overrun errors.
// Run the bench example to check the quality of your SD card.
const uint32_t SAMPLE_INTERVAL_MS = 200;

// Log file base name.  Must be six characters or less.
#define FILE_BASE_NAME "Data"
//------------------------------------------------------------------------------
// File system object.
SdFat sd;

// Log file.
SdFile file;

// Time in micros for next data record.
uint32_t logTime;

//==============================================================================
// User functions.  Edit writeHeader() and logData() for your requirements.

const uint8_t ANALOG_COUNT = 4;


//------------------------------------------------------------------------------
// Write data header.
void writeHeader() {
  file.print(F("micros"));
  for (uint8_t i = 0; i < ANALOG_COUNT; i++) {
    file.print(F(",adc"));
    file.print(i, DEC);
  }
  file.println();
}


//------------------------------------------------------------------------------
// Log a data record.
void logData() {
  uint16_t data[ANALOG_COUNT];

  // Read all channels to avoid SD write latency between readings.
  for (uint8_t i = 0; i < ANALOG_COUNT; i++) {
    data[i] = analogRead(i);
  }
  // Write data to file.  Start with log time in micros.
  file.print(logTime);

  // Write ADC data to CSV record.
  for (uint8_t i = 0; i < ANALOG_COUNT; i++) {
    file.write(',');
    file.print(data[i]);
  }
  file.println();
}

void PrintHex(char ch) {
	file.print(' ');
	if ((ch&0xF0)==0) file.print('0');
	file.print(ch, HEX);
}


//==============================================================================
// Error messages stored in flash.
#define error(msg) sd.errorHalt(F(msg))
//------------------------------------------------------------------------------
void setup() {
  const uint8_t BASE_NAME_SIZE = sizeof(FILE_BASE_NAME) - 1;
  char fileName[13] = FILE_BASE_NAME "00.txt";

  Serial.begin(9600);
  while (!Serial) {} // wait for Leonardo
  delay(1000);

  // Initialize the SD card at SPI_HALF_SPEED to avoid bus errors with
  // breadboards.  use SPI_FULL_SPEED for better performance.
  if (!sd.begin(chipSelect, SPI_HALF_SPEED)) {
    sd.initErrorHalt();
  }

  // Find an unused file name.
  if (BASE_NAME_SIZE > 6) {
    error("FILE_BASE_NAME too long");
  }
  while (sd.exists(fileName)) {
    if (fileName[BASE_NAME_SIZE + 1] != '9') {
      fileName[BASE_NAME_SIZE + 1]++;
    } else if (fileName[BASE_NAME_SIZE] != '9') {
      fileName[BASE_NAME_SIZE + 1] = '0';
      fileName[BASE_NAME_SIZE]++;
    } else {
      error("Can't create file name");
    }
  }
  if (!file.open(fileName, O_CREAT | O_WRITE | O_EXCL)) {
    error("file.open");
  }
  Serial.print(F("Logging to: "));
  Serial.println(fileName);

  LastWrite	= millis();
  pinMode(13, OUTPUT);

}


//------------------------------------------------------------------------------
void loop() {
	static byte LED, State, Checksum, Length;

	
	uint32_t	Now=millis();
	if (Now-LastWrite > 2000) {	// Write data to card when more than 2 seconds inactive
		file.sync();
		LastWrite	= Now;
	}

	while (Serial.available()) {
		byte	ch	= Serial.read();
//		file.println();
//		file.print("State: ");
//		file.print(State);
		if (State==0) {
			LED	= !LED;
			digitalWrite(13, LED);
			file.println();
			file.print(millis());
			PrintHex(ch);
			Checksum	= 0;
			Length		= 0;
			if (ch==0xFE) State++;
		} else {
			if (State==1 || State==2) {
				PrintHex(ch);
			} else if (State==3) {
				Length	= ch;
				PrintHex(ch);
				file.print(' ');
			} else if (State==Length-2) {
				Checksum ^= ch;
				if (Checksum!=0)	{
					file.print(" ** Checksum: ");
					PrintHex(Checksum);
				}
			} else if (State==Length-1) {
				if (ch!=0xFD) {
					file.print(" ** EOL: ");
					PrintHex(ch);
				}
				State=-1;
			} else {
				Checksum ^= ch;
				file.print((char)ch);
			}
			State++;
		}
		LastWrite	= Now;
	}
}
