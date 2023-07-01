#ifndef _GAZELLE_H_
#define _GAZELLE_H_

struct TGazelle {
	byte		gCycles;
	unsigned	RSA;
	void		Action(void);
	void		Charge(byte On);
	void		Discharge(byte On);
	void		DisplayBatteryInfo(void);
	void		IncreaseCycles(void);
	void		Init(void);
	void		HandleResponse(void);
	void		SendCmd(const __FlashStringHelper *Cmd);
	void		UpdateDisplay(const char *s);
};

#endif
