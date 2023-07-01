#include <Streaming.h>

#include <SPI.h>

#define AS3910_IRQ_MASK_OSC 0x80 /*!< osc frequency stable mask */
#define AS3910_IRQ_MASK_NFC 0x40 /*!< nfc event mask */
#define AS3910_IRQ_MASK_FWL 0x20 /*!< FIFO water level mask */
#define AS3910_IRQ_MASK_RXS 0x10 /*!< end of receive mask */
#define AS3910_IRQ_MASK_TXE 0x8  /*!< end of transmission mask */
#define AS3910_IRQ_MASK_ERR 0x4  /*!< receive error mask */
#define AS3910_IRQ_MASK_CRC 0x2  /*!< crc error mask */
#define AS3910_IRQ_MASK_COL 0x1  /*!< bit collision mask */

const int	OKPin	= A1, LEDPin	= A2;

struct as3909 {
	const int	SS = 10;
	const int	IRQ= A0;
	byte		IrqStatus;
	enum TCmd {
		SetDefault=1,
		Clear,
		TransmitWithCRC=4,
		TransmitWithoutCRC,
		TransmitREQA,
		TransmitWUPA,
		TransmitNFCwithInitialCollisionAvoidance,
		TransmitNFCwithResponseCollisionAvoidance,
		TransmitNFCwithResponseCollisionAvoidanceN0,
		MaskReceiveData=16,
		UnmaskReceiveData,
		ADConvert,
		MeasureRF,
		Squelch,
		ClearSquelch,
		AdjustRegulators,
		CalibrateModulationDepth,
		CalibrateAntenna,
		CheckAntennaResonance,
		ClearRSSI,
		TransparentMode=28
	};
	enum TReg {
		ISOMode=0,
		OperationControl,
		Conf2,
		Conf3,
		Conf4,
		Conf5,
		RcvConf,
		IntMask,
		Int,
		FIFOStatus,
		Collision,
		NXmit0,
		NXmit1,
		ADOut,
		AntennaCalibration,
		ExternalTrim,
		ModulationDepthDefinition,
		ModulationDepthDisplay,
		ModulatedLevel,
		NonModulatedLevel,
		NFCIPFieldDetectionThreshold,
		RegulatorsDisplay,
		RegulatedVoltageDefinition,
		ReceiverStateDisplay
	};
	inline void Act(void)		{ digitalWrite(SS, HIGH);	}
	inline void inAct(void)		{ digitalWrite(SS, LOW);	}
	inline byte IrqActive(void)	{ return digitalRead(IRQ);	}
	byte	read(TReg Register);
	void	write(TReg Register, char Value);
	void	readFIFO(byte NBytes, char *Pt);
	void	writeFIFO(byte NBytes, const char *Pt);
	byte	execute(TCmd command);
	void	begin(void);
	void	DisableInterrupts(byte Interrupt);
	void	EnableInterrupts(byte Interrupt);
	void	TransmitString(const char *s);
	void	ReceiveString(char *s);
	byte	WaitForInterrupt(byte Interrupt, int TimeoutMS);
} nfc;

byte as3909::read(TReg Register)  { 
	byte RetVal; 
	Act(); 
	SPI.transfer(Register | 0x40); 
	RetVal	= SPI.transfer(0);
	inAct();
	return RetVal;
}

void as3909::write(TReg Register, char Value) {
	Act();
	SPI.transfer(Register);
	SPI.transfer(Value);
	inAct();
}

void as3909::readFIFO(byte NBytes, char *Pt) {
	Act();
	SPI.transfer(0xBF);
	while (NBytes--) *Pt++ = SPI.transfer(0);
	inAct();
}

void as3909::writeFIFO(byte NBytes, const char *Pt) {
	Act();
	SPI.transfer(0x80);
	while (NBytes--) SPI.transfer(*Pt++);
	inAct();
}

