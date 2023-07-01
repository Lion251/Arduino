#ifndef _GAZELLE_H_
#define _GAZELLE_H_

#include <WString.h>

typedef unsigned char byte;

struct TGazelle {
	byte		gCycles;
	unsigned	RSA;
	unsigned	BatVoltage;
	signed		Current;
	unsigned	RemainingCapacity;
	unsigned	TotalCapacity;
	unsigned	Status;
	void		Action(void);
	void		Init(void);
	void		HandleResponse(void);
	void		SendCmd(const __FlashStringHelper *Cmd);
	void		UpdateDisplay(const char *s);
};

#endif
