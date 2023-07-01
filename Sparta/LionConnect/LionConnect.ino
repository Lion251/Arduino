#include <SoftwareSerial.h>
#include "Gazelle.h"

TGazelle	Gazelle;

//------------------------------------------------------------------------------
void setup() {
	Gazelle.Init();
}

//------------------------------------------------------------------------------
void loop() {
	Gazelle.Action();
}