byte as3909::execute(TCmd Command) {
	Act();
	SPI.transfer(Command | 0xC0);
	inAct();
}

void as3909::begin(void) {
	SPI.begin();
	SPI.beginTransaction(SPISettings(4000000, MSBFIRST, SPI_MODE1));
	
	pinMode(SS, OUTPUT);
	pinMode(IRQ, INPUT);
	inAct();
	Serial << "Disabling chip" << endl;
	execute(SetDefault);
	DisableInterrupts(0xFF);
	EnableInterrupts(AS3910_IRQ_MASK_OSC);
	
	write(Conf2, 0x36);		// 13,56 MHz Xtal
	write(OperationControl, 0x80);	// Enable oscillator and regulator and Target Mode

	if (!WaitForInterrupt(AS3910_IRQ_MASK_OSC, 1000)) {
		Serial << "Oscillator error" << endl;
		DisableInterrupts(AS3910_IRQ_MASK_OSC);
		return;
	}
	DisableInterrupts(AS3910_IRQ_MASK_OSC);

	Serial << "Adjusting regulator: ";
	execute(AdjustRegulators);
	delay(10);
	Serial << (read(RegulatorsDisplay)>>4) << endl;

	EnableInterrupts(AS3910_IRQ_MASK_NFC);
	write(NFCIPFieldDetectionThreshold, 0xF1);		// 
	write(RcvConf, 0x80);		// Enable AGC
	write(ISOMode, 0x80);
	write(OperationControl, 0xC8);
	

}

void as3909::DisableInterrupts(byte Interrupts) {
	write(Int, read(Int) | ~Interrupts);
}

void as3909::EnableInterrupts(byte Interrupts) {
	write(Int, Interrupts | read(Int));
}

void as3909::TransmitString(const char *s) {
	int	Len = strlen(s);
	execute(Clear);
	write(NXmit1, Len>>2);
	write(NXmit0, (Len & 3) <<6);
	writeFIFO(Len, s);
	execute(TransmitNFCwithInitialCollisionAvoidance);
	Serial << s << endl;
	Serial << "Waiting for IRQ:  ";
	while (!IrqActive());
	Serial << _HEX(read(Int)) << endl;
}


void as3909::ReceiveString(char *s) {
	int	Length;
	long	Timeout	= millis() + 100;
	byte	Err;

	*s	= 0;
	//execute(Clear);
	EnableInterrupts(AS3910_IRQ_MASK_RXS | AS3910_IRQ_MASK_ERR | AS3910_IRQ_MASK_NFC);
 	write(OperationControl, 0xC8);	// Hunt for carrier
	if (!WaitForInterrupt(AS3910_IRQ_MASK_NFC, 100));	
	execute(UnmaskReceiveData);
	digitalWrite(LEDPin, HIGH);
	WaitForInterrupt(AS3910_IRQ_MASK_RXS, 100);
	digitalWrite(LEDPin, LOW);

	Length	= read(FIFOStatus) >> 2;
	readFIFO(Length, s);
	s[Length]	= 0;

	execute(MaskReceiveData);
	delay(1);
}

byte as3909::WaitForInterrupt(byte Interrupt, int TimeoutMS) {
	long	timo = millis() + TimeoutMS;

	IrqStatus	= 0;
	while ((long)(millis() - timo) < 0) {
		if (IrqActive()) {
			IrqStatus = read(Int);
			if (IrqStatus & Interrupt) return IrqStatus;
		}
	}
	return 0;
}

void setup() {
	Serial.begin(9600);
	nfc.begin();
	pinMode(LEDPin, OUTPUT);
	pinMode(OKPin, OUTPUT);
}

void loop() {
	char	s[64];
	static char Length;

	nfc.ReceiveString(s);
	if (!strcmp(s, "Hallo beste mensen, een test!!")) {
		digitalWrite(OKPin, 1);
		delay(1);
		digitalWrite(OKPin, 0);
	}
}

