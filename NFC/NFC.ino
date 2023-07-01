#include <Streaming.h>

#include <SPI.h>

struct as3909 {
	const int	SS = 10;
	const int	IRQ= A0;
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
	void	execute(TCmd command);
	void	begin(void);
	void	TransmitString(const char *s);
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

void as3909::execute(TCmd Command) {
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
	write(OperationControl, 0);	// disable chip
	delay(1000);

	write(ISOMode, 0x80);
	write(Conf2, 0x06);		// 13,56 MHz Xtal
	write(NFCIPFieldDetectionThreshold, 0xF1);		// 

	Serial << "Enabling chip" << endl;
	write(OperationControl, 0x80);	// Enable oscillator and regulator

	Serial << "OperationControl: " << _HEX(read(OperationControl)) << endl;
	Serial << "Waiting for IRQ:  ";
	while (!IrqActive());
	Serial << _HEX(read(Int)) << endl;

	Serial << "Adjusting regulator: ";
	execute(AdjustRegulators);
	delay(10);
	Serial << (read(RegulatorsDisplay)>>4) << endl;
}

void as3909::TransmitString(const char *s) {
	int	Len = strlen(s);
	execute(Clear);
	write(NXmit1, Len>>2);
	write(NXmit0, (Len & 3) <<6);
	writeFIFO(Len, s);
	execute(TransmitNFCwithInitialCollisionAvoidance);
//	Serial << s << endl;
//	Serial << "Waiting for IRQ:  ";
	while (!IrqActive()); read(Int);
//	Serial << _HEX(read(Int)) << endl;
}

void setup() {
	Serial.begin(9600);
	nfc.begin();
}

void loop() {
	static byte	i=0;
	nfc.TransmitString("Hallo beste mensen, een test!!");
	delay(5);
	Serial << ".";
	if (++i==80) {
		Serial << endl;
		i=0;
	}
}
